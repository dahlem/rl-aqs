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

/** @file UtilisationHandler.hh
 * This header file specifies the utilisation handler subject.
 */

#ifndef __UTILISATIONHANDLER_HH__
#define __UTILISATIONHANDLER_HH__


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


/** @class UtilisationHandler
 * The class @code{UtilisationHandler} handles utilisation events in the DES.
 */
class UtilisationHandler : public design::Observer<PostAnyEvent>
{
public:
    UtilisationHandler(DesBus&);
    ~UtilisationHandler();

    void update(PostAnyEvent *subject);

private:
    dnet::Graph &m_graph;

    // derived fields
    dnet::VertexBdtMap vertex_Bdt_map;
    dnet::VertexUtilisationMap vertex_utilisation_map;
    dnet::VertexLastEventTimeMap vertex_last_event_time_map;
    dnet::VertexNumberInQueueMap vertex_number_in_queue_map;

};


    }
}


#endif /* __UTILISATIONHANDLER_HH__ */
