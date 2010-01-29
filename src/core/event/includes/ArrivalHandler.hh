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

/** @file ArrivalHandler.hh
 * This header file specifies the arrival handler subject.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __ARRIVALHANDLER_HH__
#define __ARRIVALHANDLER_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>
#include <boost/shared_array.hpp>


#include "Observer.hh"
namespace design = des::design;

#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "WEvonet.hh"
namespace dnet = des::network;

#include "CRN.hh"
namespace dsample = des::sampling;

#include "ArrivalEvent.hh"
#include "DesBus.hh"



namespace des
{
namespace core
{

typedef boost::shared_array <boost::int32_t> Int32SA;

/** @class ArrivalHandler
 * The class @code{ArrivalHandler} handles arrival events in the DES.
 */
class ArrivalHandler : public design::Observer<ArrivalEvent>
{
public:
    ArrivalHandler(DesBus&,  Int32SA p_service_ids);
    ~ArrivalHandler();

    void update(ArrivalEvent *subject);

private:
    dcommon::Queue &m_queue;
    dnet::Graph &m_graph;
    Int32SA m_service_ids;

    dnet::VertexBusyMap vertex_busy_map;
    dnet::VertexTimeServiceEndsMap vertex_time_service_ends_map;
    dnet::VertexServiceRateMap vertex_service_map;
    dnet::VertexNumberInQueueMap vertex_number_in_queue_map;
    dnet::VertexAverageDelayInQueueMap vertex_average_delay_in_queue_map;
    dnet::VertexNumEventsMap vertex_num_events_map;

};


/** @typedef tArrivalHandlerSP
 * a type definition of the shared pointer of the arrival handler
 */
typedef boost::shared_ptr <ArrivalHandler> tArrivalHandlerSP;


}
}


#endif /* __ARRIVALHANDLER_HH__ */
