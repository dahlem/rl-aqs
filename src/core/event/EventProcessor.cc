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
#include <iomanip>
#include <string>

#include "events.hh"
#include "EventProcessor.hh"
namespace dcore = des::core;

#include "Entry.hh"
#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "Results.hh"
namespace dio = des::io;


dcore::EventProcessor::EventProcessor(dcommon::tQueueSP p_queue,
                                      dnet::tGraphSP p_graph,
                                      dcore::tArrivalEventSP p_arrivalEvent,
                                      dcore::tDepartureEventSP p_departureEvent,
                                      double p_stopTime)
    : m_queue(p_queue), m_graph(p_graph), m_arrivalEvent(p_arrivalEvent),
      m_departureEvent(p_departureEvent), m_stopTime(p_stopTime)
{}


dcore::EventProcessor::~EventProcessor()
{}


void dcore::EventProcessor::setUnprocessedResults(
    dio::tResultsSP p_unprocessedEvents)
{
    m_unprocessedEvents = p_unprocessedEvents;
}


void dcore::EventProcessor::setProcessedResults(
    dio::tResultsSP p_processedEvents)
{
    m_processedEvents = p_processedEvents;
}


void dcore::EventProcessor::process()
{
    dcommon::Entry *entry = NULL;
    std::stringstream s;

    try {
        if (m_processedEvents != NULL) {
            s << "arrivalTime,origin,destination,type";
            m_processedEvents->print(s);
        }

        while ((entry = m_queue->dequeue()) != NULL) {
            // if stop time has been reached break out and handle the event below
            if (entry->arrival > m_stopTime) {
                break;
            } else {
                if (m_processedEvents != NULL) {
                    s.str("");
                    // log the event
                    s << std::setprecision(14) << entry->arrival << ","
                      << entry->origin << "," << entry->destination << "," << entry->type;

                    m_processedEvents->print(s);
                }
            }

            switch (entry->type) {
              case LAST_ARRIVAL_EVENT:
                  // generate new events
              case ARRIVAL_EVENT:
                  m_arrivalEvent->arrival(entry);
                  break;
              case DEPARTURE_EVENT:
                  m_departureEvent->departure(entry);
                  break;
              default:
                  break;
            }

            delete entry;
        }

        postProcess(entry);
    } catch (dcommon::QueueException &qe) {
        std::cout << "Exception: " << qe.what() << std::endl;
    }
}


void dcore::EventProcessor::postProcess(dcommon::Entry *p_entry)
    throw (dcommon::QueueException)
{
    dcommon::Entry *entry = p_entry;

    // record the events left over
    if (m_unprocessedEvents != NULL) {
        if (entry != NULL) {
            std::stringstream s;
            s << "arrivalTime,origin,destination,type";
            m_unprocessedEvents->print(s);

            do {
                s.str("");
                s << std::setprecision(14) << entry->arrival << ","
                  << entry->origin << "," << entry->destination << "," << entry->type;

                m_unprocessedEvents->print(s);
                delete entry;
            } while ((entry = m_queue->dequeue()) != NULL);
        }
    }
}
