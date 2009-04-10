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

/** @file UnprocessedEventHandler.cc
 * Implementation of a basic unprocessedEvent handler.
 */
#include <iomanip>
#include <string>
#include <utility>

#include <boost/graph/adjacency_list.hpp>
#include <boost/iterator/filter_iterator.hpp>

#include "Entry.hh"
namespace dcommon = des::common;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "GenerateEventHandler.hh"
#include "EventGenerator.hh"



namespace des
{
namespace core
{


dcore::GenerateEventHandler::GenerateEventHandler(
    dnet::Graph &p_graph,
    Int32SA p_arrivalRngs,
    int p_generations,
    dcommon::Queue &p_queue,
    double p_stopTime)
    : m_graph(p_graph), m_arrivalRngs(p_arrivalRngs), m_generations(p_generations),
      m_queue(p_queue), m_stopTime(p_stopTime)
{
    int vertices = 0;

    m_interval = m_stopTime / m_generations;

    vertices = boost::num_vertices(m_graph);
    m_currentGeneration = dcore::tIntSA(new int[vertices]);

    for (boost::int32_t i = 0; i < vertices; ++i) {
        m_currentGeneration[i] = 1;
    }

#ifndef NDEBUG_SAMPLING
    std::cout << "Arrival seeds: ";
    for (int i = 0; i < vertices; ++i) {
        std::cout << m_arrivalRngs[i] << ", ";
    }
    std::cout << std::endl << std::cout.flush();
#endif /* NDEBUG_SAMPLING */
}


dcore::GenerateEventHandler::~GenerateEventHandler()
{}


void dcore::GenerateEventHandler::update(dcore::LastArrivalEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();

    if (m_generations > 1) {
        int dest = entry->getDestination();

        // increment the current generation for the destination vertex
        if (m_interval * (m_currentGeneration[dest] + 1) <= m_stopTime) {
            m_currentGeneration[dest]++;

            double stopTime = m_currentGeneration[dest] * m_interval;
            int count = 0;

            // filter the graph to find the vertex
            std::pair <dnet::VertexIterator, dnet::VertexIterator> v_iter;
            v_iter = boost::vertices(m_graph);

            typedef boost::filter_iterator<dnet::exists_vertex_index<dnet::VertexIndexMap>, dnet::VertexIterator>
                FilterIter;

            dnet::VertexIndexMap vertex_index_props_map =
                get(boost::vertex_index, m_graph);

            dnet::exists_vertex_index<dnet::VertexIndexMap>
                predicate(vertex_index_props_map, dest);

            FilterIter filter_iter_first(predicate, v_iter.first, v_iter.second);
            FilterIter filter_iter_last(predicate, v_iter.second, v_iter.second);
            dsample::tGslRngSP arrival_rng;

            for (; filter_iter_first != filter_iter_last; ++filter_iter_first) {
                if (count == 0) {
                    dnet::VertexNextEventTimeMap vertex_next_event_time_map =
                        get(vertex_next_event_time, m_graph);
                    dnet::VertexArrivalRateMap vertex_arrival_props_map =
                        get(vertex_arrival_rate, m_graph);
                    // generate a single event
                    double arrival_rate = vertex_arrival_props_map[*filter_iter_first];
                    double startTime = vertex_next_event_time_map[*filter_iter_first];

                    // generate the events
                    arrival_rng = dsample::CRN::getInstance().get(
                        m_arrivalRngs[dest]);
                    dcore::EventGenerator::generate(m_graph, m_queue, arrival_rng,
                                                    dest, arrival_rate,
                                                    startTime, stopTime);
                } else {
                    std::cout << "Error: Expected a vertex!" << std::endl;
                    break;
                }

                count++;
            }


//             double stopTime = m_currentGeneration[dest] * m_interval;
//             dnet::Vertex vertex = boost::vertex(entry->getDestination(), *m_graph);
//             dnet::VertexArrivalRateMap vertex_arrival_props_map =
//                 get(vertex_arrival_rate, *m_graph);
//             dnet::VertexNextEventTimeMap vertex_next_event_time_map =
//                 get(vertex_next_event_time, *m_graph);
//             // generate a single event
//             double arrival_rate = vertex_arrival_props_map[vertex];
//             double startTime = vertex_next_event_time_map[vertex];

//             // generate the events
//             dsample::tGslRngSP arrival_rng = dsample::CRN::getInstance().get(
//                 m_arrivalRngs[dest]);
//             dcore::EventGenerator::generate(m_graph, m_queue, arrival_rng,
//                                             dest, arrival_rate,
//                                             startTime, stopTime);
        }
    }
}



}
}
