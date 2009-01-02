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

/** @file WEvonet.cc
 * Implementation of the methods declared in WEvonet.hh.
 *
 * @author Dominik Dahlem
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <fstream>
#include <list>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

#include <boost/foreach.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/graph/property_iter_range.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include <boost/pending/integer_range.hpp>
#include <boost/random/linear_congruential.hpp>

#include <gsl/gsl_rng.h>

#include "epidemic_visit.hh"
#include "enduce_strength.hh"
#include "WEvonet.hh"


namespace des { namespace network {



tGraphSP WEvonet::createBBVGraph(boost::uint32_t p_size, boost::uint32_t max_edges, double fixed_edge_weight, double max_arrival_rate,
                                 tGslRngSP num_edges_rng, tGslRngSP uniform_rng, tGslRngSP vertex_arrival_rng)
{
    // create the graph
    tGraphSP g = tGraphSP(new Graph(0));

    // get references to the property maps
    EdgeWeightMap edge_weight_props_map
        = get(boost::edge_weight, *g);
    VertexArrivalRateMap vertex_arrival_props_map
        = get(vertex_arrival_rate, *g);
    VertexServiceRateMap vertex_service_props_map
        = get(vertex_service_rate, *g);
    VertexBusyMap vertex_busy_map
        = get(vertex_busy, *g);
    VertexTimeServiceEndsMap vertex_time_service_ends_map
        = get(vertex_time_service_ends, *g);
    VertexIndexMap vertex_index_props_map
        = get(boost::vertex_index, *g);
    VertexNumberInQueueMap vertex_number_in_queue_props_map
        = get(vertex_number_in_queue, *g);
    VertexAverageDelayInQueueMap vertex_average_delay_in_queue_props_map
        = get(vertex_average_delay_in_queue, *g);
    VertexNumEventsMap vertex_num_events_map
        = get(vertex_num_events, *g);
    VertexUtilisationMap vertex_utilisation_map
        = get(vertex_utilisation, *g);
    VertexBdtMap vertex_Bdt_map
        = get(vertex_Bdt, *g);
    VertexQdtMap vertex_Qdt_map
        = get(vertex_Qdt, *g);
    VertexLastEventTimeMap vertex_last_event_time_map
        = get(vertex_last_event_time, *g);
    VertexExpectedAverageNumberEventMap vertex_expected_average_number_event_map
        = get(vertex_expected_average_number_event, *g);

    // create a small graph upon which the evolution is excercised
    Vertex v1 = add_vertex(*g);
    vertex_arrival_props_map[v1] = (gsl_rng_uniform(vertex_arrival_rng.get()) * max_arrival_rate);
    vertex_service_props_map[v1] = vertex_arrival_props_map[v1];
    vertex_index_props_map[v1] = 0;
    vertex_busy_map[v1] = false;
    vertex_time_service_ends_map[v1] = 0.0;
    vertex_number_in_queue_props_map[v1] = 0;
    vertex_average_delay_in_queue_props_map[v1] = 0.0;
    vertex_num_events_map[v1] = 0;
    vertex_utilisation_map[v1] = 0.0;
    vertex_Bdt_map[v1] = 0.0;
    vertex_Qdt_map[v1] = 0.0;
    vertex_last_event_time_map[v1] = 0.0;
    vertex_expected_average_number_event_map[v1] = 0.0;

    Vertex v2 = add_vertex(*g);

    if (fixed_edge_weight > 0) {
        vertex_arrival_props_map[v2] = vertex_arrival_props_map[v1] * fixed_edge_weight;
    } else {
        vertex_arrival_props_map[v2] = vertex_arrival_props_map[v1] * 0.5;
    }
    vertex_service_props_map[v2] = vertex_arrival_props_map[v2];
    vertex_index_props_map[v2] = 1;
    vertex_busy_map[v2] = false;
    vertex_time_service_ends_map[v2] = 0.0;
    vertex_number_in_queue_props_map[v2] = 0;
    vertex_average_delay_in_queue_props_map[v2] = 0.0;
    vertex_num_events_map[v2] = 0;
    vertex_utilisation_map[v2] = 0.0;
    vertex_Bdt_map[v2] = 0.0;
    vertex_Qdt_map[v2] = 0.0;
    vertex_last_event_time_map[v2] = 0.0;
    vertex_expected_average_number_event_map[v2] = 0.0;

    Vertex v3 = add_vertex(*g);
    if (fixed_edge_weight > 0) {
        vertex_arrival_props_map[v3] = vertex_arrival_props_map[v1] * fixed_edge_weight;
    } else {
        vertex_arrival_props_map[v3] = vertex_arrival_props_map[v1] * 0.5;
    }
    vertex_service_props_map[v3] = vertex_arrival_props_map[v3];
    vertex_index_props_map[v3] = 2;
    vertex_busy_map[v3] = false;
    vertex_time_service_ends_map[v3] = 0.0;
    vertex_number_in_queue_props_map[v3] = 0;
    vertex_average_delay_in_queue_props_map[v3] = 0.0;
    vertex_num_events_map[v3] = 0;
    vertex_utilisation_map[v3] = 0.0;
    vertex_Bdt_map[v3] = 0.0;
    vertex_Qdt_map[v3] = 0.0;
    vertex_last_event_time_map[v3] = 0.0;
    vertex_expected_average_number_event_map[v3] = 0.0;

    Edge e1 = (add_edge(v1, v2, *g)).first;
    edge_weight_props_map[e1] = 0.5;
    Edge e2 = (add_edge(v1, v3, *g)).first;
    edge_weight_props_map[e2] = 0.5;

    advance(p_size - 3, g, num_edges_rng, uniform_rng, vertex_arrival_rng,
            fixed_edge_weight, max_arrival_rate, max_edges);

    return g;
}


void WEvonet::advance(boost::uint32_t p_steps, tGraphSP g,
                      tGslRngSP num_edges_rng, tGslRngSP uniform_rng, tGslRngSP vertex_arrival_rng,
                      double fixed_edge_weight, double max_arrival_rate, boost::uint32_t max_edges)
{
    VServiceIterator service_it, service_it_end;
    VertexArrivalRateMap vertex_arrival_props_map
        = get(vertex_arrival_rate, *g);
    VertexServiceRateMap vertex_service_props_map
        = get(vertex_service_rate, *g);
    VertexIndexMap vertex_index_props_map
        = get(boost::vertex_index, *g);
    VertexBusyMap vertex_busy_map
        = get(vertex_busy, *g);
    VertexTimeServiceEndsMap vertex_time_service_ends_map
        = get(vertex_time_service_ends, *g);
    VertexNumberInQueueMap vertex_number_in_queue_map
        = get(vertex_number_in_queue, *g);
    VertexAverageDelayInQueueMap vertex_average_delay_in_queue_map
        = get(vertex_average_delay_in_queue, *g);
    VertexNumEventsMap vertex_num_events_map
        = get(vertex_num_events, *g);
    VertexUtilisationMap vertex_utilisation_map
        = get(vertex_utilisation, *g);
    VertexBdtMap vertex_Bdt_map
        = get(vertex_Bdt, *g);
    VertexQdtMap vertex_Qdt_map
        = get(vertex_Qdt, *g);
    VertexLastEventTimeMap vertex_last_event_time_map
        = get(vertex_last_event_time, *g);
    VertexExpectedAverageNumberEventMap vertex_expected_average_number_event_map
        = get(vertex_expected_average_number_event, *g);

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
        std::vector <double> service_rates(vertices);

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
                  boost::indirect_cmp <double*, std::greater <double> >(&service_rates[0]));

        // create vertex
        Vertex v = boost::add_vertex(*g);
        vertex_arrival_props_map[v] = (gsl_rng_uniform(vertex_arrival_rng.get()) * max_arrival_rate);
        vertex_service_props_map[v] = vertex_arrival_props_map[v];
        vertex_index_props_map[v] = vertices;
        vertex_busy_map[v] = false;
        vertex_time_service_ends_map[v] = 0.0;
        vertex_number_in_queue_map[v] = 0;
        vertex_average_delay_in_queue_map[v] = 0.0;
        vertex_num_events_map[v] = 0;
        vertex_utilisation_map[v] = 0.0;
        vertex_Bdt_map[v] = 0.0;
        vertex_Qdt_map[v] = 0.0;
        vertex_last_event_time_map[v] = 0.0;
        vertex_expected_average_number_event_map[v] = 0.0;

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
                Vertex z = boost::vertex(service_rate_order[j], *g);

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
        assign_edge_weights(v, g);

        // re-calculate the node strengths using depth-first search originating from the new node
        balance_vertex_strength(v, g, fixed_edge_weight);
    }
}


void WEvonet::balance_vertex_strength(Vertex &v, tGraphSP g, double fixed_edge_weight)
{
    VertexArrivalRateMap vertex_arrival_props_map = get(vertex_arrival_rate, *g);
    VertexServiceRateMap vertex_service_props_map = get(vertex_service_rate, *g);
    EdgeWeightMap edge_weight_props_map = get(boost::edge_weight, *g);
    VertexIndexMap vertex_index_props_map = get(boost::vertex_index, *g);

    size_t vertices = boost::num_vertices(*g);

    // external property to keep the enduced differences in strengths
    std::vector <double> strength_diff_vec(vertices);
    std::vector <double> strength_diff_apply_vec(vertices);

    typedef boost::iterator_property_map <std::vector <double>::iterator,
        boost::property_map <Graph, boost::vertex_index_t>::type, double, double&> IterStrDiffMap;

    IterStrDiffMap strength_diff_map(strength_diff_vec.begin(), vertex_index_props_map);
    IterStrDiffMap strength_diff_apply_map(strength_diff_apply_vec.begin(), vertex_index_props_map);

    // create the visitor
    epidemic_vertex_arrival_visitor <VertexArrivalRateMap, EdgeWeightMap,
        VertexIndexMap, IterStrDiffMap>
        vis_enduced_strength(vertex_arrival_props_map, edge_weight_props_map,
                             vertex_index_props_map, strength_diff_map,
                             strength_diff_apply_map, fixed_edge_weight);

    // 1. determine the enduced differences in vertex strength
    boost::epidemic_visit(*g,
                          v,
                          visitor(vis_enduced_strength));

    // 2. apply the enduced differences in vertex strength
    apply_enduced_strength_visitor <VertexServiceRateMap, IterStrDiffMap>
        vis_apply_enduced_strength(vertex_service_props_map, strength_diff_map);

    std::vector<boost::default_color_type> color_vec(vertices);

    boost::breadth_first_visit(*g,
                        v,
                        visitor(vis_apply_enduced_strength).
                        color_map(make_iterator_property_map(
                                      color_vec.begin(),
                                      vertex_index_props_map,
                                      color_vec[0])));
}


void WEvonet::assign_edge_weights(Vertex &v, tGraphSP g)
{
    EdgeWeightMap edge_weight_props_map = get(boost::edge_weight, *g);

    Graph::degree_size_type degree = boost::out_degree(v, *g);
    OutEdgeIterator out_it, out_it_end;

    BOOST_FOREACH(Edge e, (boost::out_edges(v, *g))) {
        edge_weight_props_map[e] = 1.0 / degree;
    }
}

tGraphSP WEvonet::createERGraph(boost::uint32_t p_size, double fixed_edge_weight, double max_arrival_rate,
                                tGslRngSP p_vertex_arrival_rng, double p)
{
    typedef boost::erdos_renyi_iterator<boost::minstd_rand, Graph> ERGen;
    boost::minstd_rand gen;

    // Create graph with p_size nodes and edges with probability p
    tGraphSP g = tGraphSP(new Graph(ERGen(gen, p_size, p, false), ERGen(), p_size));

    VertexIndexMap vertexIndexMap = get(boost::vertex_index, *g);
    VertexArrivalRateMap vertex_arrival_props_map
        = get(vertex_arrival_rate, *g);
    VertexServiceRateMap vertex_service_props_map
        = get(vertex_service_rate, *g);

    // assign ids, arrival and service rates
    std::pair <VertexIterator, VertexIterator> p_v;
    int i = 0;
    for (p_v = boost::vertices(*g); p_v.first != p_v.second; ++p_v.first) {
        vertexIndexMap[*p_v.first] = i++;
        vertex_arrival_props_map[*p_v.first] =
            (gsl_rng_uniform(p_vertex_arrival_rng.get()) * max_arrival_rate);
        vertex_service_props_map[*p_v.first] = vertex_arrival_props_map[*p_v.first];
    }

    // remove cycles
    typedef std::vector <Edge> EdgesToBeRemoved;
    EdgesToBeRemoved edgesToBeRemoved;
    cycle_detector <EdgesToBeRemoved> vis(edgesToBeRemoved);
    boost::depth_first_search(*g, visitor(vis));

    EdgesToBeRemoved::iterator it(edgesToBeRemoved.begin()),
        it_end(edgesToBeRemoved.end());
    for (; it != it_end; ++it) {
        boost::remove_edge(*it, *g);
    }

    // balance the service rates
    typedef std::list <Vertex> BalanceOrder;
    BalanceOrder balance_order;
    boost::topological_sort(*g, std::front_inserter(balance_order));

    for (p_v = boost::vertices(*g); p_v.first != p_v.second; ++p_v.first) {
        assign_edge_weights(*p_v.first, g);
    }

    for (BalanceOrder::iterator it = balance_order.begin(); it != balance_order.end(); ++it) {
        if (boost::out_degree(*it, *g) > 0) {
            balance_vertex_strength(*it, g, fixed_edge_weight);
        }
    }

    return g;
}

}
}
