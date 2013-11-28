// Copyright (C) 2008, 2009, 2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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
#include <sstream>

#include <boost/cstdint.hpp>

#include "Results.hh"
namespace dio = des::io;

#include "LhsUtils.hh"



namespace des
{
namespace core
{

void LhsUtils::serialiseDesign(tDesArgsSP p_desArgs, const gsl_matrix *p_design, std::string &p_filename, std::string &p_dir)
{
    std::stringstream line;
    dio::Results design(p_filename, p_dir);
    int dims = 0;

    if (p_desArgs->min_size < std::numeric_limits<boost::uint16_t>::max()) {
        dims++;
        line << SIZE;
    }
    if (p_desArgs->min_max_edges < std::numeric_limits<boost::uint16_t>::max()) {
        dims++;
        if (dims > 1) {
            line << "," << MAX_EDGES;
        } else {
            line << MAX_EDGES;
        }
    }
    if (p_desArgs->min_edge_prob < std::numeric_limits<double>::max()) {
        dims++;
        if (dims > 1) {
            line << "," << EDGE_PROB;
        } else {
            line << EDGE_PROB;
        }
    }
    if (p_desArgs->min_boost_arrival < std::numeric_limits<double>::max()) {
        dims++;
        if (dims > 1) {
            line << "," << BOOST_ARRIVAL;
        } else {
            line << BOOST_ARRIVAL;
        }
    }
    if (p_desArgs->min_boost_edge < std::numeric_limits<double>::max()) {
        dims++;
        if (dims > 1) {
            line << "," << BOOST_EDGE;
        } else {
            line << BOOST_EDGE;
        }
    }
    if (p_desArgs->min_rl_q_alpha < std::numeric_limits<double>::max()) {
        dims++;
        if (dims > 1) {
            line << "," << RL_Q_ALPHA;
        } else {
            line << RL_Q_ALPHA;
        }
    }
    if (p_desArgs->min_rl_q_lambda < std::numeric_limits<double>::max()) {
        dims++;
        if (dims > 1) {
            line << "," << RL_Q_LAMBDA;
        } else {
            line << RL_Q_LAMBDA;
        }
    }
    if (p_desArgs->min_rl_policy_epsilon < std::numeric_limits<double>::max()) {
        dims++;
        if (dims > 1) {
            line << "," << RL_POLICY_EPSILON;
        } else {
            line << RL_POLICY_EPSILON;
        }
    }
    if (p_desArgs->min_nn_momentum < std::numeric_limits<double>::max()) {
        dims++;
        if (dims > 1) {
            line << "," << CL_NN_MOMENTUM;
        } else {
            line << CL_NN_MOMENTUM;
        }
    }
    if (p_desArgs->min_rl_policy_wpl_eta < std::numeric_limits<double>::max()) {
        dims++;
        if (dims > 1) {
            line << "," << RL_POLICY_WPL_ETA;
        } else {
            line << RL_POLICY_WPL_ETA;
        }
    }
    if (p_desArgs->min_cognitive_A_pos < std::numeric_limits<double>::max()) {
        dims++;
        if (dims > 1) {
            line << "," << CL_COGNITIVE_A_POS;
        } else {
            line << CL_COGNITIVE_A_POS;
        }
    }
    if (p_desArgs->min_cognitive_A_neg < std::numeric_limits<double>::max()) {
        dims++;
        if (dims > 1) {
            line << "," << CL_COGNITIVE_A_NEG;
        } else {
            line << CL_COGNITIVE_A_NEG;
        }
    }
    if (p_desArgs->min_cognitive_r_pos < std::numeric_limits<double>::max()) {
        dims++;
        if (dims > 1) {
            line << "," << CL_COGNITIVE_R_POS;
        } else {
            line << CL_COGNITIVE_R_POS;
        }
    }
    if (p_desArgs->min_cognitive_r_neg < std::numeric_limits<double>::max()) {
        dims++;
        if (dims > 1) {
            line << "," << CL_COGNITIVE_R_NEG;
        } else {
            line << CL_COGNITIVE_R_NEG;
        }
    }

    // print header
    design.print(line);
    line.str("");

    for (boost::uint32_t i = 0; i < p_design->size1; ++i) {
        for (boost::uint32_t j = 0; j < dims; ++j) {
            if (j > 0) {
                line << "," << gsl_matrix_get(p_design, i, j);
            } else {
                line << gsl_matrix_get(p_design, i, j);
            }
        }

        // print design location
        design.print(line);
        line.str("");
    }
}



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
    if (p_desArgs->min_rl_policy_wpl_eta < std::numeric_limits<double>::max()) {
        dims++;
    }
    if (p_desArgs->min_cognitive_A_pos < std::numeric_limits<double>::max()) {
        dims++;
    }
    if (p_desArgs->min_cognitive_A_neg < std::numeric_limits<double>::max()) {
        dims++;
    }
    if (p_desArgs->min_cognitive_r_pos < std::numeric_limits<double>::max()) {
        dims++;
    }
    if (p_desArgs->min_cognitive_r_neg < std::numeric_limits<double>::max()) {
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


int LhsUtils::getRlWplEtaIndex(tDesArgsSP p_desArgs)
{
    int index = -1;

    if (p_desArgs->min_rl_policy_wpl_eta < std::numeric_limits<double>::max()) {
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
        if (p_desArgs->min_rl_policy_wpl_eta < std::numeric_limits<double>::max()) {
            index++;
        }
    }

    return index;
}


int LhsUtils::getCplAPosIndex(tDesArgsSP p_desArgs)
{
    int index = -1;

    if (p_desArgs->min_cognitive_A_pos < std::numeric_limits<double>::max()) {
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
        if (p_desArgs->min_rl_policy_wpl_eta < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_cognitive_A_pos < std::numeric_limits<double>::max()) {
            index++;
        }
    }

    return index;
}


int LhsUtils::getCplANegIndex(tDesArgsSP p_desArgs)
{
    int index = -1;

    if (p_desArgs->min_cognitive_A_neg < std::numeric_limits<double>::max()) {
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
        if (p_desArgs->min_rl_policy_wpl_eta < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_cognitive_A_pos < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_cognitive_A_neg < std::numeric_limits<double>::max()) {
            index++;
        }
    }

    return index;
}


int LhsUtils::getCplRPosIndex(tDesArgsSP p_desArgs)
{
    int index = -1;

    if (p_desArgs->min_cognitive_r_pos < std::numeric_limits<double>::max()) {
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
        if (p_desArgs->min_rl_policy_wpl_eta < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_cognitive_A_pos < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_cognitive_A_neg < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_cognitive_r_pos < std::numeric_limits<double>::max()) {
            index++;
        }
    }

    return index;
}


int LhsUtils::getCplRNegIndex(tDesArgsSP p_desArgs)
{
    int index = -1;

    if (p_desArgs->min_cognitive_r_neg < std::numeric_limits<double>::max()) {
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
        if (p_desArgs->min_rl_policy_wpl_eta < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_cognitive_A_pos < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_cognitive_A_neg < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_cognitive_r_pos < std::numeric_limits<double>::max()) {
            index++;
        }
        if (p_desArgs->min_cognitive_r_neg < std::numeric_limits<double>::max()) {
            index++;
        }
    }

    return index;
}



}
}
