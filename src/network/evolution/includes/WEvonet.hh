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

/** @file WEvonet.hh
 * Declaration of the methods for the weighted directed network generation model.
 *
 * @author Dominik Dahlem
 */
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

#include <gsl/gsl_rng.h>


/** @enum vertex_service_rate_t
 * This enum extends the vertex properties by a service_rate attribute.
 */
enum vertex_service_rate_t { vertex_service_rate = 1111 };


namespace boost
{
    BOOST_INSTALL_PROPERTY(vertex, service_rate);
}


namespace des
{
    namespace network
    {

/** @typedef VertexServiceRateProperty
 * Specifies the property for the vertex service rate
 */
typedef property<vertex_service_rate_t, float> VertexServiceRateProperty;

/** @typedef EdgeWeightProperty
 * Specifies the property for the edge weight
 */
typedef property<edge_weight_t, float> EdgeWeightProperty;

/** @typedef Graph
 * Specifies the Graph as an adjacency list. The edges are represented in a
 * set which means that no parallel edges are allowed. However, of the available
 * STL containers, this is the most expensive in terms of space complexity. The Vertices
 * are stored in a STL list to allow quick addition and removal with constant time
 * complexity.
 * @todo ddahlem: verify whether bidirectionality is required.
 */
typedef adjacency_list<setS,
                       listS,
                       bidirectionalS,
                       property<vertex_index_t, uint32_t, VertexServiceRateProperty>,
                       EdgeWeightProperty> Graph;

/** @typedef Vertex
 * Specifies the vertex descriptor of a graph
 */
typedef graph_traits<Graph>::vertex_descriptor Vertex;

/** @typedef Edge
 * Specifies the edge descriptor of a graph
 */
typedef graph_traits<Graph>::edge_descriptor Edge;

/** @typedef VertexServiceRateMap
 * Specifies the map that stores the vertex service rate property
 */
typedef property_map<Graph, vertex_service_rate_t>::type VertexServiceRateMap;

/** @typedef VertexIndexMap
 * Specifies the map that stores the vertex indeces
 */
typedef property_map<Graph, vertex_index_t>::type VertexIndexMap;

/** @typedef EdgeWeightMap
 * Specifies the edge weight property
 */
typedef property_map<Graph, edge_weight_t>::type EdgeWeightMap;

/** @typedef VServiceIterator
 * Specifies the iterator for the vertex service rates
 */
typedef graph_property_iter_range<Graph, vertex_service_rate_t>::iterator VServiceIterator;

/** @typedef OutEdgeIterator
 * Specifies the iterator for the out degree edges
 */
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIterator;

/** @typedef tGraphSP
 * Specifies shared pointer to the Graph object
 */
typedef shared_ptr<Graph> tGraphSP;

/** @typedef tGslRngSP
 * Specifies shared pointer to the GSL random number generator objects
 */
typedef shared_ptr<gsl_rng> tGslRngSP;



/** @class WEvonet
 * This class specifies the interface to the weighted directed graph evolution model.
 * The model implemented is detailed in a paper published to the Physical Review Letters
 * by Alain Barrat titled "Weighted Evolving Networks: Coupling Topology and Weight Dynamics.
 */
class WEvonet
{
public:
    /**
     * Constructor which evolves a weighted directed network given a particular size.
     *
     * @param int the size of the network
     * @param tGslRngSP the GSL random number to draw the number of edges a new
     *        vertex is linked up to
     * @param tGslRngSP the GSL random number to draw a uniform number to decide
     *        which vertex to link up to.
     */
    WEvonet(int p_size, tGslRngSP p_edge_rng, tGslRngSP p_uniform_rng);

    /**
     * Destructor
     */
    ~WEvonet();

    /** @fn void advance(int p_steps)
     * Evolve the network for a further given steps
     *
     * @param int the number of iterations (i.e., vertices to add).
     */
    void advance(int p_steps);

    /** @fn void print(const std::string& filename)
     * Print the graph using the graphviz interface of BGL into a file
     *
     * @param const std::string& the filename to be printed into
     */
    void print(const std::string& filename);

private:

    /** @fn void assign_edge_weights(Vertex &v)
     * Assign the edge weights to the created edges of the newly created
     * vertex.
     *
     * @param Vertex& The newly created vertex
     */
    void assign_edge_weights(Vertex &v);

    /**
     * The random number generator to determine how many edges should be
     * created originating from the newly created vertex.
     */
    tGslRngSP num_edges_rng;

    /**
     * The random number generator to determine a uniform random number in the range
     * [0, 1) to decide which vertices to link up to.
     */
    tGslRngSP uniform_rng;

    /**
     * The BGL graph object
     */
    tGraphSP g;
};


    }
}


#endif
