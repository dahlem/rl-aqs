// Copyright (C) 2008-2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file EventProcessor.hh
 * Declaration of the event processor interface.
 */
#ifndef __EVENTPROCESSOR_HH__
#define __EVENTPROCESSOR_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "AckEvent.hh"
#include "AdminEvent.hh"
#include "ArrivalEvent.hh"
#include "DepartureEvent.hh"
#include "LastArrivalEvent.hh"
#include "LeaveEvent.hh"
#include "PostAnyEvent.hh"
#include "PostEvent.hh"
#include "PreAnyEvent.hh"
#include "DesBus.hh"


namespace des
{
    namespace core
    {

/** @class EventProcessor
 * This class processes the events in the event queue
 */
class EventProcessor
{
public:
    EventProcessor(DesBus&,
                   AdminEvent&,
                   PreAnyEvent&,
                   PostAnyEvent&,
                   ArrivalEvent&,
                   DepartureEvent&,
                   PostEvent&,
                   LastArrivalEvent&,
                   AckEvent&,
                   LeaveEvent&);
    ~EventProcessor();

    bool process();


private:
    dcommon::LadderQueue &m_queue;
    AdminEvent &m_adminEvent;
    PreAnyEvent &m_preAnyEvent;
    PostAnyEvent &m_postAnyEvent;
    ArrivalEvent &m_arrivalEvent;
    DepartureEvent &m_departureEvent;
    PostEvent &m_postEvent;
    LastArrivalEvent &m_lastArrivalEvent;
    AckEvent &m_ackEvent;
    LeaveEvent &m_leaveEvent;

    double m_stopTime;
    double m_oldTime;

};


    }
}


#endif /* __EVENTPROCESSOR_HH__ */
