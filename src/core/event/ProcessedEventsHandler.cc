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

/** @file ProcessedEventHandler.cc
 * Implementation of a basic processedEvent handler.
 */
#include <iomanip>
#include <string>

#include "PreAnyEvent.hh"
#include "ProcessedEventsHandler.hh"
namespace dcore = des::core;

#include "Entry.hh"
namespace dcommon = des::common;



dcore::ProcessedEventsHandler::ProcessedEventsHandler(dio::Results &p_processedEvents)
    : m_processedEvents(p_processedEvents)
{
    std::stringstream s;
    s << dcommon::Entry::header();
    m_processedEvents.print(s);
}


dcore::ProcessedEventsHandler::~ProcessedEventsHandler()
{}


void dcore::ProcessedEventsHandler::update(dcore::PreAnyEvent *subject)
{
    std::stringstream s;
    dcommon::Entry *entry;

    entry = subject->getEvent();

    // log the event
    s.str("");
    s << std::setprecision(14) << const_cast <const dcommon::Entry&> (*entry);
    m_processedEvents.print(s);
}
