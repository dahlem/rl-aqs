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

/** @file NumEventsHandler.hh
 * This header file specifies the numEvents handler subject.
 */

#ifndef __NUMEVENTSHANDLER_HH__
#define __NUMEVENTSHANDLER_HH__


#include "Observer.hh"
namespace design = des::design;

#include "WEvonet.hh"
namespace dnet = des::network;

#include "ArrivalEvent.hh"
#include "DesBus.hh"


namespace des
{
namespace core
{


/** @class NumEventsHandler
 * The class @code{NumEventsHandler} handles numEvents events in the DES.
 */
class NumEventsHandler : public design::Observer<dcore::ArrivalEvent>
{
public:
    NumEventsHandler(DesBus&);
    ~NumEventsHandler();

    void update(ArrivalEvent *subject);

private:
    dnet::Graph &m_graph;

    // derived fields
    dnet::VertexNumEventsMap vertex_num_events_map;

};


}
}


#endif /* __NUMEVENTSHANDLER_HH__ */
