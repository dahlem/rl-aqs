// Copyright (C) 2008, 2009, 2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file LeaveHandler.cc
 * Implementation of a basic leave handler.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#include "Entry.hh"
#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "events.hh"
#include "LeaveEvent.hh"
#include "LeaveHandler.hh"
#include "GraphChannel.hh"
#include "QueueChannel.hh"


namespace des
{
namespace core
{


LeaveHandler::LeaveHandler(DesBus &p_bus)
    : m_graph((dynamic_cast<GraphChannel&> (p_bus.getChannel(id::GRAPH_CHANNEL))).getGraph()),
      m_queue((dynamic_cast<QueueChannel&> (p_bus.getChannel(id::QUEUE_CHANNEL))).getQueue()),
      m_EventInSystem(new dstats::OnlineStats[boost::num_vertices(m_graph)])
{
    m_vertexAvgEventInSystemTimeMap = get(vertex_avg_event_in_system_time, m_graph);
}


LeaveHandler::~LeaveHandler()
{
}


void LeaveHandler::update(LeaveEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();
    int origin = entry->getOrigin();

#ifndef NDEBUG_EVENTS
    std::cout << "** LeaveHandler for vertex: " << origin << std::endl;
    std::cout << "Event: " << const_cast <const dcommon::Entry&> (*entry) << std::endl;
#endif /* NDEBUG_EVENTS */

    dnet::Vertex vertex = boost::vertex(origin, m_graph);

    double inSystem = 0.0;
    if ((entry->getArrival() - entry->getExternalArrival()) > 0.0) {
        inSystem = (entry->getArrival() - entry->getExternalArrival());
    }

    m_EventInSystem[origin].push(inSystem);
    m_vertexAvgEventInSystemTimeMap[vertex] = m_EventInSystem[origin].mean();
}


}
}
