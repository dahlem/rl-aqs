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

/** @file ResponseStatsHandler.cc
 * Implementation of a basic responseStats handler.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NDEBUG_EVENTS
# include <iostream>
#endif /* NDEBUG_EVENTS */

#include "Entry.hh"
namespace dcommon = des::common;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "Stats.hh"
namespace dstats = des::statistics;

#include "events.hh"
#include "AckEvent.hh"
#include "ResponseStatsHandler.hh"


namespace des
{
namespace core
{


ResponseStatsHandler::ResponseStatsHandler(dnet::tGraphSP p_graph)
    : m_graph(p_graph)
{
    vertex_mean_response_map = get(vertex_mean_reward, *m_graph);
    vertex_num_events_processed_map = get(vertex_num_events_processed, *m_graph);
}


ResponseStatsHandler::~ResponseStatsHandler()
{}


void ResponseStatsHandler::update(AckEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();

#ifndef NDEBUG_EVENTS
    std::cout << "** ResponseStats for vertex: " << entry->getDestination() << std::endl;
    std::cout << "Event: " << const_cast <const dcommon::Entry&> (*entry) << std::endl;
#endif /* NDEBUG_EVENTS */

    dnet::Vertex vertex = boost::vertex(entry->getDestination(), *m_graph);
    double size = vertex_num_events_processed_map[vertex];
    double xbar = vertex_mean_response_map[vertex];
    double x = entry->getArrival() - entry->topArrival();

#ifndef NDEBUG_EVENTS
    std::cout << "old stats -- size: " << size << ", xbar: " << xbar
              << ", x: " << x << std::endl;
#endif /* NDEBUG_EVENTS */

    double mean = 0.0;
    size++;

    if (size < 2) {
        mean = x;
    } else {
        mean = dstats::Stats::mean(size, xbar, x);
    }

#ifndef NDEBUG_EVENTS
    std::cout << "new stats -- mean: " << mean << ", size: "
              << size << std::endl;
#endif /* NDEBUG_EVENTS */

    // update the delay statistics
    vertex_num_events_processed_map[vertex] = size;
    vertex_mean_response_map[vertex] = mean;
}


}
}
