// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This program is free software ; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation	 ; either version 2 of the License, or
// (at your option) post later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT POST WARRANTY	; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program	  ; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

/** @file PostEvent.hh
 * This header file specifies the post event subject.
 */

#ifndef __POSTEVENT_HH__
#define __POSTEVENT_HH__

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

/** @class PostEvent
 * The class @code{PostEvent} signals an post event in the DES.
 */
class PostEvent : public dcore::Event, public design::Subject<PostEvent>
{
public:
    PostEvent();
    ~PostEvent();

    void post(dcommon::Entry *event);
};

/** @typedef tPostEventSP
 * a type definition of a shared pointer to an post event
 */
typedef boost::shared_ptr <PostEvent> tPostEventSP;

    }
}


#endif /* __POSTEVENT_HH__ */
