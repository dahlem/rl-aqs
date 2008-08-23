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

/** @file EventProcessor.hh
 * Declaration of the event processor interface.
 */
#ifndef __EVENTPROCESSOR_HH__
#define __EVENTPROCESSOR_HH__

#include <boost/shared_ptr.hpp>

#include "WEvonet.hh"
namespace dnet = des::network;

#include "ArrivalEvent.hh"
#include "DepartureEvent.hh"
namespace dcore = des::core;

#include "LadderQueue.hh"
namespace dcommon = des::common;


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
    EventProcessor(dcommon::tQueueWP p_queue,
                   dnet::tGraphWP p_graph,
                   dcore::tArrivalEventWP p_arrivalEvent,
                   dcore::tDepartureEventWP p_departureEvent);
    ~EventProcessor();

    void process();

private:
    dcommon::tQueueWP m_queue;
    dnet::tGraphWP m_graph;
    dcore::tArrivalEventWP m_arrivalEvent;
    dcore::tDepartureEventWP m_departureEvent;

};

typedef boost::shared_ptr <dcore::EventProcessor> tEventProcessorSP;


    }
}


#endif /* __EVENTPROCESSOR_HH__ */
