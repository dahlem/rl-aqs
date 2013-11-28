// Copyright (C) 2009-2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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


#include "Observer.hh"
namespace design = des::design;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "Policy.hh"
namespace drl = des::rl;

#include "OnlineStats.hh"
namespace dstats = des::statistics;

#include "AckEvent.hh"
#include "DesBus.hh"


namespace des
{
namespace core
{

typedef boost::scoped_array<dstats::OnlineStats> tQOnlineStatsSA;

/** @class RLResponseHandler
 * The class @code{RLResponseHandler} handles the RL update statistic.
 */
class RLResponseHandler : public design::Observer<AckEvent>
{
public:
    RLResponseHandler(DesBus&, double, double, drl::Policy&);

    ~RLResponseHandler();

    void update(AckEvent *subject);

private:
    dnet::Graph &m_graph;
    double m_q_alpha;
    double m_q_lambda;
    drl::Policy &m_policy;
    tQOnlineStatsSA qStatsSA;

    // derived fields
    dnet::EdgeIndexMap edge_index_map;
    dnet::VertexNextActionMap vertex_next_action_map;
    dnet::VertexIndexMap vertex_index_map;
    dnet::EdgeQValueMap edge_q_val_map;
};


}
}


#endif /* __CORE_RLRESPONSEHANDLER_HH__ */
