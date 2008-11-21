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

/** @file UnprocessedEventHandler.hh
 * This header file specifies the unprocessedEvent handler subject.
 */

#ifndef __UNPROCESSEDEVENTHANDLER_HH__
#define __UNPROCESSEDEVENTHANDLER_HH__

#include <boost/shared_ptr.hpp>


#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "PostEvent.hh"
namespace dcore = des::core;

#include "Observer.hh"
namespace design = des::design;

#include "Results.hh"
namespace dio = des::io;


namespace des
{
    namespace core
    {


/** @class UnprocessedEventHandler
 * The class @code{UnprocessedEventHandler} handles unprocessedEvent events in the DES.
 */
class UnprocessedEventsHandler : public design::Observer<dcore::PostEvent>
{
public:
    UnprocessedEventsHandler(dio::tResultsSP, dcommon::tQueueSP);
    ~UnprocessedEventsHandler();

    void update(dcore::PostEvent *subject);

private:
    dio::tResultsSP m_unprocessedEvents;
    dcommon::tQueueSP m_queue;

};


/** @typedef tUnprocessedEventHandlerSP
 * a type definition of the shared pointer of the unprocessedEvent handler
 */
typedef boost::shared_ptr <UnprocessedEventsHandler> tUnprocessedEventsHandlerSP;


    }
}


#endif /* __UNPROCESSEDEVENTHANDLER_HH__ */
