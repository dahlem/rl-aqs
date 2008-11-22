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

/** @file LastEventHandler.hh
 * This header file specifies the lastEvent handler subject.
 */

#ifndef __LASTEVENTHANDLER_HH__
#define __LASTEVENTHANDLER_HH__

#include <boost/shared_ptr.hpp>


#include "PostAnyEvent.hh"
namespace dcore = des::core;

#include "Observer.hh"
namespace design = des::design;

#include "WEvonet.hh"
namespace dnet = des::network;


namespace des
{
    namespace core
    {


/** @class LastEventHandler
 * The class @code{LastEventHandler} handles lastEvent events in the DES.
 */
class LastEventHandler : public design::Observer<dcore::PostAnyEvent>
{
public:
    LastEventHandler(dnet::tGraphSP p_graph);
    ~LastEventHandler();

    void update(dcore::PostAnyEvent *subject);

private:
    dnet::tGraphSP m_graph;

    // derived fields
    dnet::VertexLastEventTimeMap vertex_last_event_time_map;

};


/** @typedef tLastEventHandlerSP
 * a type definition of the shared pointer of the lastEvent handler
 */
typedef boost::shared_ptr <LastEventHandler> tLastEventHandlerSP;


    }
}


#endif /* __LASTEVENTHANDLER_HH__ */
