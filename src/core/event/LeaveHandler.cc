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

/** @file LeaveHandler.cc
 * Implementation of a basic leave handler.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#include "events.hh"
#include "LeaveEvent.hh"
#include "LeaveHandler.hh"
namespace dcore = des::core;

#include "Entry.hh"
#include "LadderQueue.hh"
namespace dcommon = des::common;


dcore::LeaveHandler::LeaveHandler(dcommon::Queue &p_queue)
    : m_queue(p_queue)
{
}


dcore::LeaveHandler::~LeaveHandler()
{}


void dcore::LeaveHandler::update(dcore::LeaveEvent *subject)
{
}
