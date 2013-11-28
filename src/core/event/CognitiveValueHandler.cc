// Copyright (C) 2009, 2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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

/** @file CognitiveValueHandler.cc
 * Implementation of a CognitiveValue handler.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NDEBUG_EVENTS
# include <iostream>
#endif /* NDEBUG_EVENTS */

#include <cmath>

#include <boost/foreach.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "Entry.hh"
namespace dcommon = des::common;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "events.hh"
#include "AckEvent.hh"
#include "CognitiveValueHandler.hh"
#include "GraphChannel.hh"
#include "ConfigChannel.hh"


namespace des
{
namespace core
{


CognitiveValueHandler::CognitiveValueHandler(DesBus& p_bus)
    : m_graph((dynamic_cast<GraphChannel&> (p_bus.getChannel(id::GRAPH_CHANNEL))).getGraph()),
      m_lastTime(new double[boost::num_edges(m_graph)])
{
    dcore::desArgs_t config = (dynamic_cast<dcore::ConfigChannel&> (p_bus.getChannel(dcore::id::CONFIG_CHANNEL))).getConfig();
    m_ANeg = config.cognitive_A_neg;
    m_APos = config.cognitive_A_pos;
    m_rNeg = config.cognitive_r_neg;
    m_rPos = config.cognitive_r_pos;

    edge_emotion_map = get(edge_emotion, m_graph);
    edge_rmin_map = get(edge_rmin, m_graph);
    edge_rmax_map = get(edge_rmax, m_graph);
    edge_e_pos_map = get(edge_e_pos, m_graph);
    edge_e_neg_map = get(edge_e_neg, m_graph);
    edge_index_map = get(edge_eindex, m_graph);
    edge_q_val_map = get(edge_q_val, m_graph);

    for (boost::uint16_t i = 0; i < boost::num_edges(m_graph); ++i) {
        m_lastTime[i] = 0.0;
    }
}


CognitiveValueHandler::~CognitiveValueHandler()
{}


void CognitiveValueHandler::update(AckEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();

#ifndef NDEBUG_EVENTS
    std::cout << "** CognitiveValue for vertex: " << entry->getDestination() << std::endl;
    std::cout << "Event: " << const_cast <const dcommon::Entry&> (*entry) << std::endl;
#endif /* NDEBUG_EVENTS */

    dnet::Vertex vertex = boost::vertex(entry->getDestination(), m_graph);
    dnet::Graph::degree_size_type degree = boost::out_degree(vertex, m_graph);

    if (degree > 1) {
        // observe reward (the longer it takes the smaller the reward)
        double reward = entry->getReward();

        dnet::Edge e = boost::edge(
            vertex, boost::vertex(entry->getOrigin(), m_graph), m_graph).first;

        // decay the previous signals for all edges emanating from destination
        BOOST_FOREACH(dnet::Edge edge, (boost::out_edges(vertex, m_graph))) {
            double diff = entry->getArrival() - m_lastTime[edge_index_map[edge]];
            edge_e_pos_map[edge] = edge_e_pos_map[edge] * exp(m_rPos * diff);
            edge_e_neg_map[edge] = edge_e_neg_map[edge] * exp(m_rNeg * diff);
            m_lastTime[edge_index_map[edge]] = entry->getArrival();
        }

#ifndef NDEBUG_EVENTS
        std::cout << "reward: " << reward << ", Q: " << edge_q_val_map[e] << std::endl;
#endif /* NDEBUG_EVENTS */

        // add new signal
        double signal = reward - edge_q_val_map[e];
        if (reward > edge_q_val_map[e]) {
            edge_e_pos_map[e] = edge_e_pos_map[e] + m_APos * signal;
        } else {
            edge_e_neg_map[e] = edge_e_neg_map[e] + m_ANeg * signal;
        }

        // update the rmin, rmax values
        if (edge_rmin_map[e] > signal) {
            edge_rmin_map[e] = signal;
        }
        if (edge_rmax_map[e] < signal) {
            edge_rmax_map[e] = signal;
        }

        // bound the signal to the maximum/minimum experienced reward
        BOOST_FOREACH(dnet::Edge edge, (boost::out_edges(vertex, m_graph))) {
            double posSignal = (edge_e_pos_map[edge] > edge_rmax_map[edge]) ? edge_rmax_map[edge] : edge_e_pos_map[edge];
            double negSignal = (edge_e_neg_map[edge] < edge_rmin_map[edge]) ? edge_rmin_map[edge] : edge_e_neg_map[edge];
            edge_emotion_map[edge] = posSignal + negSignal;

#ifndef NDEBUG_EVENTS
            std::cout << "edge: " << edge_index_map[edge] << std::endl;
            std::cout << "pos signal: " << edge_e_pos_map[edge] << ", max pos. signal: " << edge_rmax_map[edge] << std::endl;
            std::cout << "neg signal: " << edge_e_neg_map[edge] << ", min neg. signal: " << edge_rmin_map[edge] << std::endl;
            std::cout << "Cognitive value: " << edge_emotion_map[edge] << " = " << posSignal << " + " << negSignal << std::endl;
#endif /* NDEBUG_EVENTS */
        }
    }
}


}
}
