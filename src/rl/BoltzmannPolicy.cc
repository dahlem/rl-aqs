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

/** @class OREqZero
 * Find out whether an element in a sequence evaluates to zero
 */
class OREqZero {
private:
    bool m_result;

public:
    OREqZero () : m_result(false)
        {}

    /** @fn void operator() (int elem)
     * Process an element of the sequence and evaluate whether it is equal to zero
     */
    void operator() (tValues elem)
        {
            m_result = m_result || (elem.second == 0.0);
        }

    /** @fn bool value()
     * @return return the result of the logical OR of all elements evaluated to zero
     */
    bool value() {
        return m_result;
    }
};



BoltzmannPolicy::BoltzmannPolicy(
    double p_tau,
    dsample::tGslRngSP p_uniform_rng)
    : m_tau(p_tau), m_uniform_rng(p_uniform_rng)
{}


boost::uint16_t BoltzmannPolicy::operator() (
    boost::uint16_t p_source, tValuesVec &p_values, PAttr p_attr)
{
#ifndef NDEBUG_EVENTS
    std::cout << "** Boltzmann Policy" << std::endl;
#endif /* NDEBUG_EVENTS */

    boost::int16_t action = -1;

    if (p_values.size() > 1) {
        double tau = m_tau;
        tValuesVec probabilities;

        OREqZero orEqZero = std::for_each(p_values.begin(), p_values.end(), OREqZero());

        // if one element in the vector is zero, then we can't log transform
        if (orEqZero.value()) {
            double equalProb = 1.0 / p_values.size();

            for (boost::uint16_t i = 0; i < p_values.size(); ++i) {
                tValues prob;

                prob.first = p_values[i].first;
                prob.second = equalProb;
                probabilities.push_back(prob);
            }
        } else {
//         tau = p_attr->tau;

            // calculate the exps first
            std::vector<double> exps;
            BOOST_FOREACH(tValues v, p_values) {
                // log-transform the v.second value
                double logTransform = -log10(-v.second);
                exps.push_back(exp(logTransform/tau));
            }

            // calculate the denominator, i.e. sum of all exps
            double denominator = accumulate(exps.begin(), exps.end(), 0.0);

            for (boost::uint16_t i = 0; i < exps.size(); ++i) {
                tValues prob;

                prob.first = p_values[i].first;
                prob.second = exps[i]/denominator;
                probabilities.push_back(prob);
            }
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
    }
#ifndef NDEBUG_EVENTS
    std::cout << "Destination: " << action << std::endl;
#endif /* NDEBUG_EVENTS */

    return action;
}


}
}
