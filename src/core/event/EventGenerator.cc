// Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>

#include <gsl/gsl_randist.h>

#include "CRN.hh"
#include "Rng.hh"
namespace dsample = des::sampling;

#include "Entry.hh"
#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "events.hh"
#include "EventGenerator.hh"
namespace dcore = des::core;

#include "DirectedGraph.hh"
namespace dnet = des::network;



void dcore::EventGenerator::generate(
    dnet::Graph &p_graph,
    dcommon::Queue &p_queue,
    dsample::tGslRngSP arrival_rng,
    boost::int32_t destination,
    double arrival_rate,
    double stop_time)
{
    generate(p_graph, p_queue, arrival_rng, destination,
             arrival_rate, 0.0, stop_time);
}


void dcore::EventGenerator::generate(
    dnet::Graph &p_graph,
    dcommon::Queue &p_queue,
    dsample::tGslRngSP arrival_rng,
    boost::int32_t destination,
    double arrival_rate,
    double start_time,
    double stop_time)
{
    double cur_arrival = 0.0, new_arrival = 0.0;

    // if start time is larger than 0, then we assume this
    // time is unprocessed (last event that wasn't put into the queue in
    // a previous event generation iteration).
    if (start_time > 0.0) {
        if (start_time <= stop_time) {
            dcommon::Entry *entry = new dcommon::Entry(
                0.0,
                start_time,
                destination,
                dcore::EXTERNAL_EVENT,
                dcore::ARRIVAL_EVENT);

            p_queue.push(entry);
        }
    }

    cur_arrival = -dsample::Rng::poiss(
        arrival_rate, gsl_rng_uniform(arrival_rng.get()));

    cur_arrival += start_time;

    // for as long as there is no stopping event
    while (cur_arrival < stop_time) {
        // generate arrival events
        new_arrival = dsample::Rng::poiss(
            arrival_rate, gsl_rng_uniform(arrival_rng.get()));

        if ((cur_arrival - new_arrival) <= stop_time) {
            // enqueue the last arrival event
            dcommon::Entry *entry = new dcommon::Entry(
                0.0,
                cur_arrival,
                destination,
                dcore::EXTERNAL_EVENT,
                dcore::ARRIVAL_EVENT);

            try {
                p_queue.push(entry);
#ifndef NDEBUG_EVENTS
                std::cout << "External arrival event scheduled for vertex "
                          << destination << std::endl;
#endif /* NDEBUG_EVENTS */
            } catch (dcommon::QueueException &qe) {
                std::cout << "Error scheduling external arrival event: " << entry->getArrival() << " " << qe.what() << std::endl;
                if (entry != NULL) {
                    delete entry;
                }
                throw;
            }
            cur_arrival -= new_arrival;
        } else {
            dnet::VertexNextEventTimeMap vertex_next_event_time_map =
                get(vertex_next_event_time, p_graph);
            dnet::Vertex vertex = boost::vertex(destination, p_graph);

            // enqueue the last arrival event
            dcommon::Entry *entry = new dcommon::Entry(
                0.0,
                cur_arrival,
                destination,
                dcore::EXTERNAL_EVENT,
                dcore::LAST_ARRIVAL_EVENT);

            try {
                p_queue.push(entry);
#ifndef NDEBUG_EVENTS
                std::cout << "External last arrival event scheduled for vertex "
                          << destination << std::endl;
#endif /* NDEBUG_EVENTS */
            } catch (dcommon::QueueException &qe) {
                std::cout << "Error scheduling external last arrival event: " << entry->getArrival() << " " << qe.what() << std::endl;
                if (entry != NULL) {
                    delete entry;
                }
                throw;
            }

            // store the event that could not be pushed into the queue
            // if event phases are enabled this value will be picked up.
            // (see first if statement)
            vertex_next_event_time_map[vertex] = (cur_arrival - new_arrival);

            break;
        }
    }
}


void dcore::EventGenerator::generate(
    dcommon::Queue &p_queue,
    dsample::tGslRngSP arrival_rng,
    boost::int32_t destination,
    double arrival_rate)
{
    double cur_arrival = -dsample::Rng::poiss(
        arrival_rate, gsl_rng_uniform(arrival_rng.get()));

    // enqueue the last arrival event
    dcommon::Entry *entry = new dcommon::Entry(
        0.0,
        cur_arrival,
        destination,
        dcore::EXTERNAL_EVENT,
        dcore::ARRIVAL_EVENT);

    try {
        p_queue.push(entry);
#ifndef NDEBUG_EVENTS
        std::cout << "External arrival event scheduled for vertex " << destination << std::endl;
#endif /* NDEBUG_EVENTS */
    } catch (dcommon::QueueException &qe) {
        std::cout << "Error scheduling external arrival event: " << entry->getArrival() << " " << qe.what() << std::endl;
        if (entry != NULL) {
            delete entry;
        }
        throw;
    }
}


void dcore::EventGenerator::generateLogGraph(
    dcommon::Queue &p_queue,
    double rate,
    double stop_time)
{
    double time = 0.0;

    for (; time <= stop_time; time += rate) {
        dcommon::Entry *entry = new dcommon::Entry(
            0.0,
            time,
            -99,
            dcore::ADMIN_EVENT,
            dcore::LOG_GRAPH_EVENT);

        try {
            p_queue.push(entry);
#ifndef NDEBUG_EVENTS
            std::cout << "Admin log graph event scheduled." << std::endl;
#endif /* NDEBUG_EVENTS */
        } catch (dcommon::QueueException &qe) {
            std::cout << "Error scheduling admin log graph event: " << entry->getArrival() << " " << qe.what() << std::endl;
            if (entry != NULL) {
                delete entry;
            }
            throw;
        }
    }
}
