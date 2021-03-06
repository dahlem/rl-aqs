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

/** @file AckHandler.hh
 * This header file specifies the ack handler subject.
 */

#ifndef __ACKHANDLER_HH__
#define __ACKHANDLER_HH__


#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "Observer.hh"
namespace design = des::design;

#include "AckEvent.hh"
#include "DesBus.hh"



namespace des
{
namespace core
{

/** @class AckHandler
 * The class @code{AckHandler} handles ack events in the DES.
 */
class AckHandler : public design::Observer<dcore::AckEvent>
{
public:
    AckHandler(DesBus&);
    ~AckHandler();

    void update(dcore::AckEvent *subject);

private:
    dnet::Graph &m_graph;
    dcommon::Queue &m_queue;
    bool m_collectiveIntelligence;

    dnet::VertexIndexMap vertex_index_map;
};


    }
}


#endif /* __ACKHANDLER_HH__ */
