// Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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


#include "DepartureEvent.hh"
namespace dcore = des::core;

#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "Observer.hh"
namespace design = des::design;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "Selection.hh"
namespace drl = des::rl;


namespace des
{
namespace core
{


/** @class DepartureHandler
 * The class @code{DepartureHandler} handles departure events in the DES.
 */
class DepartureHandler : public design::Observer<dcore::DepartureEvent>
{
public:
    DepartureHandler(dcommon::Queue &p_queue, dnet::Graph &p_graph, drl::Selection &p_selection);
    ~DepartureHandler();

    void update(dcore::DepartureEvent *subject);

private:
    dcommon::Queue &m_queue;
    dnet::Graph &m_graph;
    drl::Selection &m_selection;

    dnet::VertexBusyMap vertex_busy_map;
    dnet::VertexNumberInQueueMap vertex_number_in_queue_map;
};


}
}


#endif /* __DEPARTUREHANDLER_HH__ */
