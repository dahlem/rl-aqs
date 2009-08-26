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

/** @file ExpertNormalHandler.cc
 * Implementation of a ExpertNormal handler.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NDEBUG_EVENTS
# include <iostream>
#endif /* NDEBUG_EVENTS */

#include <gsl/gsl_math.h>

#include "Entry.hh"
namespace dcommon = des::common;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "events.hh"
#include "AckEvent.hh"
#include "ExpertNormalHandler.hh"


namespace des
{
namespace core
{


ExpertNormalHandler::ExpertNormalHandler(dnet::Graph &p_graph)
    : m_graph(p_graph)
{
    vertex_expert_normal_map = get(vertex_expert_normal, m_graph);
}


ExpertNormalHandler::~ExpertNormalHandler()
{}


void ExpertNormalHandler::update(AckEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();

#ifndef NDEBUG_EVENTS
    std::cout << "** ExpertNormal for vertex: " << entry->getDestination() << std::endl;
    std::cout << "Event: " << const_cast <const dcommon::Entry&> (*entry) << std::endl;
#endif /* NDEBUG_EVENTS */

    dnet::Vertex vertex = boost::vertex(entry->getDestination(), m_graph);
    // observe reward (the longer it takes the smaller the reward)
    double reward = 0.0;
    reward = (gsl_fcmp(entry->topArrival(), entry->getArrival(), 1e-9) == 0)
        ? (0.0)
        : (entry->topArrival() - entry->getArrival());

    vertex_expert_normal_map[vertex] += reward;

#ifndef NDEBUG_EVENTS
    std::cout << "Added reward: " << reward << std::endl;
    std::cout << "To total sum: " << vertex_expert_normal_map[vertex] << std::endl;
#endif /* NDEBUG_EVENTS */
}


}
}
