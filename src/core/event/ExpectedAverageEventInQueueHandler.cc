// Copyright (C) 2008-2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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

#ifndef NDEBUG
# include <cassert>
# include <gsl/gsl_math.h>
#endif /* NDEBUG */

#include "Entry.hh"
namespace dcommon = des::common;

#include "WEvonet.hh"
namespace dnet = des::network;

#include "events.hh"
#include "ExpectedAverageEventInQueueHandler.hh"
#include "GraphChannel.hh"


namespace des
{
namespace core
{


ExpectedAverageEventInQueueHandler::ExpectedAverageEventInQueueHandler(DesBus &p_bus)
    : m_graph((dynamic_cast<GraphChannel&> (p_bus.getChannel(id::GRAPH_CHANNEL))).getGraph())
{
    vertex_Qdt_map = get(vertex_Qdt, m_graph);
    vertex_number_in_queue_map = get(vertex_number_in_queue, m_graph);
    vertex_expected_average_number_event_map = get(vertex_expected_average_number_event, m_graph);
    vertex_last_event_time_map = get(vertex_last_event_time, m_graph);
}


ExpectedAverageEventInQueueHandler::~ExpectedAverageEventInQueueHandler()
{}


void ExpectedAverageEventInQueueHandler::update(PostAnyEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();

    // update the last event time
    if ((entry->getType() == LAST_ARRIVAL_EVENT) ||
        (entry->getType() == ARRIVAL_EVENT) ||
        (entry->getType() == DEPARTURE_EVENT)) {

        dnet::Vertex vertex = boost::vertex(entry->getDestination(), m_graph);
        double q_i = 0.0;
        double num_in_queue = vertex_number_in_queue_map[vertex];

        // since we update the # of events in the queue in the arrival/departure handler
        // we have to take the state of the queue immediately before the event arrived at
        // the server into account
        if ((entry->getType() == LAST_ARRIVAL_EVENT) ||
            (entry->getType() == ARRIVAL_EVENT)) {

            // if the queue had been occupied by at least one event we know that the
            // server had been busy
            if (vertex_number_in_queue_map[vertex] > 0) {
                if ((entry->getArrival() - vertex_last_event_time_map[vertex]) >= 0.0) {
                    q_i = (entry->getArrival() - vertex_last_event_time_map[vertex]);
                    num_in_queue--;
                }
            }
        } else {
            // for all departures we know that the queue was previously busy
            if ((entry->getArrival() - vertex_last_event_time_map[vertex]) >= 0.0) {
                q_i = (entry->getArrival() - vertex_last_event_time_map[vertex]);
                num_in_queue++;
            }
        }

        q_i *= num_in_queue;

#ifndef NDEBUG_EVENTS
        std::cout << "** Update expected avg. event in queue for vertex: " << entry->getDestination() << std::endl;
        std::cout << "Number in queue: " << vertex_number_in_queue_map[vertex];
        std::cout << ", diff: " << (entry->getArrival() - vertex_last_event_time_map[vertex]);
        std::cout << std::endl << "q_i: " << q_i << std::endl;
#endif /* NDEBUG_EVENTS */

        // \hat(q) = \fract{\int_{0}^{T(n)}Q(t)dt}{T(n)}
        // Eq. 1.4 in Simulation, Modeling and Analysis by Law, Kelton
        vertex_Qdt_map[vertex] += q_i;
        vertex_expected_average_number_event_map[vertex] =
            vertex_Qdt_map[vertex] / entry->getArrival();

#ifndef NDEBUG_EVENTS
        std::cout << "Avg. Event in queue: " << vertex_expected_average_number_event_map[vertex] << std::endl;
#endif /* NDEBUG_EVENTS */
    }
}


}
}
