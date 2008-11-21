// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <iostream>
#include <utility>

#include <boost/cstdint.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/shared_ptr.hpp>

#include <gsl/gsl_randist.h>

#include "AnyEvent.hh"
#include "ArrivalEvent.hh"
#include "ArrivalHandler.hh"
#include "CL.hh"
#include "DepartureEvent.hh"
#include "DepartureHandler.hh"
#include "events.hh"
#include "EventGenerator.hh"
#include "EventProcessor.hh"
#include "GenerateEventHandler.hh"
#include "LastArrivalEvent.hh"
#include "PostEvent.hh"
#include "ProcessedEventsHandler.hh"
#include "UnprocessedEventsHandler.hh"
namespace dcore = des::core;

#include "Results.hh"
namespace dio = des::io;

#include "CRN.hh"
#include "Seeds.hh"
namespace dsample = des::sampling;

#include "Entry.hh"
#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "WEvonet.hh"
#include "GraphException.hh"
namespace dnet = des::network;




int main(int argc, char *argv[])
{
    dcore::tDesArgsSP desArgs(new dcore::desArgs_t);
    dcommon::tQueueSP queue(new dcommon::LadderQueue);
    dcore::CL cl;


    if (cl.parse(argc, argv, desArgs)) {
        return EXIT_SUCCESS;
    }

    dnet::tGraphSP graph(new dnet::Graph);

    if (desArgs->graph_filename != "") {
        // read the graph
        try {
            dnet::WEvonet::read(graph, desArgs->graph_filename, dnet::WEvonet::GRAPHML);
        } catch (dnet::GraphException &ge) {
            std::cerr << "Error: Cannot open graph file " << desArgs->graph_filename
                      << "!" << std::endl;
        }
    } else {
        // generate the graph
        std::cout << "Generate the graph " << std::endl;
    }

    boost::int32_t arrival_rng_index;
    boost::int32_t service_rng_index;
    boost::int32_t depart_uniform_rng_index;
    boost::int32_t seeds_rng_index;
    boost::uint32_t seed = 0;

    if (desArgs->seeds_filename != "") {
        // read the seeds
        dsample::Seeds::getInstance().init(desArgs->seeds_filename.c_str());

        // init the crn for the arrival events
        seed = dsample::Seeds::getInstance().getSeed();
        arrival_rng_index = dsample::CRN::getInstance().init(seed);
        dsample::CRN::getInstance().log(seed, "arrival events");

        // init the crn for the service events
        seed = dsample::Seeds::getInstance().getSeed();
        service_rng_index = dsample::CRN::getInstance().init(seed);
        dsample::CRN::getInstance().log(seed, "service events");

        // init the crn for the departure uniform rv
        seed = dsample::Seeds::getInstance().getSeed();
        depart_uniform_rng_index = dsample::CRN::getInstance().init(seed);
        dsample::CRN::getInstance().log(seed, "departure uniform");
    } else {
        // generate the seeds
        std::cout << "Use random number to generate seeds." << std::endl;

        // 1. init the random number generator for the seeds
        seeds_rng_index = dsample::CRN::getInstance().init(gsl_rng_default_seed);
        dsample::CRN::getInstance().log(gsl_rng_default_seed, "seeds");

        // 2. init the crn for the arrival events
        dsample::tGslRngSP seeds_rng = dsample::CRN::getInstance().get(seeds_rng_index - 1);
        seed = gsl_rng_uniform_int(seeds_rng.get(), gsl_rng_max(seeds_rng.get()));
        arrival_rng_index = dsample::CRN::getInstance().init(seed);
        dsample::CRN::getInstance().log(seed, "arrival events");

        // 3. init the crn for the service events
        seed = gsl_rng_uniform_int(seeds_rng.get(), gsl_rng_max(seeds_rng.get()));
        service_rng_index = dsample::CRN::getInstance().init(seed);
        dsample::CRN::getInstance().log(seed, "service events");

        // 4. init the crn for the departure uniform rv
        seed = gsl_rng_uniform_int(seeds_rng.get(), gsl_rng_max(seeds_rng.get()));
        depart_uniform_rng_index = dsample::CRN::getInstance().init(seed);
        dsample::CRN::getInstance().log(seed, "departure uniform");
    }

    dsample::tGslRngSP arrival_rng = dsample::CRN::getInstance().get(arrival_rng_index - 1);

    dnet::VertexIndexMap vertex_index_props_map =
        get(boost::vertex_index, *graph);
    dnet::VertexArrivalRateMap vertex_arrival_props_map =
        get(vertex_arrival_rate, *graph);

    // generate events for each vertex in the graph
    double stopTime;

    // find out whether we only generate the events in phases
    if (desArgs->generations < 0) {
        stopTime = desArgs->stop_time;
    } else {
        // calculate the phases
        stopTime = desArgs->stop_time / desArgs->generations;
    }

    boost::int32_t destination;
    double arrival_rate;

    // generate events over this graph
    if (desArgs->trace_event) {
        int count = 0;

        // filter the graph to find the vertex
        std::pair <dnet::VertexIterator, dnet::VertexIterator> v_iter;
        v_iter = boost::vertices(*graph);

        typedef boost::filter_iterator<dnet::exists_vertex_index<dnet::VertexIndexMap>, dnet::VertexIterator>
            FilterIter;

        dnet::exists_vertex_index<dnet::VertexIndexMap>
            predicate(vertex_index_props_map, desArgs->vertex);
        FilterIter filter_iter_first(predicate, v_iter.first, v_iter.second);
        FilterIter filter_iter_last(predicate, v_iter.second, v_iter.second);

        for (; filter_iter_first != filter_iter_last; ++filter_iter_first) {
            if (count == 0) {
                // generate a single event
                destination = vertex_index_props_map[*filter_iter_first];
                arrival_rate = vertex_arrival_props_map[*filter_iter_first];

                dcore::EventGenerator::generate(
                    queue, arrival_rng, destination, arrival_rate);
            } else {
                std::cout << "Error: Expected a single vertex to be traced!" << std::endl;
                break;
            }

            count++;
        }
    } else {
        std::pair <dnet::VertexIterator, dnet::VertexIterator> p;
        for (p = boost::vertices(*graph); p.first != p.second; ++p.first) {
            destination = vertex_index_props_map[*p.first];
            arrival_rate = vertex_arrival_props_map[*p.first];

            dcore::EventGenerator::generate(
                queue, arrival_rng, destination, arrival_rate, stopTime);
        }
    }

    dio::tResultsSP processed_events(
        new dio::Results(desArgs->events_processed, desArgs->results_dir));
    dio::tResultsSP unprocessed_events(
        new dio::Results(desArgs->events_unprocessed, desArgs->results_dir));

    // instantiate the events & handlers
    dcore::tAnyEventSP anyEvent(new dcore::AnyEvent);
    dcore::tArrivalEventSP arrivalEvent(new dcore::ArrivalEvent);
    dcore::tDepartureEventSP departureEvent(new dcore::DepartureEvent);
    dcore::tPostEventSP postEvent(new dcore::PostEvent);
    dcore::tLastArrivalEventSP lastArrivalEvent(new dcore::LastArrivalEvent);

    dcore::tProcessedEventsHandlerSP processedEventsHandler(
        new dcore::ProcessedEventsHandler(processed_events));
    dcore::tUnprocessedEventsHandlerSP unprocessedEventsHandler(
        new dcore::UnprocessedEventsHandler(unprocessed_events, queue));
    dcore::tArrivalHandlerSP arrivalHandler(
        new dcore::ArrivalHandler(queue, graph, service_rng_index));
    dcore::tDepartureHandlerSP departureHandler(
        new dcore::DepartureHandler(queue, graph, depart_uniform_rng_index));

    // we only need to register an event generation handler, if there are > 1 phases
    if (desArgs->generations > 1) {
        dcore::tGenerateEventHandlerSP generateEventHandler(
            new dcore::GenerateEventHandler(
                graph, arrival_rng, desArgs->generations, queue, desArgs->stop_time));
        lastArrivalEvent->attach(generateEventHandler);
    }

    // attach the handlers to the events
    anyEvent->attach(processedEventsHandler);
    arrivalEvent->attach(arrivalHandler);
    departureEvent->attach(departureHandler);
    postEvent->attach(unprocessedEventsHandler);

    // instantiate the event processor and set the events
    dcore::tEventProcessorSP processor(
        new dcore::EventProcessor(queue, anyEvent, arrivalEvent,
                                  departureEvent, postEvent, lastArrivalEvent,
                                  desArgs->stop_time));

    // process the events
    processor->process();

    return EXIT_SUCCESS;
}
