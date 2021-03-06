// Copyright (C) 2009-2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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

/** @file NullUnprocessedEventHandler.cc
 * Implementation of a basic null unprocessed event handler.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#if !defined(NDEBUG_EVENTS) || !defined(NDEBUG)
# include <iostream>
#endif /* NDEBUG_EVENTS */

#include "Entry.hh"
namespace dcommon = des::common;

#include "NullUnprocessedEventsHandler.hh"
#include "QueueChannel.hh"



namespace des
{
namespace core
{


NullUnprocessedEventsHandler::NullUnprocessedEventsHandler(DesBus &p_bus)
    : m_queue(dynamic_cast<dcommon::LadderQueue&> ((dynamic_cast<QueueChannel&> (p_bus.getChannel(id::QUEUE_CHANNEL))).getQueue()))
{}


NullUnprocessedEventsHandler::~NullUnprocessedEventsHandler()
{}


void NullUnprocessedEventsHandler::update(PostEvent *subject)
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
        do {
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
    std::cout << "NullUnprocessed events dropped" << std::endl;
#endif /* NDEBUG_EVENTS */
#ifndef NDEBUG
    std::cout << "NullUnprocessed events: " << num << std::endl;
#endif /* NDEBUG */
}


}
}
