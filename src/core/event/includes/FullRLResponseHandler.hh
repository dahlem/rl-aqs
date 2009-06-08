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
#include "CL.hh"
#include "ConjugateGradient.hh"
#include "FeedforwardNetwork.hh"
#include "Identity.hh"
#include "HTangent.hh"
#include "Logistic.hh"
#include "MSE.hh"
#include "Statistics.hh"
namespace dnnet = des::nnet;

#include "AckEvent.hh"


namespace des
{
namespace core
{


typedef dnnet::FeedforwardNetwork <dnnet::HTangent, dnnet::Identity> FFNet;
typedef boost::shared_ptr <FFNet> FFNetSP;

typedef dnnet::MSE <FFNetSP, dnnet::HTangent, dnnet::Identity> ObjMse;
typedef boost::shared_ptr <ObjMse> ObjMseSP;

typedef dnnet::Backpropagation <FFNetSP, ObjMseSP> BackProp;
typedef boost::shared_ptr <BackProp> BackPropSP;

typedef dnnet::ConjugateGradient <FFNetSP, ObjMseSP> ConjGrad;
typedef boost::shared_ptr <ConjGrad> ConjGradSP;


typedef boost::scoped_array <dstats::OnlineStats> tQOnlineStatsSA;
typedef boost::shared_array <double> DoubleSA;

/** @class FullRLResponseHandler
 * The class @code{FullRLResponseHandler} handles the RL update statistic.
 */
class FullRLResponseHandler : public design::Observer<AckEvent>
{
public:
    FullRLResponseHandler(dnet::Graph &p_graph, double p_q_alpha, double p_q_lambda,
                          drl::Policy &p_policy, std::vector<int> &p_state_representation,
                          boost::uint16_t p_hidden_neurons, boost::int32_t p_uniform_rng_index);

    ~FullRLResponseHandler();

    void update(AckEvent *subject);

private:
    dnet::Graph &m_graph;
    double m_q_alpha;
    double m_q_lambda;
    drl::Policy &m_policy;
    std::vector<int> &m_state_representation;
    boost::uint16_t m_hidden_neurons;
    boost::int32_t m_uniform_rng_index;

    tQOnlineStatsSA qStatsSA;

    // derived fields
    dnet::EdgeIndexMap edge_index_map;
    dnet::VertexNextActionMap vertex_next_action_map;
    dnet::VertexIndexMap vertex_index_map;
    dnet::EdgeQValueMap edge_q_val_map;

    // neural networks
    std::vector<FFNetSP> m_nets;
    std::vector<ObjMseSP> m_objectives;
    std::vector<ConjGradSP> m_conjs;
    
    DoubleSA m_inputs;
    DoubleSA m_target;
};


}
}


#endif /* __CORE_FullRLRESPONSEHANDLER_HH__ */
