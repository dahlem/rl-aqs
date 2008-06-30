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
#include <fstream>
#include <numeric>
#include <vector>
#include <iostream>

#include <boost/foreach.hpp>

#include <boost/graph/adjacency_list.hpp>
using boost::num_vertices;
using boost::vertex;
using boost::vertex_index;
using boost::edge_weight;

#include <boost/graph/breadth_first_search.hpp>
using boost::default_bfs_visitor;
using boost::breadth_first_visit;

#include <boost/graph/graphviz.hpp>
using boost::write_graphviz;
using boost::dynamic_properties;

#include <boost/graph/property_iter_range.hpp>
using boost::graph_property_iter_range;

#include <boost/pending/indirect_cmp.hpp>
using boost::indirect_cmp;

#include <boost/pending/integer_range.hpp>
using boost::integer_range;

#include <gsl/gsl_rng.h>

#include "WEvonet.hh"
using des::network::WEvonet;
using des::network::Graph;
using des::network::Vertex;
using des::network::Edge;
using des::network::VServiceIterator;
using des::network::OutEdgeIterator;

#include "epidemic_visit.hh"


template <class VertexServiceMap, class EdgeWeightMap,
          class VertexIndexMap, class StrengthDiffMap>
class epidemic_vertex_service_visitor : public default_bfs_visitor
{
public:
    epidemic_vertex_service_visitor(VertexServiceMap vertex_service_map,
                                    EdgeWeightMap edge_weight_map,
                                    VertexIndexMap vertex_index_map,
                                    StrengthDiffMap vertex_strength_diff_map,
                                    StrengthDiffMap vertex_strength_diff_apply_map)
        :  m_vertex_service_map(vertex_service_map),
           m_edge_weight_map(edge_weight_map),
           m_vertex_index_map(vertex_index_map),
           m_vertex_strength_diff_map(vertex_strength_diff_map),
           m_vertex_strength_diff_apply_map(vertex_strength_diff_apply_map) {}

    template <typename Edge, typename Graph>
    void examine_edge(Edge e, const Graph & g) const
        {
            m_vertex_strength_diff_map[target(e, g)] +=
                (m_edge_weight_map[e] * m_vertex_strength_diff_apply_map[source(e, g)]);
        }

    template <typename Vertex, typename Graph>
    void examine_vertex(Vertex u, const Graph & g) const
        {
            if (m_vertex_strength_diff_map[u] == 0.0) {
                m_vertex_strength_diff_apply_map[u] = m_vertex_service_map[u];
            } else {
                m_vertex_strength_diff_apply_map[u] = m_vertex_strength_diff_map[u];
            }
        }

    template <typename Vertex, typename Graph>
    void finish_vertex(Vertex u, const Graph & g) const
        {
            m_vertex_strength_diff_apply_map[u] = 0.0;
        }

    VertexServiceMap m_vertex_service_map;
    EdgeWeightMap m_edge_weight_map;
    VertexIndexMap m_vertex_index_map;
    StrengthDiffMap m_vertex_strength_diff_map;
    StrengthDiffMap m_vertex_strength_diff_apply_map;
};



WEvonet::WEvonet(int p_size, tGslRngSP p_edge_rng, tGslRngSP p_uniform_rng)
{
    // create the graph
    g = tGraphSP(new Graph(0));
    num_edges_rng = p_edge_rng;
    uniform_rng = p_uniform_rng;

    // get references to the property maps
    VertexServiceRateMap vertex_service_props_map = get(vertex_service_rate, (*g.get()));
    VertexIndexMap vertex_index_props_map = get(vertex_index, (*g.get()));
    EdgeWeightMap edge_weight_props_map = get(edge_weight, (*g.get()));

    // create a small graph upon which the evolution is excercised
    Vertex v1 = add_vertex((*g.get()));
    vertex_service_props_map[v1] = 12.5;
    vertex_index_props_map[v1] = 0;
    Vertex v2 = add_vertex((*g.get()));
    vertex_service_props_map[v2] = 6.25;
    vertex_index_props_map[v2] = 1;
    Vertex v3 = add_vertex((*g.get()));
    vertex_service_props_map[v3] = 6.25;
    vertex_index_props_map[v3] = 2;

    Edge e1 = (add_edge(v1, v2, (*g.get()))).first;
    edge_weight_props_map[e1] = 0.5;
    Edge e2 = (add_edge(v1, v3, (*g.get()))).first;
    edge_weight_props_map[e2] = 0.5;

    advance(p_size - 3);
}


WEvonet::~WEvonet()
{}


