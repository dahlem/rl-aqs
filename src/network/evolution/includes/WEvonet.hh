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

#ifndef __WEVONET_HH__
#define __WEVONET_HH__

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <boost/graph/adjacency_list.hpp>
using boost::adjacency_list;
using boost::bidirectionalS;
using boost::graph_traits;
using boost::listS;
using boost::setS;
using boost::vertex_index_t;
using boost::edge_weight_t;

#include <boost/graph/properties.hpp>
using boost::property;
using boost::property_map;

#include <boost/graph/property_iter_range.hpp>
using boost::graph_property_iter_range;



enum vertex_service_rate_t { vertex_service_rate = 1111 };

namespace boost
{
    BOOST_INSTALL_PROPERTY(vertex, service_rate);
}


namespace des
{
    namespace network
    {

//using boost::vertex_service_rate_t;

typedef property<vertex_service_rate_t, float> VertexServiceRateProperty;
typedef property<edge_weight_t, float> EdgeWeightProperty;

typedef adjacency_list<setS,    // no parallel edges; most expensive space complexity
                       listS,   // add/remove verteces quickly; constant time complexity
                       bidirectionalS,
                       property<vertex_index_t, uint32_t, VertexServiceRateProperty>,
                       EdgeWeightProperty> Graph;

typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef property_map<Graph, vertex_service_rate_t>::type VertexServiceRateMap;
typedef property_map<Graph, vertex_index_t>::type VertexIndexMap;
typedef property_map<Graph, edge_weight_t>::type EdgeWeightMap;

typedef graph_property_iter_range<Graph, vertex_service_rate_t>::iterator VServiceIterator;
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIterator;

class WEvonet
{
public:
    WEvonet(int p_size);
    ~WEvonet();

    void advance(int p_steps);
    void print(const std::string& filename);

private:
    shared_ptr<Graph> g;
    VertexServiceRateMap vertex_service_props_map;
    VertexIndexMap vertex_index_props_map;
    EdgeWeightMap edge_weight_props_map;
};


    }
}


#endif
