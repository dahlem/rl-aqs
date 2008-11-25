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

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <limits>

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/property_iter_range.hpp>

#include <gsl/gsl_rng.h>

#include "GraphException.hh"
namespace dnet = des::network;


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
 * This enum extends the vertex properties by a time in queue attribute.
 */
enum vertex_number_in_queue_t { vertex_number_in_queue = 1115 };


/** @enum vertex_average_delay_in_queue_t
 * This enum extends the vertex properties by a average delay in queue attribute.
 */
enum vertex_average_delay_in_queue_t { vertex_average_delay_in_queue = 1116 };


/** @enum vertex_num_events_t
 * This enum extends the vertex properties by a number of events attribute.
 */
enum vertex_num_events_t { vertex_num_events = 1117 };


/** @enum vertex_Bdt_t
 * This enum extends the vertex properties by a fractional utilisation attribute.
 */
enum vertex_Bdt_t { vertex_Bdt = 1118 };


/** @enum vertex_Qdt_t
 * This enum extends the vertex properties by a fractional expected number of events in queue attribute.
 */
enum vertex_Qdt_t { vertex_Qdt = 1119 };


/** @enum vertex_utilisation_t
 * This enum extends the vertex properties by a utilisation attribute.
 */
enum vertex_utilisation_t { vertex_utilisation = 1120 };


/** @enum vertex_utilisation_t
 * This enum extends the vertex properties by a expected average number of events in queue attribute.
 */
enum vertex_expected_average_number_event_t { vertex_expected_average_number_event = 1121 };


/** @enum vertex_last_event_time_t
 * This enum extends the vertex properties by a last event time attribute.
 */
enum vertex_last_event_time_t { vertex_last_event_time = 1122 };


// install the vertex service rate property
namespace boost
{
    BOOST_INSTALL_PROPERTY(vertex, service_rate);
    BOOST_INSTALL_PROPERTY(vertex, arrival_rate);
    BOOST_INSTALL_PROPERTY(vertex, busy);
    BOOST_INSTALL_PROPERTY(vertex, time_service_ends);
    BOOST_INSTALL_PROPERTY(vertex, number_in_queue);
    BOOST_INSTALL_PROPERTY(vertex, average_delay_in_queue);
    BOOST_INSTALL_PROPERTY(vertex, num_events);
    BOOST_INSTALL_PROPERTY(vertex, Bdt);
    BOOST_INSTALL_PROPERTY(vertex, Qdt);
    BOOST_INSTALL_PROPERTY(vertex, utilisation);
    BOOST_INSTALL_PROPERTY(vertex, expected_average_number_event);
    BOOST_INSTALL_PROPERTY(vertex, last_event_time);
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
 * Specifies the property for the vertex number in queue
 */
typedef boost::property <vertex_number_in_queue_t, int, VertexArrivalRateProperty> VertexNumberInQueueProperty;

/** @typedef VertexAverageDelayInQueueProperty
 * Specifies the property for the vertex average delay in queue
 */
typedef boost::property <vertex_average_delay_in_queue_t, float, VertexNumberInQueueProperty> VertexAverageDelayInQueueProperty;

/** @typedef VertexBusyProperty
 * Specifies the property for the vertex busy flag
 */
typedef boost::property <vertex_busy_t, bool, VertexAverageDelayInQueueProperty> VertexBusyProperty;

/** @typedef VertexTimeServiceEndsProperty
 * Specifies the property for the time service ends attribute of a vertex
 */
typedef boost::property <vertex_time_service_ends_t, float, VertexBusyProperty> VertexTimeServiceEndsProperty;

/** @typedef VertexNumEventsProperty
 * Specifies the property for the number of events attribute of a vertex
 */
typedef boost::property <vertex_num_events_t, int, VertexTimeServiceEndsProperty> VertexNumEventsProperty;

/** @typedef VertexUtilisationProperty
 * Specifies the property for the utlisation attribute of a vertex
 */
typedef boost::property <vertex_utilisation_t, float, VertexNumEventsProperty> VertexUtilisationProperty;

/** @typedef VertexBdtProperty
 * Specifies the property for the fractional utlisation attribute of a vertex
 */
typedef boost::property <vertex_Bdt_t, float, VertexUtilisationProperty> VertexBdtProperty;

/** @typedef VertexQdtProperty
 * Specifies the property for the fractional expected number of events in queue attribute of a vertex
 */
typedef boost::property <vertex_Qdt_t, float, VertexBdtProperty> VertexQdtProperty;

/** @typedef VertexExpectedAverageNumberEventProperty
 * Specifies the property for the expected average number of events in queue attribute of a vertex
 */
typedef boost::property <vertex_expected_average_number_event_t, float, VertexQdtProperty> VertexExpectedAverageNumberEventProperty;

/** @typedef VertexLastEventTimeProperty
 * Specifies the property for the last event time attribute of a vertex
 */
typedef boost::property <vertex_last_event_time_t, float, VertexExpectedAverageNumberEventProperty> VertexLastEventTimeProperty;

/** @typedef VertexProperties
 * This type definition assembles all the properties for the vertices of the graph
 */
typedef boost::property <boost::vertex_index_t, int, VertexLastEventTimeProperty> VertexProperties;

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

/** @typedef VertexNumberInQueueMap
 * Specifies the map that stores the vertex service rate property
 */
typedef boost::property_map <Graph, vertex_number_in_queue_t>::type VertexNumberInQueueMap;

/** @typedef VertexAverageDelayInQueueMap
 * Specifies the map that stores the vertex average delay in queue property
 */
typedef boost::property_map <Graph, vertex_average_delay_in_queue_t>::type VertexAverageDelayInQueueMap;

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

/** @typedef VertexNumEventsMap
 * Specifies the map that stores the vertex number of events attribute
 */
typedef boost::property_map <Graph, vertex_num_events_t>::type VertexNumEventsMap;

/** @typedef VertexUtilisationMap
 * Specifies the map that stores the vertex utilisation attribute
 */
typedef boost::property_map <Graph, vertex_utilisation_t>::type VertexUtilisationMap;

/** @typedef VertexBdtMap
 * Specifies the map that stores the vertex fractional utilisation attribute
 */
typedef boost::property_map <Graph, vertex_Bdt_t>::type VertexBdtMap;

/** @typedef VertexQdtMap
 * Specifies the map that stores the vertex fractional expected number of events in the queue attribute
 */
typedef boost::property_map <Graph, vertex_Qdt_t>::type VertexQdtMap;

/** @typedef VertexLastEventTimeMap
 * Specifies the map that stores the vertex expected average number events attribute
 */
typedef boost::property_map <Graph, vertex_expected_average_number_event_t>::type VertexExpectedAverageNumberEventMap;

/** @typedef VertexLastEventTimeMap
 * Specifies the map that stores the vertex last event time attribute
 */
typedef boost::property_map <Graph, vertex_last_event_time_t>::type VertexLastEventTimeMap;

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

/** @typedef VIndexIterator
 * Specifies the iterator for the vertex arrival rates
 */
typedef boost::graph_property_iter_range <Graph, boost::vertex_index_t>::iterator VIndexIterator;

/** @typedef OutEdgeIterator
 * Specifies the iterator for the out degree edges
 */
typedef boost::graph_traits <Graph>::out_edge_iterator OutEdgeIterator;

/** @typedef VertexIterator
 * Specifies the iterator for the vertices
 */
typedef boost::graph_traits <Graph>::vertex_iterator VertexIterator;

/** @typedef tGraphSP
 * Specifies shared pointer to the Graph object
 */
typedef boost::shared_ptr <Graph> tGraphSP;

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
    static const std::string EDGE_WEIGHT;
    static const std::string VERTEX_ID;
    static const std::string SERVICE_RATE;
    static const std::string ARRIVAL_RATE;
    static const std::string BUSY;
    static const std::string TIME_SERVICE_ENDS;
    static const std::string NUMBER_IN_QUEUE;
    static const std::string AVERAGE_DELAY_IN_QUEUE;
    static const std::string NUM_EVENTS;
    static const std::string UTILISATION;
    static const std::string BDT;
    static const std::string QDT;
    static const std::string LAST_EVENT_TIME;
    static const std::string EXPECTED_AVERAGE_NUMBER_EVENT;


