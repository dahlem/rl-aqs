// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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
#include <cstdlib>

#include <gsl/gsl_randist.h>

#include "events.hh"
#include "ArrivalEvent.hh"
#include "ArrivalHandler.hh"
namespace dcore = des::core;

#include "Entry.hh"
#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "WEvonet.hh"
namespace dnet = des::network;

#include "CRN.hh"
namespace dsample = des::sampling;


dcore::ArrivalHandler::ArrivalHandler(dcommon::tQueueSP p_queue,
    dnet::tGraphSP p_graph, boost::uint32_t p_service_idx)
    : m_queue(p_queue), m_graph(p_graph), m_service_idx(p_service_idx)
{
    m_service_rng = dsample::CRN::getInstance().get(m_service_idx - 1);
    vertex_busy_map = get(vertex_busy, *m_graph);
    vertex_service_map = get(vertex_service_rate, *m_graph);
    vertex_number_in_queue_map = get(vertex_number_in_queue, *m_graph);
    vertex_time_service_ends_map = get(vertex_time_service_ends, *m_graph);
}


dcore::ArrivalHandler::~ArrivalHandler()
{}


void dcore::ArrivalHandler::update(dcore::ArrivalEvent *subject)
{
    dcommon::Entry *entry;
    dnet::Vertex vertex;
    double service_time;
    double departure;
    double delay = 0.0;

    entry = subject->getEvent();
    vertex = boost::vertex(entry->getDestination(), *m_graph);

    service_time = gsl_ran_exponential(m_service_rng.get(),
                                       vertex_service_map[vertex]);

    // if the server is busy then re-schedule
    // otherwise schedule the departure
    if (vertex_busy_map[vertex]) {
        // the new arrival time is that of the time-service-ends
        delay = vertex_time_service_ends_map[vertex] - entry->getArrival();
        departure = vertex_time_service_ends_map[vertex] + service_time;
        vertex_number_in_queue_map[vertex]++;
    } else {
        // enqueue a departure event into the queue with a stochastic service time
        departure = entry->getArrival() + service_time;

        // set the busy flag to true
        vertex_busy_map[vertex] = true;
        vertex_number_in_queue_map[vertex] = 1;
    }

    dcommon::Entry *new_entry = new dcommon::Entry(
        entry->getId(),
        entry->getDelay() + delay,
        departure,
        entry->getDestination(),
        entry->getDestination(),
        dcore::DEPARTURE_EVENT);

    m_queue->push(new_entry);
    vertex_time_service_ends_map[vertex] = departure;
}
