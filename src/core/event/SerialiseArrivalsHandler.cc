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

/** @file SerialiseArrivalsHandler.cc
 * Implementation of a serialise arrivals handler.
 */
#include "Entry.hh"
namespace dcommon = des::common;

#include "events.hh"
#include "AdminEvent.hh"
#include "ArrivalsChannel.hh"
#include "ConfigChannel.hh"
#include "EventGenerator.hh"
#include "QueueChannel.hh"
#include "SerialiseArrivalsHandler.hh"


namespace des
{
namespace core
{


SerialiseArrivalsHandler::SerialiseArrivalsHandler(DesBus &p_bus)
    : m_queue((dynamic_cast<QueueChannel&> (p_bus.getChannel(id::QUEUE_CHANNEL))).getQueue()),
      m_desArgs(((dynamic_cast<ConfigChannel&> (p_bus.getChannel(id::CONFIG_CHANNEL))).getConfig())),
      m_arrivals((dynamic_cast<ArrivalsChannel&> (p_bus.getChannel(id::ARRIVAL_CHANNEL))).getArrivals())
{
    m_currentTimeStep = new boost::uint16_t[m_desArgs.net_size];
    m_interval = m_desArgs.stop_time / m_desArgs.mfrw_T;

    for (boost::uint16_t i = 0; i < m_desArgs.net_size; ++i) {
        m_currentTimeStep[i] = 0;
    }
}


SerialiseArrivalsHandler::~SerialiseArrivalsHandler()
{
    delete [] m_currentTimeStep;
}


void SerialiseArrivalsHandler::update(AdminEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();
    if (entry->getType() == SERIALISE_ARRIVAL_EVENT) {
        int dest = entry->getDestination();
#ifndef NDEBUG_EVENTS
        std::cout << "SerialiseArrivalHandler -- serialise arrival admin event for vertex " << dest << std::endl;
#endif /* NDEBUG */

        m_arrivals->serialise(dest, m_currentTimeStep[dest]);
        m_currentTimeStep[dest]++;

#ifndef NDEBUG_EVENTS
        std::cout << "generate serialise arrival admin event for vertex " << dest << std::endl;
#endif /* NDEBUG */
        
        double time = m_interval * m_currentTimeStep[dest];
        EventGenerator::generateSerialiseArrivalAdmin(m_queue, dest, time);
    }
}



}
}
