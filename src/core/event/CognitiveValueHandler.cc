// Copyright (C) 2009, 2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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
    // observe reward (the longer it takes the smaller the reward)
    double reward = entry->getReward();

    // update the rmin, rmax values
    dnet::Edge e = boost::edge(
        boost::vertex(entry->getDestination(), m_graph),
        boost::vertex(entry->getOrigin(), m_graph), m_graph).first;

    if (edge_rmin_map[e] > reward) {
        edge_rmin_map[e] = reward;
    }
    if (edge_rmax_map[e] < reward) {
        edge_rmax_map[e] = reward;
    }

    // decay the previous signals
    double diff = entry->getArrival() - m_lastTime[edge_index_map[e]];
    edge_e_pos_map[e] = edge_e_pos_map[e] * exp(m_rPos * diff);
    edge_e_neg_map[e] = edge_e_neg_map[e] * exp(m_rNeg * diff);
    m_lastTime[edge_index_map[e]] = entry->getArrival();

    // add new signal
    double signal = reward - edge_q_val_map[e];
    edge_e_pos_map[e] = edge_e_pos_map[e] + m_APos * signal;
    edge_e_neg_map[e] = edge_e_neg_map[e] + m_ANeg * signal;

    double upper = (edge_e_pos_map[e] > edge_rmax_map[e]) ? edge_rmax_map[e] : edge_e_pos_map[e];
    double lower = 0.0;

#ifndef NDEBUG_EVENTS
#endif /* NDEBUG_EVENTS */
}


}
}
