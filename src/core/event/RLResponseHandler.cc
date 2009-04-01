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

/** @file RLResponseHandler.cc
 * Implementation of a basic RLResponse handler.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NDEBUG_EVENTS
# include <iostream>
#endif /* NDEBUG_EVENTS */

#include <boost/foreach.hpp>

#include "Entry.hh"
namespace dcommon = des::common;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "Policy.hh"
#include "EpsilonGreedy.hh"
namespace drl = des::rl;

#include "events.hh"
#include "AckEvent.hh"
#include "RLResponseHandler.hh"


namespace des
{
namespace core
{


RLResponseHandler::RLResponseHandler(dnet::tGraphSP p_graph, double p_q_alpha, double p_q_lambda,
                                     drl::tPolicySP p_policy)
    : m_graph(p_graph), m_q_alpha(p_q_alpha), m_q_lambda(p_q_lambda), m_policy(p_policy),
      qStatsSA(new dstats::OnlineStats[boost::num_edges(*m_graph)])
{
    vertex_next_action_map = get(vertex_next_action, *m_graph);
    vertex_index_map = get(boost::vertex_index, *m_graph);
    edge_q_val_map = get(edge_q_val, *m_graph);
    edge_index_map = get(edge_eindex, *m_graph);
}


RLResponseHandler::~RLResponseHandler()
{}


void RLResponseHandler::update(AckEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();

#ifndef NDEBUG_EVENTS
    std::cout << "** RLResponse for vertex: " << entry->getDestination() << std::endl;
    std::cout << "Event: " << const_cast <const dcommon::Entry&> (*entry) << std::endl;
#endif /* NDEBUG_EVENTS */

    dnet::Vertex vertex = boost::vertex(entry->getDestination(), *m_graph);
    dnet::Graph::degree_size_type degree = boost::out_degree(vertex, *m_graph);
    boost::uint16_t newAction = 0;

    // observe reward (the longer it takes the smaller the reward)
    double reward = entry->topArrival() - entry->getArrival();

    if (degree > 1) {
#ifndef NDEBUG_EVENTS
        std::cout << "Reward: " << reward << std::endl;
#endif /* NDEBUG_EVENTS */

        // choose new action based on new state
        drl::tValuesVecSP values = drl::tValuesVecSP(new drl::tValuesVec);

        BOOST_FOREACH(dnet::Edge e, (boost::out_edges(vertex, *m_graph))) {
            drl::tValues value;
            int target_vertex = vertex_index_map[boost::target(e, *m_graph)];
            value.first = target_vertex;
            value.second = edge_q_val_map[e];

#ifndef NDEBUG_EVENTS
            std::cout << "Action-Value Pair: " << value.first << ", " << value.second << std::endl;
#endif /* NDEBUG_EVENTS */
            values->push_back(value);
        }

        newAction = (*m_policy)(entry->getDestination(), values);
#ifndef NDEBUG_EVENTS
        std::cout << "New Action: " << newAction << std::endl;
#endif /* NDEBUG_EVENTS */

        // calculate new q-value
        dnet::Edge oldE = boost::edge(
            vertex, boost::vertex(entry->getOrigin(), *m_graph), *m_graph).first;
        dnet::Edge newE = boost::edge(
            vertex, boost::vertex(newAction, *m_graph), *m_graph).first;

        double oldQ = edge_q_val_map[oldE];
        double newQ = oldQ + m_q_alpha *
            (reward + m_q_lambda * edge_q_val_map[newE] - oldQ);

        edge_q_val_map[oldE] = newQ;

#ifndef NDEBUG_EVENTS
        std::cout << "Old Q-value: " << oldQ << ", new Q-value: " << newQ << std::endl;
#endif /* NDEBUG_EVENTS */
    } else if (degree == 1) {
        dnet::Edge e = *(boost::out_edges(vertex, *m_graph).first);
        newAction = vertex_index_map[boost::target(e, *m_graph)];

        // calculate q-value
        boost::uint16_t edge_index = edge_index_map[e];
        qStatsSA[edge_index].push(reward);

#ifndef NDEBUG_EVENTS
        std::cout << "Old Q-value: " << edge_q_val_map[e]
                  << ", new Q-value: " << qStatsSA[edge_index].mean() << std::endl;
        std::cout << "Deterministic Target: " << newAction << std::endl;
#endif /* NDEBUG_EVENTS */

        edge_q_val_map[e] = qStatsSA[edge_index].mean();
    }

    // set new action
    vertex_next_action_map[vertex] = newAction;
}


}
}
