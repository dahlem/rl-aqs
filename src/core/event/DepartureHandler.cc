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

/** @file DepartureHandler.cc
 * Implementation of a basic departure handler.
 */
#include <iostream>
#include <cstdlib>
#include <vector>

#include <gsl/gsl_randist.h>

#include <boost/foreach.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include <boost/pending/integer_range.hpp>

#include "events.hh"
#include "DepartureEvent.hh"
#include "DepartureHandler.hh"
namespace dcore = des::core;

#include "Entry.hh"
#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "WEvonet.hh"
namespace dnet = des::network;

#include "CRN.hh"
namespace dsample = des::sampling;


dcore::DepartureHandler::DepartureHandler(dcommon::tQueueSP p_queue,
    dnet::tGraphSP p_graph, boost::uint32_t p_depart_uniform_idx)
    : m_queue(p_queue), m_graph(p_graph), m_depart_uniform_idx(p_depart_uniform_idx)
{
    m_depart_uniform_rng = dsample::CRN::getInstance().get(m_depart_uniform_idx - 1);
    vertex_busy_map = get(vertex_busy, *m_graph);
    vertex_number_in_queue_map = get(vertex_number_in_queue, *m_graph);
    vertex_index_map = get(boost::vertex_index, *m_graph);
    edge_weight_map = get(boost::edge_weight, *m_graph);
}


dcore::DepartureHandler::~DepartureHandler()
{}


void dcore::DepartureHandler::update(dcore::DepartureEvent *subject)
{
    dnet::OutEdgeIterator out_edge_it, out_edge_it_end;
    dcommon::Entry *entry;
    entry = subject->getEvent();

    dnet::Vertex vertex = boost::vertex(entry->destination, *m_graph);
    // if the server is busy then re-schedule
    // otherwise schedule the departure
    if (vertex_busy_map[vertex]) {
        vertex_number_in_queue_map[vertex]--;

        if (vertex_number_in_queue_map[vertex] == 0) {
            vertex_busy_map[vertex] = false;
        }

        dnet::Graph::degree_size_type degree =
            boost::out_degree(vertex, *m_graph);

        if (degree > 0) {
            tie(out_edge_it, out_edge_it_end) = boost::out_edges(vertex, *m_graph);

            std::vector <dnet::Edge> edges(degree);
            std::vector <float> edge_weights;
            std::vector <int> sorted_edge_weights(degree);
            boost::integer_range <int> range(0, degree);

            // copy the edges into a vector
            std::copy(out_edge_it, out_edge_it_end, edges.begin());

            // copy the index range into the service_rate_order vector
            std::copy(range.begin(), range.end(), sorted_edge_weights.begin());

            BOOST_FOREACH(dnet::Edge e, (boost::out_edges(vertex, *m_graph))) {
                edge_weights.push_back(edge_weight_map[e]);
            }

            // sort the sorted_edge_weights according to the edge_weights in ascending order
            std::sort(sorted_edge_weights.begin(), sorted_edge_weights.end(),
                      boost::indirect_cmp <float*, std::greater <float> >(&edge_weights[0]));

            double temp = 0.0;
            double u = gsl_rng_uniform(m_depart_uniform_rng.get());

            for (boost::uint32_t e = 0; e < degree; ++e) {
                dnet::Edge edge = edges[sorted_edge_weights[e]];
                temp += edge_weights[e];

                if (u < temp) {
                    // schedule an internal arrival event
                    boost::int32_t destination = vertex_index_map[target(edges[e], *m_graph)];
                    dcommon::Entry *new_entry = new dcommon::Entry(
                        entry->arrival,
                        destination,
                        dcore::INTERNAL_EVENT,
                        dcore::ARRIVAL_EVENT);

                    m_queue->push(new_entry);

                    break;
                }
            }
        }
    } else {
        std::cout << "departure not busy!!!" << std::endl;
    }
}
