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
#include <iostream>
#include <vector>
#include <numeric>
#include <functional>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/random.hpp>
#include <boost/graph/property_iter_range.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/pending/integer_range.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include <boost/pending/fenced_priority_queue.hpp>

using namespace boost;


namespace boost
{
    enum vertex_service_rate_t
    { vertex_service_rate = 111 };

    BOOST_INSTALL_PROPERTY(vertex, service_rate);
}

typedef boost::property<vertex_service_rate_t, float> VertexServiceRateProperty;


int main(int argc, char *argv[])
{
    typedef adjacency_list<setS,    // no parallel edges; most expensive space complexity
        listS,                      // add/remove verteces quickly; constant time complexity
        bidirectionalS,
        property<vertex_index_t, int, VertexServiceRateProperty> > Graph;

    typedef graph_traits<Graph>::vertex_descriptor Vertex;
    typedef graph_traits<Graph>::edge_descriptor Edge;
    typedef property_map<Graph, vertex_service_rate_t>::type VertexServiceRateMap;
    typedef property_map<Graph, vertex_index_t>::type VertexIndexMap;
    typedef graph_traits<Graph>::vertices_size_type Size;
    typedef Size* Iiter;

    // declare a graph object
    Graph g(0);
    VertexServiceRateMap vertex_service_props_map = get(vertex_service_rate, g);
    VertexIndexMap vertex_index_props_map = get(vertex_index, g);

    Vertex v1 = add_vertex(g);
    vertex_service_props_map[v1] = 12.5;
    vertex_index_props_map[v1] = 0;
    Vertex v2 = add_vertex(g);
    vertex_service_props_map[v2] = 13.5;
    vertex_index_props_map[v2] = 1;
    Vertex v3 = add_vertex(g);
    vertex_service_props_map[v3] = 14.5;
    vertex_index_props_map[v3] = 2;
    Vertex v4 = add_vertex(g);
    vertex_service_props_map[v4] = 15.5;
    vertex_index_props_map[v4] = 3;
    Vertex v5 = add_vertex(g);
    vertex_service_props_map[v5] = 16.5;
    vertex_index_props_map[v5] = 4;
    Vertex v6 = add_vertex(g);
    vertex_service_props_map[v6] = 3.5;
    vertex_index_props_map[v6] = 5;
    Vertex v7 = add_vertex(g);
    vertex_service_props_map[v7] = 2.5;
    vertex_index_props_map[v7] = 6;
    Vertex v8 = add_vertex(g);
    vertex_service_props_map[v8] = 1.5;
    vertex_index_props_map[v8] = 7;

    Edge e1 = (add_edge(v1, v2, g)).first;
    Edge e2 = (add_edge(v1, v3, g)).first;
    Edge e3 = (add_edge(v3, v4, g)).first;
    Edge e4 = (add_edge(v3, v5, g)).first;
    Edge e5 = (add_edge(v5, v6, g)).first;
    Edge e6 = (add_edge(v5, v7, g)).first;
    Edge e7 = (add_edge(v5, v8, g)).first;

    typedef graph_traits<Graph>::vertex_iterator vertex_iter;
    vertex_iter viter_start, viter_end;
    for (boost::tie(viter_start, viter_end) = vertices(g); viter_start != viter_end; ++viter_start) {
        std::cout << vertex_service_props_map[*viter_start] << std::endl;
    }


    // calculate the accumulated service rate
    graph_property_iter_range<Graph, vertex_service_rate_t>::iterator ci, ci_end;
    tie(ci, ci_end) = get_property_iter_range(g, vertex_service_rate);
    std::cout << "total service rate: " << std::accumulate(ci, ci_end, 0.0) << std::endl;
    std::cout << "num vertices: " << num_vertices(g) << std::endl;

    // calculate the ranking of service rates

    // a vector to hold the discover time property for each vertex
    std::vector <float> service_rates(num_vertices(g));

    // Use std::sort to order the vertices by their discover time
    std::vector<graph_traits<Graph>::vertices_size_type> service_rate_order(num_vertices(g));
     integer_range<int> range(0, 8);
     std::copy(range.begin(), range.end(), service_rate_order.begin());
     std::copy(ci, ci_end, service_rates.begin());

     std::sort(service_rate_order.begin(), service_rate_order.end(),
               indirect_cmp < float*, std::greater < float > >(&service_rates[0]));

     // print order of the service rates
     for (int i = 0; i < num_vertices(g); ++i)
        std::cout << service_rate_order[i] << " ";
     std::cout << std::endl;

     // print the respective service rates
     for (int i = 0; i < num_vertices(g); ++i)
         std::cout << service_rates[service_rate_order[i]] << std::endl;
     std::cout << std::endl;

     // get the vertices in decending service rate order
     for (int i = 0; i < num_vertices(g); ++i)
         std::cout << vertex_service_props_map[vertex(service_rate_order[i], g)] << std::endl;
     std::cout << std::endl;

//     for (int i = 0; i < num_vertices(g); ++i)
//     std::cout << get(vertex_service_rate, g, 1) << std::endl;
//     std::cout << std::endl;

    // start with a random network
    // iterate until network reaches a certain size
    // 1. add new node with n outlinks to existing ones
    // 2. select the outlinks acc. to a certain probability acc. to
    //    the relative strength of each node
    // 2.1 Do we need to sort the network in BGL?

    return EXIT_SUCCESS;
}
