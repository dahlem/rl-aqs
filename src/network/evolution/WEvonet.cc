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

/** @file WEvonet.cc
 * Implementation of the methods declared in WEvonet.hh.
 *
 * @author Dominik Dahlem
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <fstream>
#include <numeric>
#include <vector>

#ifdef NDEBUG
# include <iostream>
#endif

#include <fstream>

#include <boost/foreach.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/property_iter_range.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include <boost/pending/integer_range.hpp>

#include <gsl/gsl_rng.h>

#include "epidemic_visit.hh"
#include "enduce_strength.hh"
#include "WEvonet.hh"
namespace dnet = des::network;


const std::string dnet::WEvonet::EDGE_WEIGHT            = "weight";
const std::string dnet::WEvonet::VERTEX_ID              = "id";
const std::string dnet::WEvonet::SERVICE_RATE           = "service_rate";
const std::string dnet::WEvonet::ARRIVAL_RATE           = "arrival_rate";
const std::string dnet::WEvonet::BUSY                   = "busy";
const std::string dnet::WEvonet::TIME_SERVICE_ENDS      = "time_service_ends";
const std::string dnet::WEvonet::NUMBER_IN_QUEUE        = "number_in_queue";
const std::string dnet::WEvonet::AVERAGE_DELAY_IN_QUEUE = "average_delay_in_queue";
const std::string dnet::WEvonet::NUM_EVENTS             = "num_events";



dnet::WEvonet::WEvonet(boost::uint32_t p_size, boost::uint32_t p_max_edges,
                 tGslRngSP p_edge_rng, tGslRngSP p_uniform_rng, tGslRngSP p_vertex_arrival_rng)
{
    // create the graph
    g = tGraphSP(new dnet::Graph(0));
    num_edges_rng = p_edge_rng;
    uniform_rng = p_uniform_rng;
    vertex_arrival_rng = p_vertex_arrival_rng;

    max_edges = p_max_edges;

    // get references to the property maps
    dnet::EdgeWeightMap edge_weight_props_map
        = get(boost::edge_weight, *g);
    dnet::VertexArrivalRateMap vertex_arrival_props_map
        = get(vertex_arrival_rate, *g);
    dnet::VertexServiceRateMap vertex_service_props_map
        = get(vertex_service_rate, *g);
    dnet::VertexBusyMap vertex_busy_map
        = get(vertex_busy, *g);
    dnet::VertexTimeServiceEndsMap vertex_time_service_ends_map
        = get(vertex_time_service_ends, *g);
    dnet::VertexIndexMap vertex_index_props_map
        = get(boost::vertex_index, *g);
    dnet::VertexNumberInQueueMap vertex_number_in_queue_props_map
        = get(vertex_number_in_queue, *g);
    dnet::VertexAverageDelayInQueueMap vertex_average_delay_in_queue_props_map
        = get(vertex_average_delay_in_queue, *g);
    dnet::VertexNumEventsMap vertex_num_events_map
        = get(vertex_num_events, *g);

    // create a small graph upon which the evolution is excercised
    dnet::Vertex v1 = add_vertex(*g);
    vertex_arrival_props_map[v1] = (gsl_rng_uniform(vertex_arrival_rng.get()) * 10);
    vertex_service_props_map[v1] = vertex_arrival_props_map[v1];
    vertex_index_props_map[v1] = 0;
    vertex_busy_map[v1] = false;
    vertex_time_service_ends_map[v1] = 0.0;
    vertex_number_in_queue_props_map[v1] = 0;
    vertex_average_delay_in_queue_props_map[v1] = 0.0;
    vertex_num_events_map[v1] = 0;

    dnet::Vertex v2 = add_vertex(*g);
    vertex_arrival_props_map[v2] = vertex_arrival_props_map[v1] * 0.5;
    vertex_service_props_map[v2] = vertex_arrival_props_map[v2];
    vertex_index_props_map[v2] = 1;
    vertex_busy_map[v2] = false;
    vertex_time_service_ends_map[v2] = 0.0;
    vertex_number_in_queue_props_map[v2] = 0;
    vertex_average_delay_in_queue_props_map[v2] = 0.0;
    vertex_num_events_map[v2] = 0;

    dnet::Vertex v3 = add_vertex(*g);
    vertex_arrival_props_map[v3] = vertex_arrival_props_map[v1] * 0.5;
    vertex_service_props_map[v3] = vertex_arrival_props_map[v3];
    vertex_index_props_map[v3] = 2;
    vertex_busy_map[v3] = false;
    vertex_time_service_ends_map[v3] = 0.0;
    vertex_number_in_queue_props_map[v3] = 0;
    vertex_average_delay_in_queue_props_map[v3] = 0.0;
    vertex_num_events_map[v3] = 0;

    dnet::Edge e1 = (add_edge(v1, v2, *g)).first;
    edge_weight_props_map[e1] = 0.5;
    dnet::Edge e2 = (add_edge(v1, v3, *g)).first;
    edge_weight_props_map[e2] = 0.5;

    advance(p_size - 3);
}


dnet::WEvonet::~WEvonet()
{}


void dnet::WEvonet::advance(boost::uint32_t p_steps)
{
    dnet::VServiceIterator service_it, service_it_end;
    dnet::VertexArrivalRateMap vertex_arrival_props_map
        = get(vertex_arrival_rate, *g);
    dnet::VertexServiceRateMap vertex_service_props_map
        = get(vertex_service_rate, *g);
    dnet::VertexIndexMap vertex_index_props_map
        = get(boost::vertex_index, *g);
    dnet::VertexBusyMap vertex_busy_map
        = get(vertex_busy, *g);
    dnet::VertexTimeServiceEndsMap vertex_time_service_ends_map
        = get(vertex_time_service_ends, *g);
    dnet::VertexNumberInQueueMap vertex_number_in_queue_map
        = get(vertex_number_in_queue, *g);
    dnet::VertexAverageDelayInQueueMap vertex_average_delay_in_queue_map
        = get(vertex_average_delay_in_queue, *g);
    dnet::VertexNumEventsMap vertex_num_events_map
        = get(vertex_num_events, *g);

    double accum_service_rate;
    size_t vertices;

    // at each step do:
    // 1. create vertex
    // 2. attach vertex via m links to existing ones
    // 3. assign weights to the edges
    // 4. evaluate the vertex strengths
    for (boost::uint32_t i = 0; i < p_steps; ++i) {
        vertices = boost::num_vertices(*g);

        // calculate the accumulated service rate
        tie(service_it, service_it_end) =
            get_property_iter_range(*g, vertex_service_rate);
        accum_service_rate = std::accumulate(service_it, service_it_end, 0.0);

        // a vector to hold the discover time property for each vertex
        std::vector <float> service_rates(vertices);

        // Use std::sort to order the vertices by their service rate
        std::vector <boost::graph_traits <Graph>::vertices_size_type>
            service_rate_order(vertices);
        boost::integer_range <int> range(0, vertices);

        // copy the index range into the service_rate_order vector
        std::copy(range.begin(), range.end(), service_rate_order.begin());

        // copy the service rates into a vector
        std::copy(service_it, service_it_end, service_rates.begin());

        // sort the service_rate_order according to the service_rates in ascending order
        std::sort(service_rate_order.begin(), service_rate_order.end(),
                  boost::indirect_cmp <float*, std::greater <float> >(&service_rates[0]));

        // create vertex
        dnet::Vertex v = boost::add_vertex(*g);
        vertex_arrival_props_map[v] = (gsl_rng_uniform(vertex_arrival_rng.get()) * 10);
        vertex_service_props_map[v] = vertex_arrival_props_map[v];
        vertex_index_props_map[v] = vertices;
        vertex_busy_map[v] = false;
        vertex_time_service_ends_map[v] = 0.0;
        vertex_number_in_queue_map[v] = 0;
        vertex_average_delay_in_queue_map[v] = 0.0;
        vertex_num_events_map[v] = 0;

        // select vertices to connect to
        boost::uint32_t edges = 0;

        // if MAX_EDGES is selected than the edges are deterministic
        // otherwise they are stochastic bounded on the maximum number of available
        // vertices
        if (max_edges == WEvonet::MAX_EDGES) {
            edges = vertices;
        } else if (max_edges > vertices) {
            edges = gsl_rng_uniform_int(num_edges_rng.get(), vertices) + 1;
        } else {
            edges = gsl_rng_uniform_int(num_edges_rng.get(), max_edges) + 1;
        }

        for (boost::uint32_t e = 0; e < edges; ++e) {
            double temp = 0.0;
            double u = gsl_rng_uniform(uniform_rng.get());
            for (boost::uint32_t j = 0; j < vertices; ++j) {
                dnet::Vertex z = boost::vertex(service_rate_order[j], *g);

                temp += vertex_service_props_map[z];

                if (u < temp/accum_service_rate) {
                    // check if link already exists between new vertex and selected one
                    if (!edge(v, z, *g).second) {
                        add_edge(v, z, *g);
                        break;
                    }
                }
            }
        }

        // calculate and assign the edge weights of the newly created vertex and its out edges
        assign_edge_weights(v);

        // re-calculate the node strengths using depth-first search originating from the new node
        balance_vertex_strength(v);
    }
}


void dnet::WEvonet::balance_vertex_strength(Vertex &v)
{
    dnet::VertexArrivalRateMap vertex_arrival_props_map = get(vertex_arrival_rate, *g);
    dnet::VertexServiceRateMap vertex_service_props_map = get(vertex_service_rate, *g);
    dnet::EdgeWeightMap edge_weight_props_map = get(boost::edge_weight, *g);
    dnet::VertexIndexMap vertex_index_props_map = get(boost::vertex_index, *g);

    size_t vertices = boost::num_vertices(*g);

    // external property to keep the enduced differences in strengths
    std::vector <float> strength_diff_vec(vertices);
    std::vector <float> strength_diff_apply_vec(vertices);

    typedef boost::iterator_property_map <std::vector <float>::iterator,
        boost::property_map <dnet::Graph, boost::vertex_index_t>::type, float, float&> IterStrDiffMap;

    IterStrDiffMap strength_diff_map(strength_diff_vec.begin(), vertex_index_props_map);
    IterStrDiffMap strength_diff_apply_map(strength_diff_apply_vec.begin(), vertex_index_props_map);

    // create the visitor
    dnet::epidemic_vertex_arrival_visitor <dnet::VertexArrivalRateMap, dnet::EdgeWeightMap,
        dnet::VertexIndexMap, IterStrDiffMap>
        vis_enduced_strength(vertex_arrival_props_map, edge_weight_props_map,
                             vertex_index_props_map, strength_diff_map,
                             strength_diff_apply_map);

    // 1. determine the enduced differences in vertex strength
    boost::epidemic_visit(*g,
                          v,
                          visitor(vis_enduced_strength));

    // 2. apply the enduced differences in vertex strength
    dnet::apply_enduced_strength_visitor <VertexServiceRateMap, IterStrDiffMap>
        vis_apply_enduced_strength(vertex_service_props_map, strength_diff_map);

    std::vector<boost::default_color_type> color_vec(vertices);

    boost::breadth_first_visit(*g,
                        v,
                        visitor(vis_apply_enduced_strength).
                        color_map(make_iterator_property_map(
                                      color_vec.begin(),
                                      vertex_index_props_map,
                                      color_vec[0])));

#ifdef NDEBUG
    std::pair <dnet::VertexIterator, dnet::VertexIterator> p;

    for (p = boost::vertices(*g); p.first != p.second; ++p.first) {
        std::cout << vertex_arrival_props_map[*p.first] << std::endl;
    }
#endif /* NDEBUG */
}


