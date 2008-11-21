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

/** @file AnyEvent.hh
 * This header file specifies the any event subject.
 */

#ifndef __ANYEVENT_HH__
#define __ANYEVENT_HH__

#include <boost/shared_ptr.hpp>

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

/** @class AnyEvent
 * The class @code{AnyEvent} signals an any event in the DES.
 */
class AnyEvent : public dcore::Event, public design::Subject<AnyEvent>
{
public:
    AnyEvent();
    ~AnyEvent();

    void any(dcommon::Entry *event);
};

/** @typedef tAnyEventSP
 * a type definition of a shared pointer to an any event
 */
typedef boost::shared_ptr <AnyEvent> tAnyEventSP;

    }
}


#endif /* __ANYEVENT_HH__ */
