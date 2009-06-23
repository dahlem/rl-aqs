// Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

/** @file LhsUtils.hh
 * This header declares the utility methods for LHS sampling in the context of
 * des simulations.
 *
 * @author Dominik Dahlem
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <limits>

#include <boost/cstdint.hpp>

#include "LhsUtils.hh"



namespace des
{
namespace core
{


int LhsUtils::dimensions(tDesArgsSP p_desArgs)
{
    int dims = 0;

    if (p_desArgs->min_size < std::numeric_limits<boost::uint16_t>::max()) {
        dims++;
    }
    if (p_desArgs->min_max_edges < std::numeric_limits<boost::uint16_t>::max()) {
        dims++;
    }
    if (p_desArgs->min_edge_prob < std::numeric_limits<double>::max()) {
        dims++;
    }
    if (p_desArgs->min_boost_arrival < std::numeric_limits<double>::max()) {
        dims++;
    }
    if (p_desArgs->min_boost_edge < std::numeric_limits<double>::max()) {
        dims++;
    }
    if (p_desArgs->min_rl_q_alpha < std::numeric_limits<double>::max()) {
        dims++;
    }
    if (p_desArgs->min_rl_q_lambda < std::numeric_limits<double>::max()) {
        dims++;
    }
    if (p_desArgs->min_rl_policy_epsilon < std::numeric_limits<double>::max()) {
        dims++;
    }
    if (p_desArgs->min_nn_momentum < std::numeric_limits<double>::max()) {
        dims++;
    }

    return dims;
}

int LhsUtils::getNetSizeIndex(tDesArgsSP p_desArgs)
{
    int index = -1;

    if (p_desArgs->min_size < std::numeric_limits<boost::uint16_t>::max()) {
        index++;
    }

    return index;
}


int LhsUtils::getMaxEdgesIndex(tDesArgsSP p_desArgs)
{
    int index = -1;

    if (p_desArgs->min_max_edges < std::numeric_limits<boost::uint16_t>::max()) {
        if (p_desArgs->min_size < std::numeric_limits<boost::uint16_t>::max()) {
            index++;
        }
        if (p_desArgs->min_max_edges < std::numeric_limits<boost::uint16_t>::max()) {
            index++;
        }
    }

    return index;
}


int LhsUtils::getEdgeProbIndex(tDesArgsSP p_desArgs)
{
    int index = -1;

    if (p_desArgs->min_edge_prob < std::numeric_limits<double>::max()) {
        if (p_desArgs->min_size < std::numeric_limits<boost::uint16_t>::max()) {
            index++;
        }
        if (p_desArgs->min_max_edges < std::numeric_limits<boost::uint16_t>::max()) {
            index++;
        }
        if (p_desArgs->min_edge_prob < std::numeric_limits<double>::max()) {
            index++;
        }
    }

    return index;
}


int LhsUtils::getVertexBoostIndex(tDesArgsSP p_desArgs)
{
    int index = -1;

    if (p_desArgs->min_boost_arrival < std::numeric_limits<double>::max()) {
        if (p_desArgs->min_size < std::numeric_limits<boost::uint16_t>::max()) {
            index++;
        }
        if (p_desArgs->min_max_edges < std::numeric_limits<boost::uint16_t>::max()) {
            index++;
        }
        if (p_desArgs->min_edge_prob < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_boost_arrival < std::numeric_limits<double>::max()) {
            index++;
        }
    }

    return index;
}


int LhsUtils::getEdgeBoostIndex(tDesArgsSP p_desArgs)
{
    int index = -1;

    if (p_desArgs->min_boost_edge < std::numeric_limits<double>::max()) {
        if (p_desArgs->min_size < std::numeric_limits<boost::uint16_t>::max()) {
            index++;
        }
        if (p_desArgs->min_max_edges < std::numeric_limits<boost::uint16_t>::max()) {
            index++;
        }
        if (p_desArgs->min_edge_prob < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_boost_arrival < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_boost_edge < std::numeric_limits<double>::max()) {
            index++;
        }
    }

    return index;
}


int LhsUtils::getRLAlphaIndex(tDesArgsSP p_desArgs)
{
    int index = -1;

    if (p_desArgs->min_rl_q_alpha < std::numeric_limits<double>::max()) {
        if (p_desArgs->min_size < std::numeric_limits<boost::uint16_t>::max()) {
            index++;
        }
        if (p_desArgs->min_max_edges < std::numeric_limits<boost::uint16_t>::max()) {
            index++;
        }
        if (p_desArgs->min_edge_prob < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_boost_arrival < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_boost_edge < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_rl_q_alpha < std::numeric_limits<double>::max()) {
            index++;
        }
    }

    return index;
}


int LhsUtils::getRLLambdaIndex(tDesArgsSP p_desArgs)
{
    int index = -1;

    if (p_desArgs->min_rl_q_lambda < std::numeric_limits<double>::max()) {
        if (p_desArgs->min_size < std::numeric_limits<boost::uint16_t>::max()) {
            index++;
        }
        if (p_desArgs->min_max_edges < std::numeric_limits<boost::uint16_t>::max()) {
            index++;
        }
        if (p_desArgs->min_edge_prob < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_boost_arrival < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_boost_edge < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_rl_q_alpha < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_rl_q_lambda < std::numeric_limits<double>::max()) {
            index++;
        }
    }

    return index;
}


int LhsUtils::getRLEpsilonIndex(tDesArgsSP p_desArgs)
{
    int index = -1;

    if (p_desArgs->min_rl_policy_epsilon < std::numeric_limits<double>::max()) {
        if (p_desArgs->min_size < std::numeric_limits<boost::uint16_t>::max()) {
            index++;
        }
        if (p_desArgs->min_max_edges < std::numeric_limits<boost::uint16_t>::max()) {
            index++;
        }
        if (p_desArgs->min_edge_prob < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_boost_arrival < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_boost_edge < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_rl_q_alpha < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_rl_q_lambda < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_rl_policy_epsilon < std::numeric_limits<double>::max()) {
            index++;
        }
    }

    return index;
}


int LhsUtils::getNNMomentumIndex(tDesArgsSP p_desArgs)
{
    int index = -1;

    if (p_desArgs->min_nn_momentum < std::numeric_limits<double>::max()) {
        if (p_desArgs->min_size < std::numeric_limits<boost::uint16_t>::max()) {
            index++;
        }
        if (p_desArgs->min_max_edges < std::numeric_limits<boost::uint16_t>::max()) {
            index++;
        }
        if (p_desArgs->min_edge_prob < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_boost_arrival < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_boost_edge < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_rl_q_alpha < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_rl_q_lambda < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_rl_policy_epsilon < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_nn_momentum < std::numeric_limits<double>::max()) {
            index++;
        }
    }

    return index;
}



}
}