void dnet::WEvonet::assign_edge_weights(Vertex &v)
{
    dnet::EdgeWeightMap edge_weight_props_map = get(boost::edge_weight, *g);

    dnet::Graph::degree_size_type degree = boost::out_degree(v, *g);
    dnet::OutEdgeIterator out_it, out_it_end;

    BOOST_FOREACH(dnet::Edge e, (boost::out_edges(v, *g))) {
        edge_weight_props_map[e] = 1.0 / degree;
    }
}


void dnet::WEvonet::print(const std::string& filename, const GraphTypes graphType)
{
    switch (graphType) {
      case GRAPHVIZ:
          print_dot(filename);
          break;
      case GRAPHML:
          print_graphml(filename);
          break;
      default:
          print_dot(filename);
          break;
    }
}


void dnet::WEvonet::print_dot(const std::string& filename)
{
    dnet::EdgeWeightMap edge_weight_props_map
        = get(boost::edge_weight, *g);
    dnet::VertexServiceRateMap vertex_service_props_map =
        get(vertex_service_rate, *g);
    dnet::VertexArrivalRateMap vertex_arrival_props_map =
        get(vertex_arrival_rate, *g);
    dnet::VertexIndexMap vertex_index_props_map
        = get(boost::vertex_index, *g);
    dnet::VertexBusyMap vertex_busy_map
        = get(vertex_busy, *g);
    dnet::VertexTimeServiceEndsMap vertex_time_service_ends_map =
        get(vertex_time_service_ends, *g);
    dnet::VertexNumberInQueueMap vertex_number_in_queue_map =
        get(vertex_number_in_queue, *g);
    dnet::VertexAverageDelayInQueueMap vertex_average_delay_in_queue_map =
        get(vertex_average_delay_in_queue, *g);
    dnet::VertexNumEventsMap vertex_num_events_map
        = get(vertex_num_events, *g);

    std::ofstream out(filename.c_str(), std::ios::out);

    if (out.is_open()) {
        boost::dynamic_properties dp;
        dp.property(dnet::WEvonet::EDGE_WEIGHT, edge_weight_props_map);
        dp.property(dnet::WEvonet::VERTEX_ID, vertex_index_props_map);
        dp.property(dnet::WEvonet::SERVICE_RATE, vertex_service_props_map);
        dp.property(dnet::WEvonet::ARRIVAL_RATE, vertex_arrival_props_map);
        dp.property(dnet::WEvonet::BUSY, vertex_busy_map);
        dp.property(dnet::WEvonet::TIME_SERVICE_ENDS, vertex_time_service_ends_map);
        dp.property(dnet::WEvonet::NUMBER_IN_QUEUE, vertex_number_in_queue_map);
        dp.property(dnet::WEvonet::AVERAGE_DELAY_IN_QUEUE, vertex_average_delay_in_queue_map);
        dp.property(dnet::WEvonet::NUM_EVENTS, vertex_num_events_map);

        boost::write_graphviz(out, *g, dp);
        out.close();
    }
}


