// Copyright (C) 2008-2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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
#if HAVE_CONFIG_H
# include <config.h>
#endif

#if !defined(NDEBUG_EVENTS) || !defined(NDEBUG)
# include <iostream>
#endif /* NDEBUG_EVENTS */

#include <iomanip>
#include <string>

#include "Entry.hh"
namespace dcommon = des::common;

#include "UnprocessedEventsHandler.hh"
#include "QueueChannel.hh"



namespace des
{
namespace core
{


UnprocessedEventsHandler::UnprocessedEventsHandler(DesBus &p_bus, dio::Results &p_unprocessedEvents)
    : m_queue(dynamic_cast<dcommon::LadderQueue&> ((dynamic_cast<QueueChannel&> (p_bus.getChannel(id::QUEUE_CHANNEL))).getQueue())),
      m_unprocessedEvents(p_unprocessedEvents)
{
    std::stringstream s;
    s << dcommon::Entry::header();
    m_unprocessedEvents.print(s);
}


UnprocessedEventsHandler::~UnprocessedEventsHandler()
{}


void UnprocessedEventsHandler::update(PostEvent *subject)
{
#ifndef NDEBUG_EVENTS
    std::cout << "** Drop unprocessed events" << std::endl;
#endif /* NDEBUG_EVENTS */
#ifndef NDEBUG
    int num = 0;
#endif /* NDEBUG */

    dcommon::Entry *entry = subject->getEvent();

    // record the events left over
    if (entry != NULL) {
        std::stringstream s;

        do {
            s.str("");
            s << std::setprecision(14) << const_cast<const dcommon::Entry&> (*entry);
            m_unprocessedEvents.print(s);

            if (entry != NULL) {
                delete entry;
            }
#ifndef NDEBUG
            num++;
#endif /* NDEBUG */
        } while ((entry = m_queue.dequeue()) != NULL);
    } else {
        std::cout << "event was NULL" << std::endl;
    }

#ifndef NDEBUG_EVENTS
    std::cout << "Unprocessed events dropped" << std::endl;
#endif /* NDEBUG_EVENTS */
#ifndef NDEBUG
    std::cout << "Unprocessed events: " << num << std::endl;
#endif /* NDEBUG */
}


}
}
