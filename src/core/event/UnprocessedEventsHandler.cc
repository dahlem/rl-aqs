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

/** @file UnprocessedEventHandler.cc
 * Implementation of a basic unprocessedEvent handler.
 */
#include <iomanip>
#include <string>

#include "UnprocessedEventsHandler.hh"
namespace dcore = des::core;

#include "Entry.hh"
namespace dcommon = des::common;



dcore::UnprocessedEventsHandler::UnprocessedEventsHandler(dio::tResultsSP p_unprocessedEvents,
                                                          dcommon::tQueueSP p_queue)
    : m_unprocessedEvents(p_unprocessedEvents), m_queue(p_queue)
{
    std::stringstream s;
    s << dcommon::Entry::header();
    m_unprocessedEvents->print(s);
}


dcore::UnprocessedEventsHandler::~UnprocessedEventsHandler()
{}


void dcore::UnprocessedEventsHandler::update(dcore::PostEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();

    // record the events left over
    if (entry != NULL) {
        std::stringstream s;

        do {
            s.str("");
            s << std::setprecision(14) << const_cast<const dcommon::Entry&> (*entry);
            m_unprocessedEvents->print(s);
            delete entry;
        } while ((entry = m_queue->dequeue()) != NULL);
    }
}
