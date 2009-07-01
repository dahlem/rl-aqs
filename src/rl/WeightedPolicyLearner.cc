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

/** @file WeightedPolicyLearner.cc
 * Implementation of the epsilon greedy function object
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NDEBUG_EVENTS
# include <iostream>
#endif /* NDEBUG_EVENTS */

#include <algorithm>

#include <boost/foreach.hpp>
#include <boost/shared_array.hpp>

#include "Policy.hh"
#include "WeightedPolicyLearner.hh"


namespace des
{
namespace rl
{


WeightedPolicyLearner::WeightedPolicyLearner(
    double p_epsilon,
    double p_eta,
    dnet::Graph &p_graph,
    dsample::tGslRngSP p_uniform_rng)
    : m_epsilon(p_epsilon), m_eta(p_eta), m_graph(p_graph), m_uniform_rng(p_uniform_rng)
{
    edge_weight_map = get(boost::edge_weight, m_graph);
}


boost::uint16_t WeightedPolicyLearner::operator() (
    boost::uint16_t p_source, tValuesVec &p_values, PAttr p_attr)
{
    double epsilon = 0.0;
    boost::uint16_t action = -1;

#ifndef NDEBUG_EVENTS
    std::cout << "Consider Action-Value Pairs... " << std::endl;
    BOOST_FOREACH(tValues v, p_values) {
        std::cout << "Action-Value: " << v.first << ", " << v.second << std::endl;
    }
#endif /* NDEBUG_EVENTS */

    if (p_values.size() > 1) {
        boost::shared_array<double> diff = boost::shared_array<double>(new double[p_values.size()]);
        boost::shared_array<double> probs = boost::shared_array<double>(new double[p_values.size()]);
        
        // 1. calc the mean of the q-values
        double q_mean = 0.0;
        for (boost::uint16_t i = 0; i < p_values.size(); ++i) {
            q_mean += p_values[i].second;
        }
        q_mean = q_mean / static_cast<double> (p_values.size());
        
        // 2. for each action
        for (boost::uint16_t i = 0; i < p_values.size(); ++i) {
            // 2.1. calc the difference between current Q and average Q
            diff[i] = p_values[i].second - q_mean;
            dnet::Edge edge = boost::edge(
                boost::vertex(p_source, m_graph),
                boost::vertex(p_values[i].first, m_graph),
                m_graph).first;
            double piA = edge_weight_map[edge];
            
            if (diff[i] > 0) {
                // 2.2.1 if diff > 0 then update diff <- diff * (1 - probability_of_action)
                diff[i] = diff[i] * (1 - piA);
            } else {
                // 2.2.2 else diff <- diff * (probability_of_action)
                diff[i] = diff[i] * piA;
            }
            // 2.3. calculate new policy
            probs[i] = piA + m_eta * diff[i];
        }
        
        // 3. check that no action probability is below epsilon
        bool canUpdate = true;
        for (boost::uint16_t i = 0; i < p_values.size(); ++i) {
            if (probs[i] < m_epsilon) {
                canUpdate = false;
                break;
            }
        }

        // 4. update policy
        tValuesVec probabilities(p_values.size());
        if (canUpdate) {
            double length = 0.0;
            
            for (boost::uint16_t i = 0; i < p_values.size(); ++i) {
                length += probs[i];
            }

            // normalise the vector
            // and put the props with the target index into a separate vector
            for (boost::uint16_t i = 0; i < p_values.size(); ++i) {
                dnet::Edge edge = boost::edge(
                    boost::vertex(p_source, m_graph),
                    boost::vertex(p_values[i].first, m_graph),
                    m_graph).first;
                
                double prob = probs[i] / length;
                edge_weight_map[edge] = prob;

                tValues value;
                value.first = p_values[i].first;
                value.second = prob;
                probabilities[i] = value;
            }
        } else {
            // otherwise use the previous vector
            for (boost::uint16_t i = 0; i < p_values.size(); ++i) {
                dnet::Edge edge = boost::edge(
                    boost::vertex(p_source, m_graph),
                    boost::vertex(p_values[i].first, m_graph),
                    m_graph).first;

                tValues value;
                value.first = p_values[i].first;
                value.second = edge_weight_map[edge];
                probabilities[i] = value;
            }
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

    return action;
}


}
}
