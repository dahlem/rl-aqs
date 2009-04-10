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

/** @file BoltzmannPolicy.cc
 * Implementation of the epsilon greedy function object
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NDEBUG_EVENTS
# include <iostream>
#endif /* NDEBUG_EVENTS */

#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>

#include <boost/foreach.hpp>

#include <gsl/gsl_math.h>

#include "BoltzmannPolicy.hh"


namespace des
{
namespace rl
{



BoltzmannPolicy::BoltzmannPolicy(
    double p_tau,
    dsample::tGslRngSP p_uniform_rng)
    : m_tau(p_tau), m_uniform_rng(p_uniform_rng)
{}


boost::uint16_t BoltzmannPolicy::operator() (
    boost::uint16_t p_source, tValuesVecSP p_values, PAttrSP p_attr)
{
#ifndef NDEBUG_EVENTS
    std::cout << "** Boltzmann Policy" << std::endl;
#endif /* NDEBUG_EVENTS */

    boost::int16_t action = -1;

    if (p_values->size() > 1) {
        double tau = m_tau;

        if (p_attr.get() != NULL) {
            tau = p_attr->tau;
        }

        // calculate the exps first
        std::vector<double> exps;
        BOOST_FOREACH(tValues v, (*(p_values.get()))) {
            exps.push_back(exp(v.second/tau));
        }

        // calculate the denominator, i.e. sum of all exps
        double denominator = accumulate(exps.begin(), exps.end(), 0);

        tValuesVec probabilities;
        for (boost::uint16_t i = 0; i < exps.size(); ++i) {
            tValues prob;

            prob.first = (*(p_values.get()))[i].first;
            prob.second = exps[i]/denominator;
            probabilities.push_back(prob);
        }

        // sort the vector in descending order
        std::sort(probabilities.begin(), probabilities.end(), val_greater);

        double u = gsl_rng_uniform(m_uniform_rng.get());
        double temp = 0.0;

        for (tValuesVec::iterator it = probabilities.begin(); it != probabilities.end(); ++it) {
            temp += (*it).second;

#ifndef NDEBUG_EVENTS
            std::cout << "Accum. probability: " << temp << std::endl;
#endif /* NDEBUG_EVENTS */

            if (gsl_fcmp(u, temp, 1e-9) == -1) {
                action = (*it).first;
                break;
            }
        }

        // in some rare occasion, the above could finish without finding the destination
        // so we return the last element in the list
        if (action == -1) {
            action = probabilities[probabilities.size() - 1].first;
        }
    } else if (p_values->size() == 1) {
        action = p_values->front().first;
    }
#ifndef NDEBUG_EVENTS
    std::cout << "Destination: " << action << std::endl;
#endif /* NDEBUG_EVENTS */

    return action;
}


}
}
