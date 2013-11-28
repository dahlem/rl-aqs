// Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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

/** @file enduce_strength.hh
 * This file implements the template classes to perform the strength allocation
 * upon arrivals of new nodes in a network.
 */

#ifndef __ENDUCE_STRENGTH_HH__
#define __ENDUCE_STRENGTH_HH__

#include <boost/graph/breadth_first_search.hpp>


namespace des { namespace network {


/** @class epidemic_vertex_arrival_visitor
 * A visitor to visit vertices to adjust the strengths within the network. The visitor
 * is based on the breadth first search algorithm of the Boost Graph Library. The
 * relevant properties are passed into the template as template arguments, which
 * include the @code{VertexArrivalMap}, @code{EdgeWeightMap}, @code{VertexIndexMap},
 * @code{StengthDiffMap}. The last argument carries the calculated differences in the
 * strength allocation.
 *
 * @author <a href="mailto:Dominik.Dahlem@gmail.com">Dominik Dahlem</a>
 */
template <class VertexArrivalMap, class EdgeWeightMap,
          class VertexIndexMap, class StrengthDiffMap>
class epidemic_vertex_arrival_visitor : public boost::default_bfs_visitor
{
public:
    epidemic_vertex_arrival_visitor(VertexArrivalMap vertex_arrival_map,
                                    EdgeWeightMap edge_weight_map,
                                    VertexIndexMap vertex_index_map,
                                    StrengthDiffMap vertex_strength_diff_map,
                                    StrengthDiffMap vertex_strength_diff_apply_map,
                                    double edge_weight, double boost_arrival, double boost_edge)
        :  m_vertex_arrival_map(vertex_arrival_map),
           m_edge_weight_map(edge_weight_map),
           m_vertex_index_map(vertex_index_map),
           m_vertex_strength_diff_map(vertex_strength_diff_map),
           m_vertex_strength_diff_apply_map(vertex_strength_diff_apply_map),
           m_edge_weight(edge_weight),
           m_boost_arrival(boost_arrival),
           m_boost_edge(boost_edge) {}

    template <typename Edge, typename Graph>
    void examine_edge(Edge e, const Graph & g) const
        {
            double update = m_vertex_strength_diff_apply_map[source(e, g)];

            if (m_edge_weight > 0) {
                update *= m_edge_weight;
            } else {
                update *= (m_edge_weight_map[e] * m_boost_edge);
            }

            // update the target strength difference
            m_vertex_strength_diff_map[target(e, g)] += update;
            // update the target apply strength difference,
            // it may not have been applied yet
            m_vertex_strength_diff_apply_map[target(e, g)] += update;
        }

    template <typename Vertex, typename Graph>
    void examine_vertex(Vertex u, const Graph & g) const
        {
            // assign the service rate to the apply strength difference
            // this only happens for the root node
            if (m_vertex_strength_diff_map[u] == 0.0) {
                m_vertex_strength_diff_apply_map[u] =
                    m_boost_arrival * m_vertex_arrival_map[u];
            }
        }

    template <typename Vertex, typename Graph>
    void finish_vertex(Vertex u, const Graph & g) const
        {
            // reset the apply strength difference field of the source vertex
            // because we applied all differences to its children
            m_vertex_strength_diff_apply_map[u] = 0.0;
        }

    VertexArrivalMap m_vertex_arrival_map;
    EdgeWeightMap m_edge_weight_map;
    VertexIndexMap m_vertex_index_map;
    StrengthDiffMap m_vertex_strength_diff_map;
    StrengthDiffMap m_vertex_strength_diff_apply_map;
    double m_edge_weight;
    double m_boost_arrival;
    double m_boost_edge;
};


/** @class epidemic_vertex_arrival_visitor
 * A visitor to visit vertices to apply the calculated strengths within the network. The visitor
 * is based on the breadth first search algorithm of the Boost Graph Library. The
 * relevant properties are passed into the template as template arguments, which
 * include the @code{VertexServiceMap}, @code{StengthDiffMap}. The last argument
 * carries the calculated differences in the strength allocation.
 *
 * @author <a href="mailto:Dominik.Dahlem@gmail.com">Dominik Dahlem</a>
 */
template <class VertexServiceMap, class StrengthDiffMap>
class apply_enduced_strength_visitor : public boost::default_bfs_visitor
{
public:
    apply_enduced_strength_visitor(VertexServiceMap vertex_service_map,
                                   StrengthDiffMap vertex_strength_diff_map)
        :  m_vertex_service_map(vertex_service_map),
           m_vertex_strength_diff_map(vertex_strength_diff_map) {}

    template <typename Vertex, typename Graph>
    void discover_vertex(Vertex u, const Graph & g) const
        {
            // apply the enduced strength
            m_vertex_service_map[u] += m_vertex_strength_diff_map[u];
        }

    VertexServiceMap m_vertex_service_map;
    StrengthDiffMap m_vertex_strength_diff_map;
};

    }
}

#endif /* __ENDUCE_STRENGTH_HH__ */
