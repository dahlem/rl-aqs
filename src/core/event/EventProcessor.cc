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

#include "events.hh"
#include "EventProcessor.hh"
namespace dcore = des::core;

#include "Entry.hh"
#include "LadderQueue.hh"
namespace dcommon = des::common;


dcore::EventProcessor::EventProcessor(dcommon::tQueueWP p_queue,
                                      dnet::tGraphWP p_graph,
                                      dcore::tArrivalEventWP p_arrivalEvent,
                                      dcore::tDepartureEventWP p_departureEvent)
    : m_queue(p_queue), m_graph(p_graph), m_arrivalEvent(p_arrivalEvent),
      m_departureEvent(p_departureEvent)
{}


dcore::EventProcessor::~EventProcessor()
{}


void dcore::EventProcessor::process()
{
    dcommon::tEntrySP entry;
    dcommon::tQueueSP q;
    dcore::tArrivalEventSP a;
    dcore::tDepartureEventSP d;

    if((q = m_queue.lock()) && (a = m_arrivalEvent.lock()) && (d = m_departureEvent.lock()))
    {
        while ((entry = q->dequeue()) != NULL) {
            // log the event here
            std::cout << std::setprecision(14) << entry->arrival << ","
                      << entry->destination << "," << entry->type
                      << std::endl;

            switch (entry->type) {
              case LAST_ARRIVAL_EVENT:
                  // generate new events
              case ARRIVAL_EVENT:
                  a->arrival(entry);
                  break;
              case DEPARTURE_EVENT:
                  d->departure(entry);
                  break;
              default:
                  break;
            }
        }
    }
}
