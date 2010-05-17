// Copyright (C) 2009-2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file GradientPolicyLearner.hh
 * Declaration of the methods for the gradient policy learner. The
 * implementation is inspired by Abdallah and Lesser "Multiagent
 * Reinforcement Learning and Self-organization in a Network of Agents". The PAttr
 * structure takes the gradient from an external source
 *
 * @author Dominik Dahlem
 */
#ifndef __DES_RL_GRADIENTPOLICYLEARNER_HH__
#define __DES_RL_GRADIENTPOLICYLEARNER_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */


#include <boost/scoped_array.hpp>

#include <gsl/gsl_randist.h>

#include "CRN.hh"
namespace dsample = des::sampling;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "DesBus.hh"
namespace dcore = des::core;

#include "Policy.hh"


namespace des
{
namespace rl
{

typedef boost::scoped_array <double> tDoubleSA;

class GradientPolicyLearner : public Policy
{
public:
    GradientPolicyLearner(dcore::DesBus&);

    ~GradientPolicyLearner()
        {}

    virtual boost::uint16_t operator() (
        boost::uint16_t p_source, tValuesVec &p_values, PAttr p_attr);

private:
    dnet::Graph &m_graph;
    tDoubleSA m_lastTime;

    // derived fields
    double m_epsilon;
    double m_eta;
    double m_ANeg;
    double m_APos;
    double m_rNeg;
    double m_rPos;

    dnet::EdgeWeightMap edge_weight_map;
    dnet::EdgeEmotionMap edge_emotion_map;
    dnet::EdgeRMinMap edge_rmin_map;
    dnet::EdgeRMaxMap edge_rmax_map;
    dnet::EdgeEPosMap edge_e_pos_map;
    dnet::EdgeENegMap edge_e_neg_map;
    dnet::EdgeIndexMap edge_index_map;
    dnet::EdgeQValueMap edge_q_val_map;

    dsample::tGslRngSP m_uniform_rng;
    dsample::tGslRngSP m_simplex_rng;
};


}
}



#endif /* __DES_RL_GRADIENTPOLICYLEARNER_HH__ */
