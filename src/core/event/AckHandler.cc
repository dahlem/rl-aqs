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

/** @file AckHandler.cc
 * Implementation of a basic ack handler.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#include "events.hh"
#include "AckEvent.hh"
#include "AckHandler.hh"
namespace dcore = des::core;

#include "Entry.hh"
#include "LadderQueue.hh"
namespace dcommon = des::common;


dcore::AckHandler::AckHandler(dcommon::tQueueSP p_queue)
    : m_queue(p_queue)
{
}


dcore::AckHandler::~AckHandler()
{}


void dcore::AckHandler::update(dcore::AckEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();

    if (entry->isEventQueueEmpty()) {
        // schedule leave event
        dcommon::Entry *new_entry = new dcommon::Entry(
            const_cast <const dcommon::Entry&> (*entry));

        new_entry->leave(dcore::EXTERNAL_EVENT, dcore::LEAVE_EVENT);
        m_queue->push(new_entry);
    } else {
        // schedule ack events
        boost::int32_t origin = entry->getDestination();
        boost::int32_t destination = entry->popEvent();

        dcommon::Entry *new_entry = new dcommon::Entry(
            const_cast <const dcommon::Entry&> (*entry));

        new_entry->acknowledge(origin, destination, dcore::ACK_EVENT);
        m_queue->push(new_entry);
    }
}
