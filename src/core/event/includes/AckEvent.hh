// Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This program is free software ; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation	 ; either version 2 of the License, or
// (at your option) ack later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ACK WARRANTY	; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program	  ; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

/** @file AckEvent.hh
 * This header file specifies the ack event subject.
 */

#ifndef __ACKEVENT_HH__
#define __ACKEVENT_HH__

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

/** @class AckEvent
 * The class @code{AckEvent} signals an ack event in the DES.
 */
class AckEvent : public dcore::Event, public design::Subject<AckEvent>
{
public:
    AckEvent();
    ~AckEvent();

    void ack(dcommon::Entry *event);
};

    }
}


#endif /* __ACKEVENT_HH__ */
