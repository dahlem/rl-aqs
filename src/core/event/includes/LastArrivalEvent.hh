// Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This program is free software ; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation	 ; either version 2 of the License, or
// (at your option) lastArrival later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT LASTARRIVAL WARRANTY	; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program	  ; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

/** @file LastArrivalEvent.hh
 * This header file specifies the lastArrival event subject.
 */

#ifndef __LASTARRIVALEVENT_HH__
#define __LASTARRIVALEVENT_HH__

#include "Entry.hh"
namespace dcommon = des::common;

#include "Event.hh"
namespace dcore = des::core;

#include "Subject.hh"
namespace design = des::design;



namespace des
{
    namespace core
    {

/** @class LastArrivalEvent
 * The class @code{LastArrivalEvent} signals an lastArrival event in the DES.
 */
class LastArrivalEvent : public dcore::Event, public design::Subject<LastArrivalEvent>
{
public:
    LastArrivalEvent();
    ~LastArrivalEvent();

    void lastArrival(dcommon::Entry *event);
};

    }
}


#endif /* __LASTARRIVALEVENT_HH__ */
