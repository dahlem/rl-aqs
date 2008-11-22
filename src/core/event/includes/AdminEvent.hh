// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This program is free software ; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation	 ; either version 2 of the License, or
// (at your option) admin later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ADMIN WARRANTY	; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program	  ; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

/** @file AdminEvent.hh
 * This header file specifies the admin event subject.
 */

#ifndef __ADMINEVENT_HH__
#define __ADMINEVENT_HH__

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

/** @class AdminEvent
 * The class @code{AdminEvent} signals an admin event in the DES.
 */
class AdminEvent : public dcore::Event, public design::Subject<AdminEvent>
{
public:
    AdminEvent();
    ~AdminEvent();

    void admin(dcommon::Entry *event);
};

/** @typedef tAdminEventSP
 * a type definition of a shared pointer to an admin event
 */
typedef boost::shared_ptr <AdminEvent> tAdminEventSP;

    }
}


#endif /* __ADMINEVENT_HH__ */
