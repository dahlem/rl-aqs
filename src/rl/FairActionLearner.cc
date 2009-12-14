// Copyright (C) 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This program is free software ; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation	 ; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY	; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program	  ; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

/** @file FairActionLearner.cc
 * Implementation of the epsilon greedy function object
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#if !defined(NDEBUG_FAL) || !defined(NDEBUG_EVENTS)
# include <iostream>
#endif /* NDEBUG_EVENTS */

#include <algorithm>
#include <cmath>
#include <numeric>

#include <boost/foreach.hpp>
#include <boost/shared_array.hpp>

#include <gsl/gsl_math.h>

#include "Simplex.hh"
#include "Vector.hh"
namespace dutils = des::utils;

#include "Policy.hh"
#include "FairActionLearner.hh"


namespace des
{
namespace rl
{


FairActionLearner::FairActionLearner(
    double p_epsilon,
    double p_eta,
    dnet::Graph &p_graph,
    dsample::tGslRngSP p_uniform_rng,
    dsample::tGslRngSP p_simplex_rng)
    : m_epsilon(p_epsilon), m_eta(p_eta), m_graph(p_graph),
      m_uniform_rng(p_uniform_rng), m_simplex_rng(p_simplex_rng)
{
    edge_weight_map = get(boost::edge_weight, m_graph);
}


boost::uint16_t FairActionLearner::operator() (
    boost::uint16_t p_source, tValuesVec &p_values, PAttr p_attr)
{
    int action = -1;

#if !defined(NDEBUG_FAL) || !defined(NDEBUG_EVENTS)
    std::cout << "Consider Action-Value Pairs... " << std::endl;
    BOOST_FOREACH(tValues v, p_values) {
        std::cout << "Action-Value: " << v.first << ", " << v.second << std::endl;
    }
    std::cout << std::endl;
#endif /* !defined(NDEBUG_WPL) || !defined(NDEBUG_EVENTS) */

    if (p_values.size() > 1) {
        boost::shared_array<double> gradient = boost::shared_array<double>(new double[p_values.size()]);
        boost::shared_array<double> orig = boost::shared_array<double>(new double[p_values.size()]);
        double sum = 0.0;

        // 1. calc the mean of the q-values
        double q_mean = 0.0;
        for (boost::uint16_t i = 0; i < p_values.size(); ++i) {
            dnet::Edge edge = boost::edge(
                boost::vertex(p_source, m_graph),
                boost::vertex(p_values[i].first, m_graph),
                m_graph).first;
            orig[i] = edge_weight_map[edge];
            q_mean += p_values[i].second * orig[i];
        }

        // 2. for each action
        for (boost::uint16_t i = 0; i < p_values.size(); ++i) {
            // 2.1. calc the difference between current Q and average Q
            double diff = (p_values[i].second - q_mean);
            gradient[i] = orig[i] + m_eta * diff;
        }

        dutils::Simplex::projectionDuchi(p_values.size(), gradient, 1.0, 0.0, m_simplex_rng);

#if !defined(NDEBUG_FAL) || !defined(NDEBUG_EVENTS)
        double tempSum = 0.0;
        std::cout << "after projection...." << std::endl;
        for (boost::uint16_t i = 0; i < p_values.size(); ++i) {
            tempSum += gradient[i];
            std::cout << "probability: " << gradient[i] << std::endl;
        }
        std::cout << "sum: " << tempSum << std::endl;
#endif /* !defined(NDEBUG_WPL) || !defined(NDEBUG_EVENTS) */

        sum = std::accumulate(gradient.get(), gradient.get() + p_values.size(), 0.0);
        if (gsl_fcmp(sum, 1.0, 1e-6) != 0) {
            double factor = 1.0/sum;
            dutils::Vector::mult(p_values.size(), gradient, factor);
        }

        double nMinusOne = static_cast<double>(p_values.size()) - 1.0;
        double maxEpsilon = 1.0 - nMinusOne * m_epsilon;
        dutils::Vector::scale(p_values.size(), gradient, m_epsilon, maxEpsilon);

#if !defined(NDEBUG_FAL) || !defined(NDEBUG_EVENTS)
        for (boost::uint16_t i = 0; i < p_values.size(); ++i) {
            std::cout << "projected gradient: " << gradient[i] << std::endl;
        }
        std::cout << std::endl;
#endif /* !defined(NDEBUG_WPL) || !defined(NDEBUG_EVENTS) */

        // update the probabilities according to gradient projection
        tValuesVec probabilities(p_values.size());
        for (boost::uint16_t i = 0; i < p_values.size(); ++i) {
            dnet::Edge edge = boost::edge(
                boost::vertex(p_source, m_graph),
                boost::vertex(p_values[i].first, m_graph),
                m_graph).first;
            edge_weight_map[edge] = gradient[i];

            tValues value;
            value.first = p_values[i].first;
            value.second = gradient[i];
            probabilities[i] = value;
        }

        // 5. select action
        std::sort(probabilities.begin(), probabilities.end(), val_greater);

#ifndef NDEBUG_EVENTS
        std::cout << "Consider Action-Probability Pairs... " << std::endl;
        BOOST_FOREACH(tValues v, probabilities) {
            std::cout << "Action-Value Probability: " << v.first << ", " << v.second << std::endl;
        }
#endif /* NDEBUG_EVENTS */

        double u = gsl_rng_uniform(m_uniform_rng.get());
        double temp = 0.0;

        for (tValuesVec::iterator it = probabilities.begin(); it != probabilities.end(); ++it) {
            temp += (*it).second;

#ifndef NDEBUG_EVENTS
            std::cout << "Accum. probability: " << temp << std::endl;
#endif /* NDEBUG_EVENTS */

            if (u < temp) {
                action = (*it).first;
                break;
            }
        }

        // in some rare occasion, the above could finish without finding the destination
        // so we return the last element in the list
        if (action == -1) {
            action = probabilities[probabilities.size() - 1].first;
        }
    } else if (p_values.size() == 1) {
        action = p_values.front().first;
    } else {
        action = 0;
    }

    return static_cast<boost::uint16_t> (action);
}


}
}
