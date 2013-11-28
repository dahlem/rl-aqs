// Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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

/** @file DepartureEvent.hh
 * This header file specifies the departure event subject.
 */

#ifndef __DEPARTUREEVENT_HH__
#define __DEPARTUREEVENT_HH__

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

/** @class DepartureEvent
 * The class @code{DepartureEvent} signals an departure event in the DES.
 */
class DepartureEvent : public dcore::Event, public design::Subject<DepartureEvent>
{
public:
    DepartureEvent();
    ~DepartureEvent();

    void departure(dcommon::Entry *event);
};

    }
}


#endif /* __DEPARTUREEVENT_HH__ */
