// Copyright (C) 2009-2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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

/** @file RandomSelection.cc
 * Implementation of the random selection function object
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NDEBUG_EVENTS
# include <iostream>
#endif /* NDEBUG_EVENTS */

#include <algorithm>
#include <vector>

#include <boost/foreach.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include <boost/pending/integer_range.hpp>

#include <gsl/gsl_randist.h>

#include "GraphChannel.hh"
namespace dcore = des::core;

#include "RandomSelection.hh"


namespace des
{
namespace rl
{

RandomSelection::RandomSelection(
    dcore::DesBus &p_bus, Policy &p_policy, Int32SA p_depart_uniform_ids)
    : Selection(p_policy),
      m_graph((dynamic_cast<dcore::GraphChannel&> (p_bus.getChannel(dcore::id::GRAPH_CHANNEL))).getGraph()),
      m_depart_uniform_ids(p_depart_uniform_ids)
{
    edge_weight_map = get(boost::edge_weight, m_graph);
    vertex_index_map = get(boost::vertex_index, m_graph);
}


boost::int32_t RandomSelection::operator() (boost::int32_t p_source, PAttr p_attr)
{
#ifndef NDEBUG_EVENTS
    std::cout << "** Random Selection" << std::endl;
#endif /* NDEBUG_EVENTS */
    dnet::Vertex vertex = boost::vertex(p_source, m_graph);
    dnet::Graph::degree_size_type degree = boost::out_degree(vertex, m_graph);
    boost::int32_t destination = -1;
    dnet::OutEdgeIterator out_edge_it, out_edge_it_end;

    tie(out_edge_it, out_edge_it_end) = boost::out_edges(vertex, m_graph);

    if (degree == 1) {
        destination = vertex_index_map[target(*out_edge_it, m_graph)];
    } else if (degree > 1) {
        std::vector <dnet::Edge> edges(degree);
        std::vector <double> edge_weights(degree);
        std::vector <int> sorted_edge_weights(degree);
        boost::integer_range <int> range(0, degree);

        // copy the edges into a vector
        std::copy(out_edge_it, out_edge_it_end, edges.begin());

        // copy the index range into the service_rate_order vector
        std::copy(range.begin(), range.end(), sorted_edge_weights.begin());

        int count = 0;
        BOOST_FOREACH(dnet::Edge e, (boost::out_edges(vertex, m_graph))) {
            edge_weights[count++] = edge_weight_map[e];
        }

        // sort the sorted_edge_weights according to the edge_weights in descending order
        std::sort(sorted_edge_weights.begin(), sorted_edge_weights.end(),
                  boost::indirect_cmp <double*, std::greater <double> >(&edge_weights[0]));

        double temp = 0.0;
        dsample::tGslRngSP depart_uniform_rng =
            dsample::CRN::getInstance().get(m_depart_uniform_ids[p_source]);
        double u = gsl_rng_uniform(depart_uniform_rng.get());

#ifndef NDEBUG_EVENTS
        std::cout << "Out-Degree: " << degree << std::endl;
        std::cout << "Random num: " << u << std::endl;
#endif /* NDEBUG_EVENTS */
        for (boost::uint32_t e = 0; e < degree; ++e) {
            dnet::Edge edge = edges[sorted_edge_weights[e]];
            temp += edge_weights[sorted_edge_weights[e]];

#ifndef NDEBUG_EVENTS
            std::cout << "Accum. weight: " << temp << std::endl;
#endif /* NDEBUG_EVENTS */
            if (u < temp) {
                // schedule an internal arrival event
                destination = vertex_index_map[target(edge, m_graph)];

                break;
            }
        }

        // in some rare occasion, the above could finish without finding the destination
        // so we return the last element in the list
        if (destination == -1) {
            destination = vertex_index_map[target(edges[degree - 1], m_graph)];
        }
    }
#ifndef NDEBUG_EVENTS
    std::cout << "Destination: " << destination << std::endl;
#endif /* NDEBUG_EVENTS */

    return destination;
}



}
}
