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

/** @file SystemStatisticsHandler.cc
 * Implementation of a system statistics handler
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#if !defined(NDEBUG_EVENTS) || !defined(NDEBUG)
# include <iostream>
#endif /* NDEBUG_EVENTS */

#include <numeric>

#include <boost/cstdint.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/property_iter_range.hpp>

#include "Entry.hh"
namespace dcommon = des::common;

#include "SystemStatisticsHandler.hh"
#include "GraphChannel.hh"



namespace des
{
namespace core
{


SystemStatisticsHandler::SystemStatisticsHandler(DesBus &p_bus)
    : m_graph((dynamic_cast<GraphChannel&> (p_bus.getChannel(id::GRAPH_CHANNEL))).getGraph())
{}


SystemStatisticsHandler::~SystemStatisticsHandler()
{}


void SystemStatisticsHandler::update(PostEvent *subject)
{
#ifndef NDEBUG_EVENTS
    std::cout << "** SystemStatisticsHandler" << std::endl;
#endif /* NDEBUG_EVENTS */

    // calculate the expected number of events
    dnet::VExpectedAverageNumEventsIterator it, it_end;
    boost::uint32_t size = boost::num_vertices(m_graph);
    double num_events = 0.0;

    tie(it, it_end) = boost::get_property_iter_range(
        m_graph, vertex_expected_average_number_event);

    num_events = std::accumulate(it, it_end, 0.0);
    num_events /= static_cast<double> (size);
    boost::set_property(m_graph, graph_system_num_events, num_events);

    // calculate the delay in the system
    dnet::VAverageDelayQueueIterator itd, itd_end;
    double delay = 0.0;

    tie(itd, itd_end) = boost::get_property_iter_range(
        m_graph, vertex_average_delay_in_queue);

    delay = std::accumulate(itd, itd_end, 0.0);
    delay /= static_cast<double> (size);
    boost::set_property(m_graph, graph_system_delay, delay);

    // calculate the total processing time of events in the system
    dnet::VAvgEventInSystemTimeIterator ite, ite_end;
    double processingTime = 0.0;

    tie(ite, ite_end) = boost::get_property_iter_range(
        m_graph, vertex_avg_event_in_system_time);

    processingTime = std::accumulate(ite, ite_end, 0.0);
    boost::set_property(m_graph, graph_system_processing_time, processingTime);
}


}
}
