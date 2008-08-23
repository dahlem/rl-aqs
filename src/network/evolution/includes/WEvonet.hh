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

#include <limits>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/property_iter_range.hpp>

#include <gsl/gsl_rng.h>


/** @enum vertex_service_rate_t
 * This enum extends the vertex properties by a service_rate attribute.
 */
enum vertex_service_rate_t { vertex_service_rate = 1111 };


/** @enum vertex_arrival_rate_t
 * This enum extends the vertex properties by a arrival_rate attribute.
 */
enum vertex_arrival_rate_t { vertex_arrival_rate = 1112 };


/** @enum vertex_busy_t
 * This enum extends the vertex properties by an busy attribute.
 */
enum vertex_busy_t { vertex_busy = 1113 };


/** @enum vertex_time_service_ends_t
 * This enum extends the vertex properties by a time service ends attribute.
 */
enum vertex_time_service_ends_t { vertex_time_service_ends = 1114 };


/** @enum vertex_number_in_queue_t
 * This enum extends the vertex properties by a time service ends attribute.
 */
enum vertex_number_in_queue_t { vertex_number_in_queue = 1115 };


// install the vertex service rate property
namespace boost
{
    BOOST_INSTALL_PROPERTY(vertex, service_rate);
    BOOST_INSTALL_PROPERTY(vertex, arrival_rate);
    BOOST_INSTALL_PROPERTY(vertex, busy);
    BOOST_INSTALL_PROPERTY(vertex, time_service_ends);
    BOOST_INSTALL_PROPERTY(vertex, number_in_queue);
}


namespace des { namespace network {


/** @typedef VertexServiceRateProperty
 * Specifies the property for the vertex service rate
 */
typedef boost::property <vertex_service_rate_t, float> VertexServiceRateProperty;

/** @typedef VertexArrivalRateProperty
 * Specifies the property for the vertex arrival rate
 */
typedef boost::property <vertex_arrival_rate_t, float, VertexServiceRateProperty> VertexArrivalRateProperty;

/** @typedef VertexArrivalRateProperty
 * Specifies the property for the vertex arrival rate
 */
typedef boost::property <vertex_number_in_queue_t, int, VertexArrivalRateProperty> VertexNumberInQueueProperty;

/** @typedef VertexBusyProperty
 * Specifies the property for the vertex busy flag
 */
typedef boost::property <vertex_busy_t, bool, VertexNumberInQueueProperty> VertexBusyProperty;

/** @typedef VertexTimeServiceEndsProperty
 * Specifies the property for the time service ends attribute of a vertex
 */
typedef boost::property <vertex_time_service_ends_t, float, VertexBusyProperty> VertexTimeServiceEndsProperty;

/** @typedef VertexProperties
 * This type definition assembles all the properties for the vertices of the graph
 */
typedef boost::property <boost::vertex_index_t, int, VertexTimeServiceEndsProperty> VertexProperties;

/** @typedef EdgeWeightProperty
 * Specifies the property for the edge weight
 */
typedef boost::property <boost::edge_weight_t, float> EdgeWeightProperty;


/** @typedef Graph
 * Specifies the Graph as an adjacency list. The edges are represented in a
 * set which means that no parallel edges are allowed. However, of the available
 * STL containers, this is the most expensive in terms of space complexity. The Vertices
 * are stored in a STL list to allow quick addition and removal with constant time
 * complexity.
 */
typedef boost::adjacency_list<boost::setS,
                              boost::listS,
                              boost::directedS,
                              VertexProperties,
                              EdgeWeightProperty> Graph;

/** @typedef Vertex
 * Specifies the vertex descriptor of a graph
 */
typedef boost::graph_traits <Graph>::vertex_descriptor Vertex;

/** @typedef Edge
 * Specifies the edge descriptor of a graph
 */
typedef boost::graph_traits <Graph>::edge_descriptor Edge;

/** @typedef VertexServiceRateMap
 * Specifies the map that stores the vertex service rate property
 */
typedef boost::property_map <Graph, vertex_number_in_queue_t>::type VertexNumberInQueueMap;

/** @typedef VertexServiceRateMap
 * Specifies the map that stores the vertex service rate property
 */
typedef boost::property_map <Graph, vertex_service_rate_t>::type VertexServiceRateMap;

/** @typedef VertexArrivalRateMap
 * Specifies the map that stores the vertex arrival rate property
 */
typedef boost::property_map <Graph, vertex_arrival_rate_t>::type VertexArrivalRateMap;

/** @typedef VertexIndexMap
 * Specifies the map that stores the vertex indeces
 */
typedef boost::property_map <Graph, boost::vertex_index_t>::type VertexIndexMap;

/** @typedef VertexBusyMap
 * Specifies the map that stores the vertex busy flag
 */
typedef boost::property_map <Graph, vertex_busy_t>::type VertexBusyMap;

/** @typedef VertexTimeServiceEndsMap
 * Specifies the map that stores the vertex time service ends attribute
 */
typedef boost::property_map <Graph, vertex_time_service_ends_t>::type VertexTimeServiceEndsMap;

/** @typedef EdgeWeightMap
 * Specifies the edge weight property
 */
typedef boost::property_map <Graph, boost::edge_weight_t>::type EdgeWeightMap;

/** @typedef VServiceIterator
 * Specifies the iterator for the vertex service rates
 */
typedef boost::graph_property_iter_range <Graph, vertex_service_rate_t>::iterator VServiceIterator;

/** @typedef VArrivalIterator
 * Specifies the iterator for the vertex arrival rates
 */
typedef boost::graph_property_iter_range <Graph, vertex_arrival_rate_t>::iterator VArrivalIterator;

/** @typedef OutEdgeIterator
 * Specifies the iterator for the out degree edges
 */
typedef boost::graph_traits <Graph>::out_edge_iterator OutEdgeIterator;

/** @typedef tGraphSP
 * Specifies shared pointer to the Graph object
 */
typedef boost::shared_ptr <Graph> tGraphSP;

/** @typedef tGraphwP
 * Specifies weak pointer to the Graph object
 */
typedef boost::weak_ptr <Graph> tGraphWP;

/** @typedef tGslRngSP
 * Specifies shared pointer to the GSL random number generator objects
 */
typedef boost::shared_ptr <gsl_rng> tGslRngSP;



/** @class WEvonet
 * This class specifies the interface to the weighted directed graph evolution model.
 * The model implemented is detailed in a paper published to the Physical Review Letters
 * by Alain Barrat titled "Weighted Evolving Networks: Coupling Topology and Weight Dynamics.
 */
class WEvonet
{
public:
    static const int MAX_EDGES = INT_MAX;

