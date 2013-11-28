// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This program is free software ; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation	 ; either version 2 of the License, or
// (at your option) preAny later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT PREANY WARRANTY	; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program	  ; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

/** @file PreAnyEvent.cc
 * Implementation of a basic preAny event subject.
 */

#include "PreAnyEvent.hh"
namespace dcore = des::core;

#include "Entry.hh"
namespace dcommon = des::common;


dcore::PreAnyEvent::PreAnyEvent()
{}


dcore::PreAnyEvent::~PreAnyEvent()
{}


void dcore::PreAnyEvent::preAny(dcommon::Entry *event)
{
    m_event = event;
    notify();
}
