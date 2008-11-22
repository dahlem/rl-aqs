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

/** @file LastEventHandler.cc
 * Implementation of a basic lastEvent handler.
 */
#include "events.hh"
#include "LastEventHandler.hh"
namespace dcore = des::core;

#include "Entry.hh"
namespace dcommon = des::common;

#include "WEvonet.hh"
namespace dnet = des::network;


dcore::LastEventHandler::LastEventHandler(dnet::tGraphSP p_graph)
    : m_graph(p_graph)
{
    vertex_last_event_time_map = get(vertex_last_event_time, *m_graph);
}


dcore::LastEventHandler::~LastEventHandler()
{}


void dcore::LastEventHandler::update(dcore::PostAnyEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();
    dnet::Vertex vertex = boost::vertex(entry->getDestination(), *m_graph);

    // update the last event time
    if ((entry->getType() == LAST_ARRIVAL_EVENT) ||
        (entry->getType() == ARRIVAL_EVENT)) {
        vertex_last_event_time_map[vertex] = entry->getArrival();
    }
}