void dnet::WEvonet::print_graphml(const std::string& filename)
{
    dnet::EdgeWeightMap edge_weight_props_map
        = get(boost::edge_weight, *g);
    dnet::VertexServiceRateMap vertex_service_props_map =
        get(vertex_service_rate, *g);
    dnet::VertexArrivalRateMap vertex_arrival_props_map =
        get(vertex_arrival_rate, *g);
    dnet::VertexIndexMap vertex_index_props_map
        = get(boost::vertex_index, *g);
    dnet::VertexBusyMap vertex_busy_map
        = get(vertex_busy, *g);
    dnet::VertexTimeServiceEndsMap vertex_time_service_ends_map =
        get(vertex_time_service_ends, *g);
    dnet::VertexNumberInQueueMap vertex_number_in_queue_map =
        get(vertex_number_in_queue, *g);
    dnet::VertexAverageDelayInQueueMap vertex_average_delay_in_queue_map =
        get(vertex_average_delay_in_queue, *g);
    dnet::VertexNumEventsMap vertex_num_events_map
        = get(vertex_num_events, *g);

    std::ofstream out(filename.c_str(), std::ios::out);

    if (out.is_open()) {
        boost::dynamic_properties dp;
        dp.property(dnet::WEvonet::EDGE_WEIGHT, edge_weight_props_map);
        dp.property(dnet::WEvonet::VERTEX_ID, vertex_index_props_map);
        dp.property(dnet::WEvonet::SERVICE_RATE, vertex_service_props_map);
        dp.property(dnet::WEvonet::ARRIVAL_RATE, vertex_arrival_props_map);
        dp.property(dnet::WEvonet::BUSY, vertex_busy_map);
        dp.property(dnet::WEvonet::TIME_SERVICE_ENDS, vertex_time_service_ends_map);
        dp.property(dnet::WEvonet::NUMBER_IN_QUEUE, vertex_number_in_queue_map);
        dp.property(dnet::WEvonet::AVERAGE_DELAY_IN_QUEUE, vertex_average_delay_in_queue_map);
        dp.property(dnet::WEvonet::NUM_EVENTS, vertex_num_events_map);

        boost::write_graphml(out, *g, dp, true);

        out.close();
    }
}


