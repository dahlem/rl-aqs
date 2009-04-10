// Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file DepartureHandler.cc
 * Implementation of a basic departure handler.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NDEBUG_EVENTS
# include <iostream>
#endif /* NDEBUG_EVENTS */

#ifndef NDEBUG
# include <cassert>
#endif /* NDEBUG */

#include <boost/graph/adjacency_list.hpp>

#include "events.hh"
#include "DepartureEvent.hh"
#include "DepartureHandler.hh"
namespace dcore = des::core;

#include "Entry.hh"
#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "WEvonet.hh"
namespace dnet = des::network;

#include "Selection.hh"
namespace drl = des::rl;


dcore::DepartureHandler::DepartureHandler(dcommon::Queue &p_queue,
                                          dnet::Graph &p_graph,
                                          drl::Selection &p_selection)
    : m_queue(p_queue), m_graph(p_graph), m_selection(p_selection)
{
    vertex_busy_map = get(vertex_busy, m_graph);
    vertex_number_in_queue_map = get(vertex_number_in_queue, m_graph);
}


dcore::DepartureHandler::~DepartureHandler()
{}


void dcore::DepartureHandler::update(dcore::DepartureEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();
    dnet::Vertex vertex = boost::vertex(entry->getDestination(), m_graph);


#ifndef NDEBUG_EVENTS
    std::cout << "** Departure for vertex: " << entry->getDestination() << std::endl;
    std::cout << "Event: " << const_cast <const dcommon::Entry&> (*entry) << std::endl;
    std::cout << "Number in queue: " << vertex_number_in_queue_map[vertex]
              << ", busy: " << vertex_busy_map[vertex] << std::endl;
#endif /* NDEBUG_EVENTS */

    // if the server is busy then re-schedule
    // otherwise schedule the departure
    if (vertex_number_in_queue_map[vertex] > 0) {
        vertex_number_in_queue_map[vertex]--;
    }

    if (vertex_number_in_queue_map[vertex] == 0) {
        vertex_busy_map[vertex] = false;
    }

    dnet::Graph::degree_size_type degree =
        boost::out_degree(vertex, m_graph);

    if (degree > 0) {
        dcommon::Entry *new_entry = new dcommon::Entry(
            const_cast <const dcommon::Entry&> (*entry));
        boost::int32_t destination = m_selection(entry->getDestination());

#ifndef NDEBUG
        assert(destination >= 0);
#endif /* NDEBUG_EVENTS */

        new_entry->depart(destination, dcore::ARRIVAL_EVENT);

#ifndef NDEBUG_EVENTS
        std::cout << "Schedule new arrival event: " << const_cast <const dcommon::Entry&> (*new_entry)
                  << std::endl;
#endif /* NDEBUG_EVENTS */

        new_entry->pushEvent(new_entry->getOrigin());

        try {
            m_queue.push(new_entry);
#ifndef NDEBUG_EVENTS
            std::cout << "Arrival event scheduled." << std::endl;
#endif /* NDEBUG_EVENTS */
        } catch (dcommon::QueueException &qe) {
            std::cout << "Error scheduling arrival event: " << new_entry->getArrival() << " " << qe.what() << std::endl;
            delete new_entry;
            throw;
        }
    } else {
        if (entry->isEventQueueEmpty()) {
            // schedule leave event
            dcommon::Entry *new_entry = new dcommon::Entry(
                const_cast <const dcommon::Entry&> (*entry));

            new_entry->leave(dcore::EXTERNAL_EVENT, dcore::LEAVE_EVENT);

#ifndef NDEBUG_EVENTS
            std::cout << "Schedule new leave event: " << const_cast <const dcommon::Entry&> (*new_entry)
                      << std::endl;
#endif /* NDEBUG_EVENTS */

            try {
                m_queue.push(new_entry);
#ifndef NDEBUG_EVENTS
                std::cout << "Leave event scheduled." << std::endl;
#endif /* NDEBUG_EVENTS */
            } catch (dcommon::QueueException &qe) {
                std::cout << "Error scheduling leave event: " << new_entry->getArrival() << " " << qe.what() << std::endl;
                delete new_entry;
                throw;
            }
        } else {
            // schedule ack events
            boost::int32_t origin = entry->getDestination();
            boost::int32_t destination = entry->popEvent();
            dcommon::Entry *new_entry = new dcommon::Entry(
                const_cast <const dcommon::Entry&> (*entry));

            new_entry->acknowledge(origin, destination, dcore::ACK_EVENT);

#ifndef NDEBUG_EVENTS
            std::cout << "Schedule new acknowledge event: " << const_cast <const dcommon::Entry&> (*new_entry)
                      << std::endl;
#endif /* NDEBUG_EVENTS */

            try {
                m_queue.push(new_entry);
#ifndef NDEBUG_EVENTS
                std::cout << "Acknowledge event scheduled." << std::endl;
#endif /* NDEBUG_EVENTS */
            } catch (dcommon::QueueException &qe) {
                std::cout << "Error scheduling acknowledge event: " << new_entry->getArrival() << " " << qe.what() << std::endl;
                delete new_entry;
                throw;
            }
        }
    }
}
