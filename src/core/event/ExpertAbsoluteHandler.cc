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

/** @file ExpertAbsoluteHandler.cc
 * Implementation of a ExpertAbsolute handler.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NDEBUG_EVENTS
# include <iostream>
#endif /* NDEBUG_EVENTS */

#include <cmath>
#include <gsl/gsl_math.h>

#include "Entry.hh"
namespace dcommon = des::common;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "events.hh"
#include "AckEvent.hh"
#include "ExpertAbsoluteHandler.hh"


namespace des
{
namespace core
{


ExpertAbsoluteHandler::ExpertAbsoluteHandler(dnet::Graph &p_graph)
    : m_graph(p_graph)
{
    vertex_expert_absolute_map = get(vertex_expert_absolute, m_graph);
}


ExpertAbsoluteHandler::~ExpertAbsoluteHandler()
{}


void ExpertAbsoluteHandler::update(AckEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();

#ifndef NDEBUG_EVENTS
    std::cout << "** ExpertAbsolute for vertex: " << entry->getDestination() << std::endl;
    std::cout << "Event: " << const_cast <const dcommon::Entry&> (*entry) << std::endl;
#endif /* NDEBUG_EVENTS */

    dnet::Vertex vertex = boost::vertex(entry->getDestination(), m_graph);
    dnet::Graph::degree_size_type degree = boost::out_degree(vertex, m_graph);
    boost::uint16_t newAction = 0;

    // observe reward (the longer it takes the smaller the reward)
    double reward = 0.0;
    reward = (gsl_fcmp(entry->topArrival(), entry->getArrival(), 1e-9) == 0)
        ? (0.0)
        : (entry->topArrival() - entry->getArrival());

    // set new action
    vertex_expert_absolute_map[vertex] += fabs(reward);

#ifndef NDEBUG_EVENTS
    std::cout << "Added reward: " << reward << std::endl;
    std::cout << "To total sum: " << vertex_expert_absolute_map[vertex] << std::endl;
#endif /* NDEBUG_EVENTS */
}


}
}
