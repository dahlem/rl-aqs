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

/** @file Event.hh
 * This header file specifies the abstract event class.
 */

#ifndef __EVENT_HH__
#define __EVENT_HH__

#include "Entry.hh"
namespace dcommon = des::common;


namespace des
{
    namespace core
    {

/** @class Event
 * The class @code{Event} is a base class for all events in DES.
 *
 * @author Dominik Dahlem
 */
class Event
{
public:
    /**
     * Declare the destructor virtual to make this an abstract class.
     */
    virtual ~Event() = 0;

    /** @fn getEvent()
     * retrieve the current processed event.
     *
     * @return the currently processed event.
     */
    dcommon::tEntrySP getEvent();

protected:
    dcommon::tEntrySP m_event;

};


    }
}


#endif /* __EVENT_HH__ */
