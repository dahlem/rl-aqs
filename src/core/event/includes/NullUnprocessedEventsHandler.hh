// Copyright (C) 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file NullUnprocessedEventHandler.hh
 * This header file specifies the unprocessedEvent handler subject.
 */

#ifndef __NULLUNPROCESSEDEVENTHANDLER_HH__
#define __NULLUNPROCESSEDEVENTHANDLER_HH__


#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "PostEvent.hh"
namespace dcore = des::core;

#include "Observer.hh"
namespace design = des::design;


namespace des
{
    namespace core
    {


/** @class NullUnprocessedEventHandler
 * The class @code{NullUnprocessedEventHandler} handles unprocessedEvent events in the DES.
 */
class NullUnprocessedEventsHandler : public design::Observer<dcore::PostEvent>
{
public:
    NullUnprocessedEventsHandler(dcommon::LadderQueue&);
    ~NullUnprocessedEventsHandler();

    void update(dcore::PostEvent *subject);

private:
    dcommon::LadderQueue &m_queue;

};



    }
}


#endif /* __NULLUNPROCESSEDEVENTHANDLER_HH__ */
