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

/** @file EpsilonGreedy.cc
 * Implementation of the epsilon greedy function object
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NDEBUG_EVENTS
# include <iostream>
#endif /* NDEBUG_EVENTS */

#include <algorithm>

#include "Policy.hh"
#include "EpsilonGreedy.hh"


namespace des
{
namespace rl
{



EpsilonGreedy::EpsilonGreedy(
    double p_epsilon,
    dsample::tGslRngSP p_epsilon_rng,
    dsample::tGslRngSP p_uniform_rng)
    : m_epsilon(p_epsilon), m_epsilon_rng(p_epsilon_rng), m_uniform_rng(p_uniform_rng)
{}


boost::uint16_t EpsilonGreedy::operator() (
    boost::uint16_t p_source, tValuesVecSP p_values, PAttrSP p_attr)
{
    double epsilon = 0.0;
    boost::uint16_t action = 0;

    if (p_values->size() > 1) {
        std::sort((p_values.get())->begin(), (p_values.get())->end(), val_greater);

        epsilon = gsl_rng_uniform(m_epsilon_rng.get());
        if (epsilon <= (1 - m_epsilon)) {
            // choose the greedy action
            action = p_values->front().first;
#ifndef NDEBUG_EVENTS
            std::cout << "Greedy action: " << action << std::endl;
#endif /* NDEBUG_EVENTS */
        } else {
            // choose randomly among the other ones
            boost::uint16_t index =
                gsl_rng_uniform_int(m_uniform_rng.get(), p_values.get()->size() - 1) + 1;
            action = (*(p_values.get()))[index].first;
#ifndef NDEBUG_EVENTS
            std::cout << "random action: " << action << std::endl;
#endif /* NDEBUG_EVENTS */
        }
    } else if (p_values->size() == 1) {
        action = p_values->front().first;
    }

    return action;
}


}
}
