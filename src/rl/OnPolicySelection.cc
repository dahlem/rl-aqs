// Copyright (C) 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This program is free software ; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation	 ; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY	; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program	  ; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

/** @file OnPolicySelection.cc
 * Implementation of the onPolicy selection function object
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NDEBUG_EVENTS
# include <iostream>
#endif /* NDEBUG_EVENTS */

#include <boost/foreach.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "OnPolicySelection.hh"


namespace des
{
namespace rl
{

OnPolicySelection::OnPolicySelection(
    Policy &p_policy, dnet::Graph &p_graph)
    : Selection(p_policy), m_graph(p_graph)
{
    vertex_next_action_map = get(vertex_next_action, m_graph);
}


boost::int32_t OnPolicySelection::operator() (boost::int32_t p_source)
{
#ifndef NDEBUG_EVENTS
    std::cout << "** OnPolicy Selection" << std::endl;
#endif /* NDEBUG_EVENTS */

    dnet::Vertex vertex = boost::vertex(p_source, m_graph);
    boost::int32_t destination = vertex_next_action_map[vertex];

#ifndef NDEBUG_EVENTS
    std::cout << "Destination: " << destination << std::endl;
#endif /* NDEBUG_EVENTS */

    if (destination < 0) {
#ifndef NDEBUG_EVENTS
        std::cout << "get edge_q_val map" << std::endl;
#endif /* NDEBUG_EVENTS */
        dnet::EdgeQValueMap edge_q_val_map = get(edge_q_val, m_graph);

#ifndef NDEBUG_EVENTS
        std::cout << "get vertex_index map" << std::endl;
#endif /* NDEBUG_EVENTS */
        dnet::VertexIndexMap vertex_index_map = get(boost::vertex_index, m_graph);

#ifndef NDEBUG_EVENTS
        std::cout << "instantiate the vector of action/values: " << boost::out_degree(vertex, m_graph) << std::endl;
#endif /* NDEBUG_EVENTS */
        tValuesVec values(boost::out_degree(vertex, m_graph));

#ifndef NDEBUG_EVENTS
        std::cout << "populate the action/value vector" << std::endl;
#endif /* NDEBUG_EVENTS */
        int count = 0;
        BOOST_FOREACH(dnet::Edge e, (boost::out_edges(vertex, m_graph))) {
            tValues value;
            int target_vertex = vertex_index_map[boost::target(e, m_graph)];
            value.first = target_vertex;
            value.second = edge_q_val_map[e];

#ifndef NDEBUG_EVENTS
            std::cout << "Action-Value Pair: " << value.first << ", " << value.second << std::endl;
#endif /* NDEBUG_EVENTS */
            values[count++] = value;
#ifndef NDEBUG_EVENTS
            std::cout << "Pushed value." << std::endl;
#endif /* NDEBUG_EVENTS */
        }

        PAttr attr(0.0, PolicyContext::selection());
        destination = m_policy(p_source, values, attr);
    }

#ifndef NDEBUG_EVENTS
    std::cout << "Destination: " << destination << std::endl;
#endif /* NDEBUG_EVENTS */

    return destination;
}



}
}
