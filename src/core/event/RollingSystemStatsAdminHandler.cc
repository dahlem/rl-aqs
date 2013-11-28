// Copyright (C) 2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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

/** @file RollingSystemStatsAdminHandler.cc
 * Implementation of a handler to generate external arrival events.
 */
#include <numeric>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/property_iter_range.hpp>

#include "Entry.hh"
namespace dcommon = des::common;

#include "events.hh"
#include "EventGenerator.hh"
#include "RollingSystemStatsAdminHandler.hh"
#include "ConfigChannel.hh"
#include "GraphChannel.hh"
#include "QueueChannel.hh"



namespace des
{
namespace core
{


RollingSystemStatsAdminHandler::RollingSystemStatsAdminHandler(
    DesBus& p_bus)
    : m_graph((dynamic_cast<GraphChannel&> (p_bus.getChannel(id::GRAPH_CHANNEL))).getGraph()),
      m_queue((dynamic_cast<QueueChannel&> (p_bus.getChannel(id::QUEUE_CHANNEL))).getQueue()),
      m_desArgs(((dynamic_cast<ConfigChannel&> (p_bus.getChannel(id::CONFIG_CHANNEL))).getConfig())),
      m_delay(new dstats::OnlineStats),
      m_numEvents(new dstats::OnlineStats),
      m_processingTime(new dstats::OnlineStats)
{
}


RollingSystemStatsAdminHandler::~RollingSystemStatsAdminHandler()
{
    delete m_delay;
    delete m_numEvents;
    delete m_processingTime;
}


void RollingSystemStatsAdminHandler::update(AdminEvent *subject)
{
#ifndef NDEBUG_EVENTS
    std::cout << "** RollingSystemStatsAdminHandler" << std::endl;
#endif /* NDEBUG_EVENTS */

    dcommon::Entry *entry = subject->getEvent();

    // update the last event time
    if (entry->getType() == SYSTEM_STATISTICS_EVENT) {
        double startCalculating = m_desArgs.stop_time
            - (static_cast<double> (m_desArgs.system_stats_steps)
               * static_cast<double> (m_desArgs.system_stats_interval));

        if (entry->getArrival() > startCalculating) {
#ifndef NDEBUG_EVENTS
            std::cout << "calculate system statistics" << std::endl;
#endif /* NDEBUG_EVENTS */

            // calculate the expected number of events
            dnet::VExpectedAverageNumEventsIterator it, it_end;
            boost::uint32_t size = boost::num_vertices(m_graph);
            double num_events = 0.0;

            tie(it, it_end) = boost::get_property_iter_range(
                m_graph, vertex_expected_average_number_event);

            num_events = std::accumulate(it, it_end, 0.0);
            num_events /= static_cast<double> (size);
            m_numEvents->push(num_events);
            boost::set_property(m_graph, graph_system_num_events, m_numEvents->mean());

            // calculate the delay in the system
            dnet::VAverageDelayQueueIterator itd, itd_end;
            double delay = 0.0;

            tie(itd, itd_end) = boost::get_property_iter_range(
                m_graph, vertex_average_delay_in_queue);

            delay = std::accumulate(itd, itd_end, 0.0);
            delay /= static_cast<double> (size);
            m_delay->push(delay);
            boost::set_property(m_graph, graph_system_delay, m_delay->mean());

            // calculate the total processing time of events in the system
            dnet::VAvgEventInSystemTimeIterator ite, ite_end;
            double processingTime = 0.0;

            tie(ite, ite_end) = boost::get_property_iter_range(
                m_graph, vertex_avg_event_in_system_time);

            processingTime = std::accumulate(ite, ite_end, 0.0);
            m_processingTime->push(processingTime);
            boost::set_property(m_graph, graph_system_processing_time, m_processingTime->mean());
        }

        double startTime = entry->getArrival() + static_cast<double> (m_desArgs.system_stats_interval);

#ifndef NDEBUG_EVENTS
        std::cout << "Generate new admin event to calculate system statistics at time " << startTime << std::endl;
#endif /* NDEBUG_EVENTS */

        if (startTime <= m_desArgs.stop_time) {
            EventGenerator::generateAdminEventType(m_queue, startTime, SYSTEM_STATISTICS_EVENT);
        }
    }
}




}
}
