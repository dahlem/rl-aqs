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


RLResponseHandler::RLResponseHandler(dnet::tGraphSP p_graph, boost::shared_array<double> p_alpha,
                                     boost::shared_array<double> p_r, boost::uint16_t p_levels,
                                     double p_q_alpha, double p_q_lambda, drl::tPolicySP p_policy)
    : m_graph(p_graph), m_alpha(p_alpha), m_r(p_r), m_levels(p_levels),
      m_q_alpha(p_q_alpha), m_q_lambda(p_q_lambda), m_policy(p_policy)
{
    edge_index_map = get(edge_eindex, *m_graph);
    vertex_next_action_map = get(vertex_next_action, *m_graph);
    vertex_index_map = get(boost::vertex_index, *m_graph);
    edge_q_val_map = get(edge_q_val, *m_graph);
    
    m_actionValues = tDoubleVecSP(new std::vector<double>());

    BOOST_FOREACH(dnet::Edge e, (boost::edges(*m_graph))) {
        double q_val = edge_q_val_map[e];
        m_actionValues->push_back(q_val);
    }
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
    
    if (degree > 1) {
        // observe reward (the longer it takes the smaller the reward)
        double reward = entry->topArrival() - entry->getArrival();
#ifndef NDEBUG_EVENTS
        std::cout << "Reward: " << reward << std::endl;
#endif /* NDEBUG_EVENTS */
    
        // choose new action based on new state
        drl::tValuesVecSP values = drl::tValuesVecSP(new drl::tValuesVec);
    
        BOOST_FOREACH(dnet::Edge e, (boost::out_edges(vertex, *m_graph))) {
            drl::tValues value;
            int edge_index = edge_index_map[e];
            int target_vertex = vertex_index_map[boost::target(e, *m_graph)];
            value.first = target_vertex;
            value.second = (*(m_actionValues.get()))[edge_index];

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
    
        double oldQ = (*(m_actionValues.get()))[edge_index_map[oldE]];
        double newQ = oldQ + m_q_alpha *
            (reward + m_q_lambda * (*(m_actionValues.get()))[edge_index_map[newE]] - oldQ);

        (*(m_actionValues.get()))[edge_index_map[oldE]] = newQ;
        edge_q_val_map[oldE] = newQ;
    
#ifndef NDEBUG_EVENTS
        std::cout << "Old Q-value: " << oldQ << ", new Q-value: " << newQ << std::endl;
#endif /* NDEBUG_EVENTS */
    } else if (degree == 1) {
        dnet::Edge e = *(boost::out_edges(vertex, *m_graph).first);
        newAction = vertex_index_map[boost::target(e, *m_graph)];

#ifndef NDEBUG_EVENTS
        std::cout << "Deterministic Target: " << newAction << std::endl;
#endif /* NDEBUG_EVENTS */
    }

    // set new action
    vertex_next_action_map[vertex] = newAction;
}


}
}
