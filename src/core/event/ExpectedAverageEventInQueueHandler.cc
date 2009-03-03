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

/** @file ExpectedAverageEventInQueueHandler.cc
 * Implementation of a basic expectedAverageEventInQueue handler.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NDEBUG_EVENTS
# include <iostream>
#endif /* NDEBUG_EVENTS */

#include "events.hh"
#include "ExpectedAverageEventInQueueHandler.hh"
namespace dcore = des::core;

#include "Entry.hh"
namespace dcommon = des::common;

#include "WEvonet.hh"
namespace dnet = des::network;


dcore::ExpectedAverageEventInQueueHandler::ExpectedAverageEventInQueueHandler(dnet::tGraphSP p_graph)
    : m_graph(p_graph)
{
    vertex_Qdt_map = get(vertex_Qdt, *m_graph);
    vertex_number_in_queue_map = get(vertex_number_in_queue, *m_graph);
    vertex_expected_average_number_event_map = get(vertex_expected_average_number_event, *m_graph);
    vertex_last_event_time_map = get(vertex_last_event_time, *m_graph);
}


dcore::ExpectedAverageEventInQueueHandler::~ExpectedAverageEventInQueueHandler()
{}


void dcore::ExpectedAverageEventInQueueHandler::update(dcore::PostAnyEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();

    // update the last event time
    if ((entry->getType() == LAST_ARRIVAL_EVENT) ||
        (entry->getType() == ARRIVAL_EVENT) ||
        (entry->getType() == DEPARTURE_EVENT)) {

        dnet::Vertex vertex = boost::vertex(entry->getDestination(), *m_graph);
        double q_i = entry->getArrival() - vertex_last_event_time_map[vertex];

#ifndef NDEBUG_EVENTS
        std::cout << "** Update expected avg. event in queue for vertex: " << entry->getDestination() << std::endl;
#endif /* NDEBUG_EVENTS */

        q_i *= vertex_number_in_queue_map[vertex];

        // \hat(q) = \fract{\int_{0}^{T(n)}Q(t)dt}{T(n)}
        // Eq. 1.4 in Simulation, Modeling and Analysis by Law, Kelton
        vertex_Qdt_map[vertex] = vertex_Qdt_map[vertex] + q_i;
        vertex_expected_average_number_event_map[vertex] =
            vertex_Qdt_map[vertex] / entry->getArrival();
    }

#ifndef NDEBUG_EVENTS
    std::cout << "Avg. Event in queue updated." << std::endl;
#endif /* NDEBUG_EVENTS */
}