void dnet::WEvonet::read(tGraphSP p_graph, const std::string& p_filename,
                                const GraphTypes p_graphType)
    throw (dnet::GraphException)
{
    switch (p_graphType) {
      case GRAPHVIZ:
          read_dot(p_graph, p_filename);
          break;
      case GRAPHML:
          read_graphml(p_graph, p_filename);
          break;
      default:
          read_graphml(p_graph, p_filename);
          break;
    }
}


void dnet::WEvonet::read_graphml(tGraphSP p_graph, const std::string& p_filename)
    throw (dnet::GraphException)
{
    std::ifstream in(p_filename.c_str(), std::ifstream::in);

    if (in.is_open()) {
        boost::dynamic_properties dp;
        dp.property(dnet::WEvonet::EDGE_WEIGHT, get(boost::edge_weight, *p_graph));
        dp.property(dnet::WEvonet::VERTEX_ID, get(boost::vertex_index, *p_graph));
        dp.property(dnet::WEvonet::SERVICE_RATE, get(vertex_service_rate, *p_graph));
        dp.property(dnet::WEvonet::ARRIVAL_RATE, get(vertex_arrival_rate, *p_graph));
        dp.property(dnet::WEvonet::BUSY, get(vertex_busy, *p_graph));
        dp.property(dnet::WEvonet::TIME_SERVICE_ENDS, get(vertex_time_service_ends, *p_graph));
        dp.property(dnet::WEvonet::NUMBER_IN_QUEUE, get(vertex_number_in_queue, *p_graph));
        dp.property(dnet::WEvonet::AVERAGE_DELAY_IN_QUEUE, get(vertex_average_delay_in_queue, *p_graph));
        dp.property(dnet::WEvonet::NUM_EVENTS, get(vertex_num_events, *p_graph));

        try {
            boost::read_graphml(in, (*p_graph.get()), dp);
        } catch (...) {
            throw dnet::GraphException(dnet::GraphException::GRAPH_READ_ERROR);
        }

        in.close();
    } else {
        throw dnet::GraphException(dnet::GraphException::GRAPH_INPUT_ERROR);
    }
}


