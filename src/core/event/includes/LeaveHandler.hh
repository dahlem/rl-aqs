// Copyright (C) 2008-2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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

/** @file LeaveHandler.hh
 * This header file specifies the leave handler subject.
 */

#ifndef __LEAVEHANDLER_HH__
#define __LEAVEHANDLER_HH__

#include <boost/scoped_array.hpp>

#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "Observer.hh"
namespace design = des::design;

#include "OnlineStats.hh"
namespace dstats = des::statistics;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "LeaveEvent.hh"
#include "DesBus.hh"


namespace des
{
namespace core
{


typedef boost::scoped_array<dstats::OnlineStats> tOnlineStatsSA;

/** @class LeaveHandler
 * The class @code{LeaveHandler} handles leave events in the DES.
 */
class LeaveHandler : public design::Observer<dcore::LeaveEvent>
{
public:
    explicit LeaveHandler(DesBus&);
    ~LeaveHandler();

    void update(dcore::LeaveEvent *subject);

private:
    dnet::Graph &m_graph;
    dcommon::Queue &m_queue;
    tOnlineStatsSA m_EventInSystem;

    dnet::VertexAvgEventInSystemTimeMap m_vertexAvgEventInSystemTimeMap;
};



}
}


#endif /* __LEAVEHANDLER_HH__ */
