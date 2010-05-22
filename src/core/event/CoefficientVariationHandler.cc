// Copyright (C) 2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file CoefficientVariationHandler.cc
 * Implementation of a CoefficientVariation handler.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NDEBUG_EVENTS
# include <iostream>
#endif /* NDEBUG_EVENTS */

#include <cmath>
#include <numeric>

#include "Entry.hh"
namespace dcommon = des::common;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "events.hh"
#include "AckEvent.hh"
#include "CoefficientVariationHandler.hh"
#include "GraphChannel.hh"
#include "ConfigChannel.hh"


namespace des
{
namespace core
{


CoefficientVariationHandler::CoefficientVariationHandler(DesBus& p_bus)
    : m_graph((dynamic_cast<GraphChannel&> (p_bus.getChannel(id::GRAPH_CHANNEL))).getGraph()),
      m_circBuffer(new CircularBuffer[boost::num_vertices(m_graph)])
{
    dcore::desArgs_t config = (dynamic_cast<dcore::ConfigChannel&> (p_bus.getChannel(dcore::id::CONFIG_CHANNEL))).getConfig();

    for (boost::uint16_t i = 0; i < boost::num_vertices(m_graph); ++i) {
        m_circBuffer[i].set_capacity(config.coefficient_variation_window);
    }

    edge_index_map = get(edge_eindex, m_graph);
    edge_q_val_map = get(edge_q_val, m_graph);
    vertex_coeff_var_map = get(vertex_coeff_var, m_graph);
}


CoefficientVariationHandler::~CoefficientVariationHandler()
{
    delete [] m_circBuffer;
}


void CoefficientVariationHandler::update(AckEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();

#ifndef NDEBUG_EVENTS
    std::cout << "** CoefficientVariation for vertex: " << entry->getDestination() << std::endl;
    std::cout << "Event: " << const_cast <const dcommon::Entry&> (*entry) << std::endl;
#endif /* NDEBUG_EVENTS */

    dnet::Vertex vertex = boost::vertex(entry->getDestination(), m_graph);
    dnet::Graph::degree_size_type degree = boost::out_degree(vertex, m_graph);

    if (degree > 0) {
        dnet::Edge edge = boost::edge(
            vertex, boost::vertex(entry->getOrigin(), m_graph), m_graph).first;
        m_circBuffer[entry->getDestination()].push_back(edge_q_val_map[edge]);

        double cv = 0.0;
        double mu =  std::accumulate(m_circBuffer[entry->getDestination()].begin(),
                                     m_circBuffer[entry->getDestination()].end(),
                                     0.0)
            / static_cast<double> (m_circBuffer[entry->getDestination()].size());
        double error = 0.0;
        double temp = 0.0;

        for (boost::uint16_t i = 0; i < m_circBuffer[entry->getDestination()].size(); ++i) {
            temp = m_circBuffer[entry->getDestination()][i] - mu;
            error += temp * temp;
        }

        cv = sqrt(error / static_cast<double> (m_circBuffer[entry->getDestination()].size())) / mu;

        vertex_coeff_var_map[vertex] = cv;
    }
}


}
}
