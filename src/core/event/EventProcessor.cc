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

/** @file EventProcessor.hh
 * Implementation of the event processor interface.
 */
#include <iostream>

#include "events.hh"
#include "EventProcessor.hh"
namespace dcore = des::core;

#include "Entry.hh"
#include "LadderQueue.hh"
namespace dcommon = des::common;



dcore::EventProcessor::EventProcessor(dcommon::tQueueSP p_queue,
                                      dcore::tAdminEventSP p_adminEvent,
                                      dcore::tPreAnyEventSP p_preAnyEvent,
                                      dcore::tPostAnyEventSP p_postAnyEvent,
                                      dcore::tArrivalEventSP p_arrivalEvent,
                                      dcore::tDepartureEventSP p_departureEvent,
                                      dcore::tPostEventSP p_postEvent,
                                      dcore::tLastArrivalEventSP p_lastArrivalEvent,
                                      double p_stopTime)
    : m_queue(p_queue), m_adminEvent(p_adminEvent), m_preAnyEvent(p_preAnyEvent),
      m_postAnyEvent(p_postAnyEvent), m_arrivalEvent(p_arrivalEvent),
      m_departureEvent(p_departureEvent), m_postEvent(p_postEvent),
      m_lastArrivalEvent(p_lastArrivalEvent), m_stopTime(p_stopTime)
{}


dcore::EventProcessor::~EventProcessor()
{}


void dcore::EventProcessor::process()
{
    dcommon::Entry *entry = NULL;

    try {
        while ((entry = m_queue->dequeue()) != NULL) {
            // if it is a admin event, then handle it
            if (entry->getType() == LOG_GRAPH_EVENT) {
                m_adminEvent->admin(entry);
            } else {

                // if stop time has been reached break out and handle the event below
                if (entry->getArrival() > m_stopTime) {
                    break;
                } else {
                    m_preAnyEvent->preAny(entry);
                }

                switch (entry->getType()) {
                  case LAST_ARRIVAL_EVENT:
                      // generate new events
                      m_lastArrivalEvent->lastArrival(entry);
                  case ARRIVAL_EVENT:
                      m_arrivalEvent->arrival(entry);
                      break;
                  case DEPARTURE_EVENT:
                      m_departureEvent->departure(entry);
                      break;
                  default:
                      break;
                }

                m_postAnyEvent->postAny(entry);

                delete entry;
            }
        }

        m_postEvent->post(entry);

        std::cout << "Finished processing events." << std::endl;
    } catch (dcommon::QueueException &qe) {
        if (qe.errorCode() == dcommon::QueueException::QUEUE_EMPTY) {
            std::cout << "Finished processing events." << std::endl;
        } else {
            std::cout << "Exception: " << qe.what() << std::endl;
        }
    }
}