void WEvonet::advance(int p_steps)
{
    VServiceIterator service_it, service_it_end;
    VertexServiceRateMap vertex_service_props_map = get(vertex_service_rate, (*g.get()));
    VertexIndexMap vertex_index_props_map = get(vertex_index, (*g.get()));

    double accum_service_rate;

    // at each step do:
    // 1. create vertex
    // 2. attach vertex via m links to existing ones
    // 3. assign weights to the edges
    // 4. evaluate the vertex strengths
    for (int i = 0; i < p_steps; ++i) {
        // calculate the accumulated service rate
        tie(service_it, service_it_end) =
            get_property_iter_range((*g.get()), vertex_service_rate);
        accum_service_rate = std::accumulate(service_it, service_it_end, 0.0);

        // a vector to hold the discover time property for each vertex
        std::vector <float> service_rates(num_vertices((*g.get())));

        // Use std::sort to order the vertices by their discover time
        std::vector <graph_traits<Graph>::vertices_size_type>
            service_rate_order(num_vertices((*g.get())));
        integer_range <int> range(0, num_vertices((*g.get())));

        // copy the index range into the service_rate_order vector
        std::copy(range.begin(), range.end(), service_rate_order.begin());

        // copy the service rates into a vector
        std::copy(service_it, service_it_end, service_rates.begin());

        // sort the service_rate_order according to the service_rates in ascending order
        std::sort(service_rate_order.begin(), service_rate_order.end(),
                  indirect_cmp <float*, std::greater <float> >(&service_rates[0]));

        // create vertex
        std::cout << "Create new vertex... " << std::endl;

        Vertex v = add_vertex((*g.get()));
        vertex_service_props_map[v] = 11.1;
        vertex_index_props_map[v] = num_vertices((*g.get())) - 1;

        // select vertices to connect to
        unsigned int edges = gsl_rng_uniform_int(num_edges_rng.get(), 5) + 1;

        for (int e = 0; e < edges; ++e) {
            double temp = 0.0;
            double u = gsl_rng_uniform(uniform_rng.get());
            for (int j = 0; j < num_vertices((*g.get())) - 1; ++j) {
                temp += vertex_service_props_map[vertex(service_rate_order[j], (*g.get()))];

                if (u < temp/accum_service_rate) {
                    // check if link already exists between new vertex and selected one
                    if (!edge(v, vertex(service_rate_order[j], (*g.get())), (*g.get())).second) {
                        add_edge(v, vertex(service_rate_order[j], (*g.get())), (*g.get()));
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


void WEvonet::balance_vertex_strength(Vertex &v)
{
    VertexServiceRateMap vertex_service_props_map = get(vertex_service_rate, (*g.get()));
    EdgeWeightMap edge_weight_props_map = get(edge_weight, (*g.get()));
    VertexIndexMap vertex_index_props_map = get(vertex_index, (*g.get()));

    // external property to keep the enduced differences in strengths
    std::vector<float> strength_diff_vec(num_vertices((*g.get())));
    std::vector<float> strength_diff_apply_vec(num_vertices((*g.get())));

    typedef boost::iterator_property_map <std::vector <float>::iterator,
        property_map <Graph, vertex_index_t>::type, float, float&> IterStrDiffMap;

    IterStrDiffMap strength_diff_map(strength_diff_vec.begin(), get(vertex_index, (*g.get())));
    IterStrDiffMap strength_diff_apply_map(strength_diff_apply_vec.begin(), get(vertex_index, (*g.get())));

    // create the visitor
    epidemic_vertex_service_visitor <VertexServiceRateMap, EdgeWeightMap,
        VertexIndexMap, IterStrDiffMap>
        vis(vertex_service_props_map, edge_weight_props_map, vertex_index_props_map,
            strength_diff_map, strength_diff_apply_map);

    // 1. determine the enduced differences in vertex strength
    epidemic_visit((*g.get()),
                   v,
                   visitor(vis));

    typedef graph_traits <Graph>::vertex_iterator vertex_iter_t;
    std::pair <vertex_iter_t, vertex_iter_t> p;
    for (p = vertices((*g.get())); p.first != p.second; ++p.first) {
        std::cout << strength_diff_map[*p.first] << std::endl;
    }

    // 2. apply the enduced differences in vertex strength
}


void WEvonet::print(const std::string& filename)
{
    VertexServiceRateMap vertex_service_props_map = get(vertex_service_rate, (*g.get()));
    VertexIndexMap vertex_index_props_map = get(vertex_index, (*g.get()));
    EdgeWeightMap edge_weight_props_map = get(edge_weight, (*g.get()));

    std::ofstream out(filename.c_str(), std::ios::out);

    if (out.is_open()) {
        dynamic_properties dp;
        dp.property("node_id", vertex_index_props_map);
        dp.property("label", edge_weight_props_map);
        dp.property("label", vertex_service_props_map);

        write_graphviz(out, (*g.get()), dp);
        out.close();
    }
}


void WEvonet::assign_edge_weights(Vertex &v)
{
    EdgeWeightMap edge_weight_props_map = get(edge_weight, (*g.get()));

    Graph::degree_size_type degree = out_degree(v, (*g.get()));
    OutEdgeIterator out_it, out_it_end;

    BOOST_FOREACH(Edge e, (out_edges(v, (*g.get())))) {
        edge_weight_props_map[e] = 1.0 / degree;
    }
}
