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
#include "DirectedGraph.hh"



namespace des { namespace network {


/** @class WEvonet
 * This class specifies the interface to the weighted directed graph evolution model.
 * The model implemented is detailed in a paper published to the Physical Review Letters
 * by Alain Barrat titled "Weighted Evolving Networks: Coupling Topology and Weight Dynamics.
 */
class WEvonet
{
public:


    static const boost::uint32_t MAX_EDGES = UINT_MAX;

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
    static tGraphSP createBBVGraph(boost::uint32_t p_size, boost::uint32_t p_max_edges,
                                   double p_edge_weight, double p_max_arrival_rate, tGslRngSP p_edge_rng,
                                   tGslRngSP p_uniform_rng, tGslRngSP p_vertex_arrival_rng);

    static tGraphSP createERGraph(boost::uint32_t p_size, double p_edge_weight, double p_max_arrival_rate,
                                  tGslRngSP p_vertex_arrival_rng, double p);

private:

    WEvonet()
        {}

    ~WEvonet()
        {}


    /** @fn void advance(int p_steps)
     * Evolve the network for a further given steps
     *
     * @param boost::uint32_t the number of iterations (i.e., vertices to add).
     */
    static void advance(boost::uint32_t, tGraphSP,
                        tGslRngSP, tGslRngSP, tGslRngSP,
                        double, double, boost::uint32_t);

    /** @fn void assign_edge_weights(Vertex &v)
     * Assign the edge weights to the created edges of the newly created
     * vertex.
     *
     * @param Vertex& The newly created vertex
     */
    static void assign_edge_weights(Vertex&, tGraphSP);

    /** @fn void balance_vertex_strength(Vertex &v)
     * Balance the vertex strenghts given a starting position. The starting position
     * usually is the newly added vertex. A epidemc visit is performed to calculate the
     * enduced differences in strength of the affected nodes. Then the strengths of the
     * nodes are adjusted accordingly (here the service rate).
     *
     * @param Vertex& The vertex from which to start the adjustment
     */
    static void balance_vertex_strength(Vertex&, tGraphSP, double);

};


    }
}


#endif