    /** @enum GraphTypes
     * This enum declares the supported output graph formats. Currently supported
     * are the dot format (graphviz) and the graphml format.
     */
    enum GraphTypes { GRAPHVIZ, GRAPHML };

    /**
     * Constructor which evolves a weighted directed network given a particular size.
     *
     * @param int the size of the network
     * @param int the maximum number of edges
     * @param tGslRngSP the GSL random number to draw the number of edges a new
     *        vertex is linked up to
     * @param tGslRngSP the GSL random number to draw a uniform number to decide
     *        which vertex to link up to.
     * @param tGslRngSP the GSL random number to draw a uniform number for the vertex
     *        arrival rate.
     */
    WEvonet(int p_size, int p_max_edges,
            tGslRngSP p_edge_rng, tGslRngSP p_uniform_rng, tGslRngSP p_vertex_arrival_rng);

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

    /** @fn void print(const std::string& filename, GraphTypes graphType)
     * Print the graph using either of the supported graph formats. The default format
     * is graphviz.
     *
     * @param const std::string& the filename to be printed into
     * @param const GraphTypes the graph type selected for the output
     */
    void print(const std::string& filename, const GraphTypes graphType);

private:

    /** @fn void assign_edge_weights(Vertex &v)
     * Assign the edge weights to the created edges of the newly created
     * vertex.
     *
     * @param Vertex& The newly created vertex
     */
    void assign_edge_weights(Vertex &v);

    /** @fn void balance_vertex_strength(Vertex &v)
     * Balance the vertex strenghts given a starting position. The starting position
     * usually is the newly added vertex. A epidemc visit is performed to calculate the
     * enduced differences in strength of the affected nodes. Then the strengths of the
     * nodes are adjusted accordingly (here the service rate).
     *
     * @param Vertex& The vertex from which to start the adjustment
     */
    void balance_vertex_strength(Vertex &v);

    /** @fn void print_dot(const std::string& filename)
     * Print the graph using the graphviz interface of BGL into a file
     *
     * @param const std::string& the filename to be printed into
     */
    void print_dot(const std::string& filename);

    /** @fn void print_graphml(const std::string& filename)
     * Print the graph into a graphml format
     *
     * @param const std::string& the filename to be printed into
     */
    void print_graphml(const std::string& filename);

    /**
     * The random number generator to determine how many edges should be
     * created originating from the newly created vertex.
     */
    tGslRngSP num_edges_rng;

    /**
     * The random number generator to determine the vertex arrival rate when a new
     * vertex is created.
     */
    tGslRngSP vertex_arrival_rng;

    /**
     * The random number generator to determine a uniform random number in the range
     * [0, 1) to decide which vertices to link up to.
     */
    tGslRngSP uniform_rng;

    /**
     * The BGL graph object
     */
    tGraphSP g;

    /**
     * The maximum number of allowed edges for a newly created vertex.
     */
    int max_edges;
};


    }
}


#endif
