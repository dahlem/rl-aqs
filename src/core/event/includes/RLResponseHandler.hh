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

/** @file RLResponseHandler.hh
 * This header file specifies the ack handler subject.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __CORE_RLRESPONSEHANDLER_HH__
#define __CORE_RLRESPONSEHANDLER_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>
#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>


#include "Observer.hh"
namespace design = des::design;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "Policy.hh"
namespace drl = des::rl;


#include "AckEvent.hh"


namespace des
{
namespace core
{

typedef boost::scoped_array <double> tDoubleVecSP;


/** @class RLResponseHandler
 * The class @code{RLResponseHandler} handles the RL update statistic.
 */
class RLResponseHandler : public design::Observer<AckEvent>
{
public:
    RLResponseHandler(dnet::tGraphSP p_graph, double p_q_alpha, double p_q_lambda,
                      drl::tPolicySP p_policy);

    ~RLResponseHandler();

    void update(AckEvent *subject);

private:
    dnet::tGraphSP m_graph;
    double m_q_alpha;
    double m_q_lambda;
    drl::tPolicySP m_policy;
    tDoubleVecSP m_actionValues;

    // derived fields
    dnet::EdgeIndexMap edge_index_map;
    dnet::VertexNextActionMap vertex_next_action_map;
    dnet::VertexIndexMap vertex_index_map;
    dnet::EdgeQValueMap edge_q_val_map;

};


/** @typedef tRLResponseHandlerSP
 * a type definition of the shared pointer of the ack handler
 */
typedef boost::shared_ptr <RLResponseHandler> tRLResponseHandlerSP;


}
}


#endif /* __CORE_RLRESPONSEHANDLER_HH__ */
