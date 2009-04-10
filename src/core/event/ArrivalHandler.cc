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

/** @file ArrivalHandler.cc
 * Implementation of a basic arrival handler.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#if !defined(NDEBUG_EVENTS) || !defined(NDEBUG_SAMPLING)
# include <iostream>
#endif /* NDEBUG_EVENTS */

#ifndef NDEBUG
# include <cassert>
#endif /* NDEBUG */

#include <cstdlib>

#include <gsl/gsl_math.h>
#include <gsl/gsl_randist.h>

#include "Entry.hh"
#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "WEvonet.hh"
namespace dnet = des::network;

#include "CRN.hh"
namespace dsample = des::sampling;

#include "Stats.hh"
namespace dstats = des::statistics;


#include "events.hh"
#include "ArrivalEvent.hh"
#include "ArrivalHandler.hh"


namespace des
{
namespace core
{


ArrivalHandler::ArrivalHandler(dcommon::tQueueSP p_queue,
                               dnet::tGraphSP p_graph, Int32SA p_service_ids)
    : m_queue(p_queue), m_graph(p_graph), m_service_ids(p_service_ids)
{
    vertex_busy_map = get(vertex_busy, *m_graph);
    vertex_service_map = get(vertex_service_rate, *m_graph);
    vertex_number_in_queue_map = get(vertex_number_in_queue, *m_graph);
    vertex_time_service_ends_map = get(vertex_time_service_ends, *m_graph);
    vertex_num_events_map = get(vertex_num_events, *m_graph);
    vertex_average_delay_in_queue_map = get(vertex_average_delay_in_queue, *m_graph);

#ifndef NDEBUG_SAMPLING
    std::cout << "Service seed: ";
    for (boost::uint32_t i = 0; i < boost::num_vertices(*m_graph); ++i) {
        std::cout << m_service_ids[i] << ", ";
    }
    std::cout << std::endl << std::cout.flush();
#endif /* NDEBUG_SAMPLING */
}


ArrivalHandler::~ArrivalHandler()
{}


void ArrivalHandler::update(ArrivalEvent *subject)
{
    dcommon::Entry *entry;
    dnet::Vertex vertex;
    double service_time;
    double departure;
    double delay = 0.0;

    entry = subject->getEvent();
    vertex = boost::vertex(entry->getDestination(), *m_graph);

    try {
        dsample::tGslRngSP service_rng = dsample::CRN::getInstance().get(
            m_service_ids[entry->getDestination()]);
        service_time = gsl_ran_exponential(service_rng.get(),
                                           1 / vertex_service_map[vertex]);
    } catch (dsample::SamplingException &se) {
#ifndef NDEBUG_EVENTS
        std::cout << "Exception: " << se.what() << std::endl;
        std::cout << "Event Processed: "
                  << const_cast <const dcommon::Entry&> (*entry)
                  << std::endl;
#endif /* NDEBUG_EVENTS */

        throw;
    }


    dcommon::Entry *new_entry = new dcommon::Entry(
        const_cast <const dcommon::Entry&> (*entry));

#ifndef NDEBUG_EVENTS
    std::cout << "** Arrival for vertex: " << entry->getDestination() << std::endl;
    std::cout << "Event: " << const_cast <const dcommon::Entry&> (*entry) << std::endl;
#endif /* NDEBUG_EVENTS */

    // if the server is busy then re-schedule
    // otherwise schedule the departure
    if (vertex_busy_map[vertex]) {
        // the new arrival time is that of the time-service-ends
        double delay = 0.0;
        delay = (gsl_fcmp(vertex_time_service_ends_map[vertex], entry->getArrival(), 1e-9) <= 0)
            ? (0.0)
            : (vertex_time_service_ends_map[vertex] - entry->getArrival());

        departure = vertex_time_service_ends_map[vertex] + service_time;
        vertex_number_in_queue_map[vertex]++;

#ifndef NDEBUG_EVENTS
        std::cout << "Busy -- service time: " << service_time
                  << ", Time service ends: " << vertex_time_service_ends_map[vertex]
                  << ",  number in queue: " << vertex_number_in_queue_map[vertex]
                  << std::endl;
#endif /* NDEBUG_EVENTS */

        // delay the event
        new_entry->delayed(delay, departure, DEPARTURE_EVENT);
    } else {
        // enqueue a departure event into the queue with a stochastic service time
        departure = entry->getArrival() + service_time;

#ifndef NDEBUG_EVENTS
        std::cout << "Not busy -- service time: " << service_time << std::endl;
#endif /* NDEBUG_EVENTS */

        // set the busy flag to true
        vertex_busy_map[vertex] = true;

        // service the event
        new_entry->service(departure, DEPARTURE_EVENT);
    }

#ifndef NDEBUG_EVENTS
    std::cout << "Calc average delay -- Num events: " << vertex_num_events_map[vertex]
              << ", average delay in queue: " << vertex_average_delay_in_queue_map[vertex]
              << ", delay: " << delay << std::endl;
#endif /* NDEBUG_EVENTS */
    // calculate the average delay in the queue
    vertex_average_delay_in_queue_map[vertex] =
        dstats::Stats::mean(vertex_num_events_map[vertex],
                            vertex_average_delay_in_queue_map[vertex],
                            delay);

    // set the time the service ends to the arrival time of the scheduled departure event
    vertex_time_service_ends_map[vertex] = new_entry->getArrival();

#ifndef NDEBUG_EVENTS
    std::cout << "Set the time the service ends to: " << vertex_time_service_ends_map[vertex]
              << std::endl;
    std::cout << "Schedule departure event: " << const_cast <const dcommon::Entry&> (*new_entry)
              << std::endl;
#endif /* NDEBUG_EVENTS */

#ifndef NDEBUG
    assert(delay >= 0.0);
#endif /* NDEBUG */

    try {
        m_queue->push(new_entry);
#ifndef NDEBUG_EVENTS
        std::cout << "Departure event scheduled." << std::endl;
#endif /* NDEBUG_EVENTS */
    } catch (dcommon::QueueException &qe) {
        std::cout << "Error scheduling departure event: " << new_entry->getArrival() << " " << qe.what() << std::endl;
        throw;
    }
}


}
}