void dnet::WEvonet::read_dot(tGraphSP p_graph, const std::string& p_filename)
    throw (dnet::GraphException)
{
    std::ifstream in(p_filename.c_str(), std::ifstream::in);

    if (in.is_open()) {
        boost::dynamic_properties dp;
        dp.property(dnet::WEvonet::EDGE_WEIGHT, get(boost::edge_weight, *p_graph));
        dp.property(dnet::WEvonet::VERTEX_ID, get(boost::vertex_index, *p_graph));
        dp.property(dnet::WEvonet::SERVICE_RATE, get(vertex_service_rate, *p_graph));
        dp.property(dnet::WEvonet::ARRIVAL_RATE, get(vertex_arrival_rate, *p_graph));
        dp.property(dnet::WEvonet::BUSY, get(vertex_busy, *p_graph));
        dp.property(dnet::WEvonet::TIME_SERVICE_ENDS, get(vertex_time_service_ends, *p_graph));
        dp.property(dnet::WEvonet::NUMBER_IN_QUEUE, get(vertex_number_in_queue, *p_graph));
        dp.property(dnet::WEvonet::AVERAGE_DELAY_IN_QUEUE, get(vertex_average_delay_in_queue, *p_graph));
        dp.property(dnet::WEvonet::NUM_EVENTS, get(vertex_num_events, *p_graph));

        try {
            boost::read_graphviz(in, (*p_graph.get()), dp);
        } catch (...) {
            throw dnet::GraphException(dnet::GraphException::GRAPH_READ_ERROR);
        }

        in.close();
    } else {
        throw dnet::GraphException(dnet::GraphException::GRAPH_INPUT_ERROR);
    }
}
