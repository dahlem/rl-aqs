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
# include <boost/foreach.hpp>
#endif /* NDEBUG_EVENTS */

#include <algorithm>

#include "Policy.hh"
#include "EpsilonGreedy.hh"


namespace des
{
namespace rl
{



EpsilonGreedy::EpsilonGreedy(
    dnet::Graph &p_graph,
    double p_epsilon,
    dsample::tGslRngSP p_epsilon_rng,
    dsample::tGslRngSP p_uniform_rng)
    : m_graph(p_graph), m_epsilon(p_epsilon), m_epsilon_rng(p_epsilon_rng),
      m_uniform_rng(p_uniform_rng)
{
    edge_weight_map = get(boost::edge_weight, m_graph);
}


boost::uint16_t EpsilonGreedy::operator() (
    boost::uint16_t p_source, tValuesVec &p_values, PAttr p_attr)
{
    double epsilon = 0.0;
    boost::uint16_t action = 0;

    if (p_values.size() > 1) {
        std::sort(p_values.begin(), p_values.end(), val_greater);

#ifndef NDEBUG_EVENTS
        std::cout << "Consider Action-Value Pairs... " << std::endl;
        BOOST_FOREACH(tValues v, p_values) {
            std::cout << "Action-Value Pair: " << v.first << ", " << v.second << std::endl;
        }
#endif /* NDEBUG_EVENTS */

        epsilon = gsl_rng_uniform(m_epsilon_rng.get());
        if (epsilon <= (1 - m_epsilon)) {
            // choose the greedy action
            action = p_values.front().first;
#ifndef NDEBUG_EVENTS
            std::cout << "Greedy action: " << action << std::endl;
#endif /* NDEBUG_EVENTS */
        } else {
            // choose randomly among the other ones
            boost::uint16_t index =
                gsl_rng_uniform_int(m_uniform_rng.get(), p_values.size() - 1) + 1;
            action = p_values[index].first;
#ifndef NDEBUG_EVENTS
            std::cout << "random action: " << action << std::endl;
#endif /* NDEBUG_EVENTS */
        }

        // set the action probabilities
        double nMinusOne = static_cast<double>(p_values.size()) - 1.0;
        for (boost::uint16_t i = 0; i < p_values.size(); ++i) {
            dnet::Edge edge = boost::edge(
                boost::vertex(p_source, m_graph),
                boost::vertex(p_values[i].first, m_graph),
                m_graph).first;

            if (i == 0) {
                // greedy action probability
                edge_weight_map[edge] = 1.0 - m_epsilon;
            } else {
                // non-greedy action probability
                edge_weight_map[edge] = m_epsilon/nMinusOne;
            }
        }
    } else if (p_values.size() == 1) {
        action = p_values.front().first;
    }

    return action;
}


}
}
