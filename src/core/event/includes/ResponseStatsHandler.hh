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

/** @file ResponseStatsHandler.hh
 * This header file specifies the ack handler subject.
 */
#ifndef __DELAYSTATSHANDLER_HH__
#define __DELAYSTATSHANDLER_HH__


#include "Observer.hh"
namespace design = des::design;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "AckEvent.hh"


namespace des
{
namespace core
{

/** @class ResponseStatsHandler
 * The class @code{ResponseStatsHandler} handles the update the delay statistic.
 */
class ResponseStatsHandler : public design::Observer<AckEvent>
{
public:
    ResponseStatsHandler(dnet::Graph &p_graph);
    ~ResponseStatsHandler();

    void update(AckEvent *subject);

private:
    dnet::Graph &m_graph;

    // derived fields
    dnet::VertexMeanRewardMap vertex_mean_reward_map;
    dnet::VertexNumEventsProcessedMap vertex_num_events_processed_map;
};



}
}


#endif /* __CORE_DELAYSTATSHANDLER_HH__ */
