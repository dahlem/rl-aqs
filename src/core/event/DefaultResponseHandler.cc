// Copyright (C) 2009-2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file DefaultResponseHandler.cc
 * Implementation of a basic RLResponse handler.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NDEBUG_EVENTS
# include <iostream>
#endif /* NDEBUG_EVENTS */

#include <boost/foreach.hpp>

#include "Entry.hh"
namespace dcommon = des::common;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "events.hh"
#include "AckEvent.hh"
#include "DefaultResponseHandler.hh"
#include "GraphChannel.hh"


namespace des
{
namespace core
{


DefaultResponseHandler::DefaultResponseHandler(DesBus &p_bus)
    : m_graph((dynamic_cast<GraphChannel&> (p_bus.getChannel(id::GRAPH_CHANNEL))).getGraph()),
      qStatsSA(new dstats::OnlineStats[boost::num_edges(m_graph)])
{
    edge_index_map = get(edge_eindex, m_graph);
    edge_q_val_map = get(edge_q_val, m_graph);
}


DefaultResponseHandler::~DefaultResponseHandler()
{}


void DefaultResponseHandler::update(AckEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();

#ifndef NDEBUG_EVENTS
    std::cout << "** DefaultResponse for vertex: " << entry->getDestination() << std::endl;
    std::cout << "Event: " << const_cast <const dcommon::Entry&> (*entry) << std::endl;
#endif /* NDEBUG_EVENTS */

    dnet::Vertex vertex = boost::vertex(entry->getDestination(), m_graph);
    dnet::Graph::degree_size_type degree = boost::out_degree(vertex, m_graph);

    if (degree > 0) {
        // observe RTT (the longer it takes the smaller the reward)
        double roundTripTime = entry->topArrival() - entry->getArrival();
        dnet::Edge e = boost::edge(
            vertex, boost::vertex(entry->getOrigin(), m_graph), m_graph).first;

        // calculate q-value
        boost::uint16_t edge_index = edge_index_map[e];
        qStatsSA[edge_index].push(roundTripTime);

#ifndef NDEBUG_EVENTS
        std::cout << "Old Q-value: " << edge_q_val_map[e]
                  << ", new Q-value: " << qStatsSA[edge_index].mean() << std::endl;
#endif /* NDEBUG_EVENTS */

        edge_q_val_map[e] = qStatsSA[edge_index].mean();
    }
}


}
}
