// Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file EventProcessor.hh
 * Implementation of the event processor interface.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NDEBUG
# include <cassert>
#endif /* NDEBUG */

#include <iostream>

#include "Entry.hh"
#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "events.hh"
#include "EventProcessor.hh"



namespace des
{
namespace core
{



EventProcessor::EventProcessor(dcommon::tQueueSP p_queue,
                                      tAdminEventSP p_adminEvent,
                                      tPreAnyEventSP p_preAnyEvent,
                                      tPostAnyEventSP p_postAnyEvent,
                                      tArrivalEventSP p_arrivalEvent,
                                      tDepartureEventSP p_departureEvent,
                                      tPostEventSP p_postEvent,
                                      tLastArrivalEventSP p_lastArrivalEvent,
                                      tAckEventSP p_ackEvent,
                                      tLeaveEventSP p_leaveEvent,
                                      double p_stopTime)
    : m_queue(p_queue), m_adminEvent(p_adminEvent), m_preAnyEvent(p_preAnyEvent),
      m_postAnyEvent(p_postAnyEvent), m_arrivalEvent(p_arrivalEvent),
      m_departureEvent(p_departureEvent), m_postEvent(p_postEvent),
      m_lastArrivalEvent(p_lastArrivalEvent), m_ackEvent(p_ackEvent),
      m_leaveEvent(p_leaveEvent), m_stopTime(p_stopTime)
{
#ifndef NDEBUG
    m_oldTime = 0.0;
#endif /* NDEBUG */
}


EventProcessor::~EventProcessor()
{}


bool EventProcessor::process()
{
    dcommon::Entry *entry = NULL;

    try {
        while (true) {
#ifndef NDEBUG_EVENTS
            std::cout << "** EventProcessor : dequeue event" << std::endl;
#endif /* NDEBUG_EVENTS */
            entry = m_queue->dequeue();
#ifndef NDEBUG_EVENTS
            std::cout << "** EventProcessor : event dequeued" << std::endl;
#endif /* NDEBUG_EVENTS */

            if (entry == NULL) {
                break;
            }

#ifndef NDEBUG
            double newTime = entry->getArrival();

            assert(m_oldTime <= newTime);
            m_oldTime = newTime;
#endif /* NDEBUG */

#ifndef NDEBUG_EVENTS
            std::cout << "** EventProcessor : Handle event: " << const_cast <const dcommon::Entry&> (*entry) << std::endl;
#endif /* NDEBUG_EVENTS */
            // if it is a admin event, then handle it
            if (entry->getType() == LOG_GRAPH_EVENT) {
#ifndef NDEBUG_EVENTS
                std::cout << "** EventProcessor : admin event start" << std::endl;
#endif /* NDEBUG_EVENTS */
                m_adminEvent->admin(entry);
#ifndef NDEBUG_EVENTS
                std::cout << "** EventProcessor : admin event finished" << std::endl;
#endif /* NDEBUG_EVENTS */
            } else {

                // if stop time has been reached break out and handle the event below
                if (entry->getArrival() > m_stopTime) {
                    break;
                } else {
#ifndef NDEBUG_EVENTS
                    std::cout << "** EventProcessor : pre any event start" << std::endl;
#endif /* NDEBUG_EVENTS */
                    m_preAnyEvent->preAny(entry);
#ifndef NDEBUG_EVENTS
                    std::cout << "** EventProcessor : pre any event finished" << std::endl;
#endif /* NDEBUG_EVENTS */
                }

                switch (entry->getType()) {
                  case LAST_ARRIVAL_EVENT:
                      // generate new events
#ifndef NDEBUG_EVENTS
                      std::cout << "** EventProcessor : last arrival event start" << std::endl;
#endif /* NDEBUG_EVENTS */
                      m_lastArrivalEvent->lastArrival(entry);
#ifndef NDEBUG_EVENTS
                      std::cout << "** EventProcessor : last arrival event finished" << std::endl;
#endif /* NDEBUG_EVENTS */
                  case ARRIVAL_EVENT:
                  case RESCHEDULED_EVENT:
#ifndef NDEBUG_EVENTS
                      std::cout << "** EventProcessor : arrival event start" << std::endl;
#endif /* NDEBUG_EVENTS */
                      m_arrivalEvent->arrival(entry);
#ifndef NDEBUG_EVENTS
                      std::cout << "** EventProcessor : arrival event finished" << std::endl;
#endif /* NDEBUG_EVENTS */
                      break;
                  case DEPARTURE_EVENT:
#ifndef NDEBUG_EVENTS
                      std::cout << "** EventProcessor : departure event start" << std::endl;
#endif /* NDEBUG_EVENTS */
                      m_departureEvent->departure(entry);
#ifndef NDEBUG_EVENTS
                      std::cout << "** EventProcessor : departure event finished" << std::endl;
#endif /* NDEBUG_EVENTS */
                      break;
                  case ACK_EVENT:
#ifndef NDEBUG_EVENTS
                      std::cout << "** EventProcessor : ack event start" << std::endl;
#endif /* NDEBUG_EVENTS */
                      m_ackEvent->ack(entry);
#ifndef NDEBUG_EVENTS
                      std::cout << "** EventProcessor : ack event finished" << std::endl;
#endif /* NDEBUG_EVENTS */
                      break;
                  case LEAVE_EVENT:
#ifndef NDEBUG_EVENTS
                      std::cout << "** EventProcessor : leave event start" << std::endl;
#endif /* NDEBUG_EVENTS */
                      m_leaveEvent->leave(entry);
#ifndef NDEBUG_EVENTS
                      std::cout << "** EventProcessor : leave event finished" << std::endl;
#endif /* NDEBUG_EVENTS */
                      break;
                  default:
                      break;
                }

#ifndef NDEBUG_EVENTS
                std::cout << "** EventProcessor : post any event start" << std::endl;
#endif /* NDEBUG_EVENTS */
                m_postAnyEvent->postAny(entry);
#ifndef NDEBUG_EVENTS
                std::cout << "** EventProcessor : post any event finished" << std::endl;
#endif /* NDEBUG_EVENTS */

                delete entry;
            }
        }

#ifndef NDEBUG_EVENTS
        std::cout << "** EventProcessor : post event start" << std::endl;
#endif /* NDEBUG_EVENTS */

#ifndef NDEBUG
        if (entry == NULL) {
            std::cout << "the entry is NULL!" << std::endl;
        }
#endif /* NDEBUG_EVENTS */

        m_postEvent->post(entry);

#ifndef NDEBUG_EVENTS
        std::cout << "** EventProcessor : post event finished" << std::endl;
#endif /* NDEBUG_EVENTS */

        std::cout << "Finished processing events." << std::endl;
    } catch (dcommon::QueueException &qe) {
        if (qe.errorCode() == dcommon::QueueException::QUEUE_EMPTY) {
            std::cout << "Finished processing events." << std::endl;
        } else {
            std::cout << "Exception: " << qe.what() << std::endl;

            if (entry != NULL) {
                std::cout << "Event Processed: "
                          << const_cast <const dcommon::Entry&> (*entry)
                          << std::endl;
            }

            return false;
        }
    }

#ifndef NDEBUG
    std::cout << m_queue->getInEvents() << ", " << m_queue->getOutEvents() << std::endl;
    std::cout << "LadderQ events :" << m_queue->size() << std::endl;
    assert(m_queue->getInEvents() == m_queue->getOutEvents());
#endif /* NDEBUG_EVENTS */


    return true;
}



}
}
