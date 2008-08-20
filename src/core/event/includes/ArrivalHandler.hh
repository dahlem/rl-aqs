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

/** @file ArrivalHandler.hh
 * This header file specifies the arrival handler subject.
 */

#ifndef __ARRIVALHANDLER_HH__
#define __ARRIVALHANDLER_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>


#include "ArrivalEvent.hh"
namespace dcore = des::core;

#include "Observer.hh"
namespace design = des::design;

#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "WEvonet.hh"
namespace dnet = des::network;

#include "CRN.hh"
namespace dsample = des::sampling;


namespace des
{
    namespace core
    {

/** @typedef tQueueSP
 * a type definition of the shared pointer of the @ref{LadderQueue}
 */
typedef boost::shared_ptr <dcommon::LadderQueue> tQueueSP;


/** @class ArrivalHandler
 * The class @code{ArrivalHandler} handles arrival events in the DES.
 */
class ArrivalHandler : public design::Observer<dcore::ArrivalEvent>
{
public:
    ArrivalHandler(dnet::tGraphSP p_graph, tQueueSP p_queue, uint32_t p_service_idx);
    ~ArrivalHandler();

    void update(dcore::ArrivalEvent *subject);

private:
    dnet::tGraphSP m_graph;
    tQueueSP m_queue;
    uint32_t m_service_idx;
    dsample::tGslRngSP m_service_rng;
    dnet::VertexBusyMap vertex_busy_map;
    dnet::VertexTimeServiceEndsMap vertex_time_service_ends_map;
    dnet::VertexServiceRateMap vertex_service_map;
    dnet::VertexNumberInQueueMap vertex_number_in_queue_map;

};


/** @typedef tArrivalHandlerSP
 * a type definition of the shared pointer of the arrival handler
 */
typedef boost::shared_ptr <ArrivalHandler> tArrivalHandlerSP;


    }
}


#endif /* __ARRIVALHANDLER_HH__ */
