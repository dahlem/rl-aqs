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

/** @file DepartureHandler.cc
 * Implementation of a basic departure handler.
 */
#include <iostream>
#include <gsl/gsl_randist.h>


#include "events.hh"
#include "DepartureEvent.hh"
#include "DepartureHandler.hh"
namespace dcore = des::core;

#include "Entry.hh"
namespace dcommon = des::common;

#include "WEvonet.hh"
namespace dnet = des::network;

#include "CRN.hh"
namespace dsample = des::sampling;


dcore::DepartureHandler::DepartureHandler(
    dnet::tGraphSP p_graph, tQueueSP p_queue)
    : m_graph(p_graph), m_queue(p_queue)
{
}


dcore::DepartureHandler::~DepartureHandler()
{}


void dcore::DepartureHandler::update(dcore::DepartureEvent *subject)
{
    dnet::VertexBusyMap vertex_busy_map = get(vertex_busy, *m_graph);
    dnet::VertexNumberInQueueMap vertex_number_in_queue_map =
        get(vertex_number_in_queue, *m_graph);

    dcommon::entry_t *entry;
    entry = subject->getEvent();

    dnet::Vertex vertex = boost::vertex(entry->destination, *m_graph);
    // if the server is busy then re-schedule
    // otherwise schedule the departure
    if (vertex_busy_map[vertex]) {
        vertex_number_in_queue_map[vertex]--;

        if (vertex_number_in_queue_map[vertex] == 0) {
            vertex_busy_map[vertex] = false;
        }
    } else {
        std::cout << "departure not busy!!!" << std::endl;
    }
}
