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

/** @file ExpectedAverageEventInQueueHandler.hh
 * This header file specifies the expectedAverageEventInQueue handler subject.
 */

#ifndef __EXPECTEDAVERAGEEVENTINQUEUEHANDLER_HH__
#define __EXPECTEDAVERAGEEVENTINQUEUEHANDLER_HH__


#include "Observer.hh"
namespace design = des::design;

#include "WEvonet.hh"
namespace dnet = des::network;

#include "PostAnyEvent.hh"
#include "DesBus.hh"


namespace des
{
namespace core
{


/** @class ExpectedAverageEventInQueueHandler
 * The class @code{ExpectedAverageEventInQueueHandler} handles expectedAverageEventInQueue events in the DES.
 */
class ExpectedAverageEventInQueueHandler : public design::Observer<dcore::PostAnyEvent>
{
public:
    ExpectedAverageEventInQueueHandler(DesBus&);
    ~ExpectedAverageEventInQueueHandler();

    void update(dcore::PostAnyEvent *subject);

private:
    dnet::Graph &m_graph;

    // derived fields
    dnet::VertexQdtMap vertex_Qdt_map;
    dnet::VertexNumberInQueueMap vertex_number_in_queue_map;
    dnet::VertexExpectedAverageNumberEventMap vertex_expected_average_number_event_map;
    dnet::VertexLastEventTimeMap vertex_last_event_time_map;

};



    }
}


#endif /* __EXPECTEDAVERAGEEVENTINQUEUEHANDLER_HH__ */
