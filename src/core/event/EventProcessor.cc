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

/** @file EventProcessor.hh
 * Implementation of the event processor interface.
 */
#include <iostream>
#include <iomanip>
#include <string>

#include <boost/graph/adjacency_list.hpp>
#include <boost/iterator/filter_iterator.hpp>

#include "events.hh"
#include "EventGenerator.hh"
#include "EventProcessor.hh"
namespace dcore = des::core;

#include "Entry.hh"
#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "WEvonet.hh"
namespace dnet = des::network;

#include "Results.hh"
namespace dio = des::io;


dcore::EventProcessor::EventProcessor(dcommon::tQueueSP p_queue,
                                      dnet::tGraphSP p_graph,
                                      dcore::tArrivalEventSP p_arrivalEvent,
                                      dcore::tDepartureEventSP p_departureEvent,
                                      double p_stopTime)
    : m_queue(p_queue), m_graph(p_graph), m_arrivalEvent(p_arrivalEvent),
      m_departureEvent(p_departureEvent), m_stopTime(p_stopTime),
      m_generations(0)
{}


dcore::EventProcessor::~EventProcessor()
{}


void dcore::EventProcessor::setUnprocessedResults(
    dio::tResultsSP p_unprocessedEvents)
{
    m_unprocessedEvents = p_unprocessedEvents;
}


void dcore::EventProcessor::setProcessedResults(
    dio::tResultsSP p_processedEvents)
{
    m_processedEvents = p_processedEvents;
}


void dcore::EventProcessor::process()
{
    dcommon::Entry *entry = NULL;
    std::stringstream s;

    try {
        if (m_processedEvents != NULL) {
            s << dcommon::Entry::header();
            m_processedEvents->print(s);
        }

        while ((entry = m_queue->dequeue()) != NULL) {

            // if stop time has been reached break out and handle the event below
            if (entry->getArrival() > m_stopTime) {
                break;
            } else {
                if (m_processedEvents != NULL) {
                    s.str("");
                    // log the event
                    s << std::setprecision(14) << const_cast <const dcommon::Entry&> (*entry);
                    m_processedEvents->print(s);
                }
            }

            switch (entry->getType()) {
              case LAST_ARRIVAL_EVENT:
                  // generate new events
                  generate(entry);
              case ARRIVAL_EVENT:
                  m_arrivalEvent->arrival(entry);
                  break;
              case DEPARTURE_EVENT:
                  m_departureEvent->departure(entry);
                  break;
              default:
                  break;
            }

            delete entry;
        }

        postProcess(entry);
        std::cout << "Finished processing events." << std::endl;
    } catch (dcommon::QueueException &qe) {
        if (qe.errorCode() == dcommon::QueueException::QUEUE_EMPTY) {
            std::cout << "Finished processing events." << std::endl;
        } else {
            std::cout << "Exception: " << qe.what() << std::endl;
        }
    }
}


void dcore::EventProcessor::postProcess(dcommon::Entry *p_entry)
    throw (dcommon::QueueException)
{
    dcommon::Entry *entry = p_entry;

    // record the events left over
    if (m_unprocessedEvents != NULL) {
        if (entry != NULL) {
            std::stringstream s;
            s << dcommon::Entry::header();
            m_unprocessedEvents->print(s);

            do {
                s.str("");
                s << std::setprecision(14) << const_cast<const dcommon::Entry&> (*entry);
                m_unprocessedEvents->print(s);
                delete entry;
            } while ((entry = m_queue->dequeue()) != NULL);
        }
    }
}


void dcore::EventProcessor::setGenerations(dsample::tGslRngSP p_arrivalRng,
                                           int p_generations)
{
    int vertices = 0;

    m_arrivalRng = p_arrivalRng;
    m_generations = p_generations;
    m_interval = m_stopTime / m_generations;

    vertices = boost::num_vertices(*m_graph);
    m_currentGeneration = dcore::tIntSA(new int[vertices]);

    for (boost::int32_t i = 0; i < vertices; ++i) {
        m_currentGeneration[i] = 1;
    }
}


void dcore::EventProcessor::generate(dcommon::Entry *p_entry)
{
    if (m_generations > 1) {
        int dest = p_entry->getDestination();

        // increment the current generation for the destination vertex
        if (m_interval * (m_currentGeneration[dest] + 1) <= m_stopTime) {
            m_currentGeneration[dest]++;
            double stopTime = m_currentGeneration[dest] * m_interval;
            int count = 0;

            // filter the graph to find the vertex
            std::pair <dnet::VertexIterator, dnet::VertexIterator> v_iter;
            v_iter = boost::vertices(*m_graph);

            typedef boost::filter_iterator<dnet::exists_vertex_index<dnet::VertexIndexMap>, dnet::VertexIterator>
                FilterIter;

            dnet::VertexIndexMap vertex_index_props_map =
                get(boost::vertex_index, *m_graph);

            dnet::exists_vertex_index<dnet::VertexIndexMap>
                predicate(vertex_index_props_map, dest);

            FilterIter filter_iter_first(predicate, v_iter.first, v_iter.second);
            FilterIter filter_iter_last(predicate, v_iter.second, v_iter.second);

            for (; filter_iter_first != filter_iter_last; ++filter_iter_first) {
                if (count == 0) {
                    dnet::VertexArrivalRateMap vertex_arrival_props_map =
                        get(vertex_arrival_rate, *m_graph);
                    // generate a single event
                    double arrival_rate = vertex_arrival_props_map[*filter_iter_first];

                    // generate the events
                    dcore::EventGenerator::generate(m_queue, m_arrivalRng,
                                                    dest, arrival_rate,
                                                    p_entry->getArrival(), stopTime);
                } else {
                    std::cout << "Error: Expected a single vertex to be traced!" << std::endl;
                    break;
                }

                count++;
            }
        }
    }
}
