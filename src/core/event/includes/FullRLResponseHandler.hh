// Copyright (C) 2009, 2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file FullRLResponseHandler.hh
 * This header file specifies the ack handler subject.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __CORE_FullRLRESPONSEHANDLER_HH__
#define __CORE_FullRLRESPONSEHANDLER_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <vector>

#include <boost/cstdint.hpp>
#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>


#include "Observer.hh"
namespace design = des::design;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "Policy.hh"
namespace drl = des::rl;

#include "OnlineStats.hh"
namespace dstats = des::statistics;

#include "Backpropagation.hh"
#include "ConjugateGradient.hh"
#include "DefaultLossPolicy.hh"
#include "FeedforwardNetwork.hh"
#include "Identity.hh"
#include "HTangent.hh"
#include "Logistic.hh"
#include "LossPolicy.hh"
#include "MSE.hh"
#include "nnet.hh"
#include "NNetFactory.hh"
#include "Statistics.hh"
#include "SlidingWindowLossPolicy.hh"
#include "Training.hh"
namespace dnnet = des::nnet;

#include "AckEvent.hh"
#include "DesBus.hh"


namespace des
{
namespace core
{

typedef boost::scoped_array <dstats::OnlineStats> tQOnlineStatsSA;

typedef dnnet::FeedforwardNetwork <dnnet::HTangent, dnnet::Identity> FFNet;
typedef boost::shared_ptr <FFNet> FFNetSP;

typedef dnnet::MSE <FFNetSP, dnnet::HTangent, dnnet::Identity> ObjMse;
typedef boost::shared_ptr <ObjMse> ObjMseSP;

typedef dnnet::Backpropagation <FFNetSP, ObjMseSP> BackProp;
typedef boost::shared_ptr <BackProp> BackPropSP;

typedef dnnet::ConjugateGradient <FFNetSP, ObjMseSP> ConjGrad;
typedef boost::shared_ptr <ConjGrad> ConjGradSP;


/** @class FullRLResponseHandler
 * The class @code{FullRLResponseHandler} handles the RL update statistic.
 */
class FullRLResponseHandler : public design::Observer<AckEvent>
{
public:
    FullRLResponseHandler(DesBus&, drl::Policy&, double, double, double);
    ~FullRLResponseHandler();

    void update(AckEvent *subject);

private:
    dnet::Graph &m_graph;
    double m_q_lambda;
    drl::Policy &m_policy;
    std::vector<int> &m_state_representation;
    bool m_outsource;
    bool m_regret_total;
    bool m_incentive_deviate;
    bool m_nn_loss_serialise;

    tQOnlineStatsSA qStatsSA;

    // derived fields
    dnet::EdgeIndexMap edge_index_map;
    dnet::VertexNextActionMap vertex_next_action_map;
    dnet::VertexIndexMap vertex_index_map;
    dnet::VertexActualRewardMap vertex_actual_reward_map;
    dnet::VertexRegretAbsoluteMap vertex_regret_total_map;
    dnet::VertexIncentiveDeviateMap vertex_incentive_deviate_map;
    dnet::VertexNNLossMap vertex_nn_loss_map;
    dnet::VertexNNLossCVMap vertex_nn_loss_cv_map;
    dnet::EdgeQValueMap edge_q_val_map;
    dnet::EdgeTotalRewardMap edge_total_reward_map;
    dnet::EdgeNNLossMap edge_nn_loss_map;
    dnet::EdgeNNLossCVMap edge_nn_loss_cv_map;

    // neural networks
    std::vector<FFNetSP> m_nets;
    std::vector<ObjMseSP> m_objectives;
    std::vector<dnnet::TrainingSP> m_trainings;

    DoubleSA m_inputs;
    DoubleSA m_target;
};


}
}


#endif /* __CORE_FullRLRESPONSEHANDLER_HH__ */
