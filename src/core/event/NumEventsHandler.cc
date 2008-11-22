// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file NumEventsHandler.cc
 * Implementation of a basic numEvents handler.
 */
#include "events.hh"
#include "NumEventsHandler.hh"
namespace dcore = des::core;

#include "Entry.hh"
namespace dcommon = des::common;

#include "WEvonet.hh"
namespace dnet = des::network;


dcore::NumEventsHandler::NumEventsHandler(dnet::tGraphSP p_graph)
    : m_graph(p_graph)
{
    vertex_num_events_map = get(vertex_num_events, *m_graph);
}


dcore::NumEventsHandler::~NumEventsHandler()
{}


void dcore::NumEventsHandler::update(dcore::ArrivalEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();
    dnet::Vertex vertex = boost::vertex(entry->getDestination(), *m_graph);

    // increment the number of numEvents events seen by this node
    vertex_num_events_map[vertex]++;
}