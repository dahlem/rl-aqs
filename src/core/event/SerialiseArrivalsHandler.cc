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

/** @file SerialiseArrivalsHandler.cc
 * Implementation of a serialise arrivals handler.
 */
#include <boost/foreach.hpp>

#include "Entry.hh"
namespace dcommon = des::common;

#include "events.hh"
#include "AdminEvent.hh"
#include "ArrivalsChannel.hh"
#include "ConfigChannel.hh"
#include "GraphChannel.hh"
#include "EventGenerator.hh"
#include "QueueChannel.hh"
#include "SerialiseArrivalsHandler.hh"


namespace des
{
namespace core
{


SerialiseArrivalsHandler::SerialiseArrivalsHandler(DesBus &p_bus)
    : m_graph((dynamic_cast<GraphChannel&> (p_bus.getChannel(id::GRAPH_CHANNEL))).getGraph()),
      m_queue((dynamic_cast<QueueChannel&> (p_bus.getChannel(id::QUEUE_CHANNEL))).getQueue()),
      m_desArgs(((dynamic_cast<ConfigChannel&> (p_bus.getChannel(id::CONFIG_CHANNEL))).getConfig())),
      m_arrivals((dynamic_cast<ArrivalsChannel&> (p_bus.getChannel(id::ARRIVAL_CHANNEL))).getArrivals())
{
    m_currentTimeStep = 0;
    m_interval = m_desArgs.stop_time / m_desArgs.mfrw_T;
}


SerialiseArrivalsHandler::~SerialiseArrivalsHandler()
{
}


void SerialiseArrivalsHandler::update(AdminEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();

    if (entry->getType() == SERIALISE_ARRIVAL_EVENT) {
#ifndef NDEBUG_EVENTS
        std::cout << "SerialiseArrivalHandler -- serialise arrival admin events" << std::endl;
#endif /* NDEBUG */

        // only schedule for time steps smaller than T
        if (m_currentTimeStep < m_desArgs.mfrw_T) {
            dnet::VertexIndexMap vertex_index_props_map =
                get(boost::vertex_index, m_graph);

#ifndef NDEBUG_EVENTS
            std::cout << "generate serialise arrival admin events" << std::endl;
#endif /* NDEBUG */
        
            boost::int32_t destination = 0;
            BOOST_FOREACH(dnet::Vertex v, boost::vertices(m_graph)) {
                destination = vertex_index_props_map[v];

#ifndef NDEBUG_EVENTS
                std::cout << "... for vertex " << destination << std::endl;
#endif /* NDEBUG */

                // generate the events
                m_arrivals->serialise(destination, m_currentTimeStep);
            }

            m_currentTimeStep++;

            double time = m_interval * m_currentTimeStep;
            EventGenerator::generateAdminEventType(m_queue, time, SERIALISE_ARRIVAL_EVENT);
        }
    }
}



}
}
