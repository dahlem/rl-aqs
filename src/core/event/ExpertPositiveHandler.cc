// Copyright (C) 2009-2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file ExpertPositiveHandler.cc
 * Implementation of a basic ExpertPositive handler.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NDEBUG_EVENTS
# include <iostream>
#endif /* NDEBUG_EVENTS */

#include <boost/foreach.hpp>

#include <gsl/gsl_math.h>

#include "Entry.hh"
namespace dcommon = des::common;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "Policy.hh"
#include "EpsilonGreedy.hh"
namespace drl = des::rl;

#include "events.hh"
#include "AckEvent.hh"
#include "ExpertPositiveHandler.hh"
#include "GraphChannel.hh"


namespace des
{
namespace core
{


ExpertPositiveHandler::ExpertPositiveHandler(DesBus &p_bus)
    : m_graph((dynamic_cast<GraphChannel&> (p_bus.getChannel(id::GRAPH_CHANNEL))).getGraph())
{
    vertex_expert_positive_map = get(vertex_expert_positive, m_graph);
    vertex_mean_response_map = get(vertex_mean_reward, m_graph);
    edge_q_val_map = get(edge_q_val, m_graph);
}


ExpertPositiveHandler::~ExpertPositiveHandler()
{}


void ExpertPositiveHandler::update(AckEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();

#ifndef NDEBUG_EVENTS
    std::cout << "** ExpertPositive for vertex: " << entry->getDestination() << std::endl;
    std::cout << "Event: " << const_cast <const dcommon::Entry&> (*entry) << std::endl;
#endif /* NDEBUG_EVENTS */

    dnet::Vertex vertex = boost::vertex(entry->getDestination(), m_graph);
    dnet::Graph::degree_size_type degree =
        boost::out_degree(vertex, m_graph);

    // observe reward (the longer it takes the smaller the reward)
    double reward =
        (gsl_fcmp(entry->topArrival(), entry->getArrival(), 1e-9) == 0)
        ? (0.0)
        : (entry->topArrival() - entry->getArrival());

    double qval = 0.0;

    if (degree > 0) {
        dnet::Edge edge = boost::edge(
            vertex, boost::vertex(entry->getOrigin(), m_graph), m_graph).first;

        qval = edge_q_val_map[edge];
    } else {
        qval = vertex_mean_response_map[vertex];
    }

    // only record improvement in the reward
    if (reward > qval) {
        vertex_expert_positive_map[vertex] += (reward - qval);
    }

#ifndef NDEBUG_EVENTS
    std::cout << "Added reward: " << reward << std::endl;
    std::cout << "To pos sum: " << vertex_expert_positive_map[vertex] << std::endl;
#endif /* NDEBUG_EVENTS */
}


}
}
