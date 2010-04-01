// Copyright (C) 2008-2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file AckHandler.cc
 * Implementation of a basic ack handler.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <boost/foreach.hpp>

#include "Entry.hh"
#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "events.hh"
#include "AckEvent.hh"
#include "AckHandler.hh"
#include "GraphChannel.hh"
#include "QueueChannel.hh"
#include "ConfigChannel.hh"


namespace des
{
namespace core
{


AckHandler::AckHandler(DesBus &p_bus)
    : m_graph((dynamic_cast<GraphChannel&> (p_bus.getChannel(id::GRAPH_CHANNEL))).getGraph()),
      m_queue((dynamic_cast<QueueChannel&> (p_bus.getChannel(id::QUEUE_CHANNEL))).getQueue())
{
    dcore::desArgs_t config = (dynamic_cast<ConfigChannel&> (p_bus.getChannel(id::CONFIG_CHANNEL))).getConfig();

    m_collectiveIntelligence = config.rl_ci;
    vertex_index_map = get(boost::vertex_index, m_graph);
}


AckHandler::~AckHandler()
{}


void AckHandler::update(AckEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();
    dnet::Vertex vertex = boost::vertex(entry->getDestination(), m_graph);

#ifndef NDEBUG_EVENTS
    std::cout << "** Acknowledge for vertex: " << entry->getDestination() << std::endl
              << "  Type: " << entry->getType() << std::endl;
#endif /* NDEBUG_EVENTS */

    // schedule ack events
    if (entry->getType() == ACK_EVENT) {
        boost::int32_t origin = entry->getDestination();
        boost::int32_t destination = entry->popEvent();

        dcommon::Entry *new_entry = new dcommon::Entry(*entry);

        if (destination == EXTERNAL_EVENT) {
            new_entry->leave(EXTERNAL_EVENT, LEAVE_EVENT);
        } else {
#ifndef NDEBUG_EVENTS
            std::cout << "Schedule acknowledge event for vertex: " << destination << std::endl;
#endif /* NDEBUG_EVENTS */

            new_entry->acknowledge(origin, destination, ACK_EVENT);
        }

        m_queue.push(new_entry);

        // if collective intelligence is enabled send acknowledgements to the subworld
        if (m_collectiveIntelligence) {
            // only send acknwowledgement, if the event is not an external event
            if (destination != EXTERNAL_EVENT) {
                BOOST_FOREACH(dnet::Edge e, (boost::in_edges(vertex, m_graph))) {
                    int target_vertex = vertex_index_map[boost::source(e, m_graph)];

                    if (target_vertex != destination) {
#ifndef NDEBUG_EVENTS
                        std::cout << "Schedule 1-hop acknowledge event for vertex: " << target_vertex << std::endl;
#endif /* NDEBUG_EVENTS */

                        dcommon::Entry *entry_1hop = new dcommon::Entry(*entry);
                        entry_1hop->acknowledge(origin, target_vertex, ACK_1HOP_EVENT);
                        m_queue.push(entry_1hop);
                    }
                }
            }
        }
    } // we ignore the ACK_1HOP_EVENT events

#ifndef NDEBUG_EVENTS
    std::cout << "** Update the events processed." << std::endl;
#endif /* NDEBUG_EVENTS */

#ifndef NDEBUG_EVENTS
    std::cout << "** Acknowledge handler done." << std::endl;
#endif /* NDEBUG_EVENTS */
}


}
}