    static const boost::uint32_t MAX_EDGES = UINT_MAX;

    /** @enum GraphTypes
     * This enum declares the supported output graph formats. Currently supported
     * are the dot format (graphviz) and the graphml format.
     */
    enum GraphTypes { GRAPHVIZ, GRAPHML };

    /**
     * Constructor which evolves a weighted directed network given a particular size.
     *
     * @param boost::uint32_t the size of the network
     * @param boost::uint32_t the maximum number of edges
     * @param double the fixed edge weights (-1, if not fixed)
     * @param tGslRngSP the GSL random number to draw the number of edges a new
     *        vertex is linked up to
     * @param tGslRngSP the GSL random number to draw a uniform number to decide
     *        which vertex to link up to.
     * @param tGslRngSP the GSL random number to draw a uniform number for the vertex
     *        arrival rate.
     */
    WEvonet(boost::uint32_t p_size, boost::uint32_t p_max_edges, double p_edge_weight,
            tGslRngSP p_edge_rng, tGslRngSP p_uniform_rng, tGslRngSP p_vertex_arrival_rng);

    /**
     * Destructor
     */
    ~WEvonet();

    /** @fn void advance(int p_steps)
     * Evolve the network for a further given steps
     *
     * @param boost::uint32_t the number of iterations (i.e., vertices to add).
     */
    void advance(boost::uint32_t p_steps);

    /** @fn void print(const std::string& filename, GraphTypes graphType)
     * Print the graph using either of the supported graph formats. The default format
     * is graphviz.
     *
     * @param const std::string& the filename to be printed into
     * @param const GraphTypes the graph type selected for the output
     */
    void print(const std::string& filename, const GraphTypes graphType);

private:

    static boost::dynamic_properties getProperties(tGraphSP p_graph);

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
    boost::uint32_t max_edges;

    /**
     * Value with which to fix the edge weights
     */
    double fixed_edge_weight;

};


/** @struct exists_vertex_index
 * This is a struct defining a predicate function to filter the graph, where the subset of the
 * original graph only contains a vertex matching a specified index.
 */
template <typename VertexIndexMap>
struct exists_vertex_index {
    exists_vertex_index()
        {}

    exists_vertex_index(VertexIndexMap p_indexMap, int p_index)
        : m_indexMap(p_indexMap), m_index(p_index)
        {}

    template <typename Vertex>
    bool operator()(const Vertex& v) const
        {
            return m_index == get(m_indexMap, v);
        }

    VertexIndexMap m_indexMap;
    int m_index;
};


    }
}


#endif
