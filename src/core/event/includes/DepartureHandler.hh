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

/** @file DepartureHandler.hh
 * This header file specifies the departure handler subject.
 */

#ifndef __DEPARTUREHANDLER_HH__
#define __DEPARTUREHANDLER_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>


#include "DepartureEvent.hh"
namespace dcore = des::core;

#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "Observer.hh"
namespace design = des::design;

#include "WEvonet.hh"
namespace dnet = des::network;

#include "CRN.hh"
namespace dsample = des::sampling;


namespace des
{
namespace core
{

typedef boost::shared_array <boost::int32_t> Int32SA;


/** @class DepartureHandler
 * The class @code{DepartureHandler} handles departure events in the DES.
 */
class DepartureHandler : public design::Observer<dcore::DepartureEvent>
{
public:
    DepartureHandler(dcommon::tQueueSP p_queue, dnet::tGraphSP p_graph, Int32SA p_depart_uniform_ids);
    ~DepartureHandler();

    void update(dcore::DepartureEvent *subject);

private:
    dcommon::tQueueSP m_queue;
    dnet::tGraphSP m_graph;
    Int32SA m_depart_uniform_ids;
    dnet::VertexBusyMap vertex_busy_map;
    dnet::VertexNumberInQueueMap vertex_number_in_queue_map;
    dnet::VertexIndexMap vertex_index_map;
    dnet::EdgeWeightMap edge_weight_map;
};


/** @typedef tDepartureHandlerSP
 * a type definition of the shared pointer of the departure handler
 */
typedef boost::shared_ptr <DepartureHandler> tDepartureHandlerSP;


}
}


#endif /* __DEPARTUREHANDLER_HH__ */
