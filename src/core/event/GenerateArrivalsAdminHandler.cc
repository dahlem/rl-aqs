// Copyright (C) 2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file GenerateArrivalsAdminHandler.cc
 * Implementation of a handler to generate external arrival events.
 */
#include <boost/graph/adjacency_list.hpp>

#include "Entry.hh"
namespace dcommon = des::common;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "events.hh"
#include "GenerateArrivalsAdminHandler.hh"
#include "EventGenerator.hh"



namespace des
{
namespace core
{


GenerateArrivalsAdminHandler::GenerateArrivalsAdminHandler(
    dnet::Graph &p_graph,
    Int32SA p_arrivalRngs,
    dcommon::Queue &p_queue)
    : m_graph(p_graph), m_arrivalRngs(p_arrivalRngs), m_queue(p_queue)
{
#ifndef NDEBUG_SAMPLING
    int vertices = 0;

    std::cout << "Arrival seeds: ";
    for (int i = 0; i < vertices; ++i) {
        std::cout << m_arrivalRngs[i] << ", ";
    }
    std::cout << std::endl << std::cout.flush();
#endif /* NDEBUG_SAMPLING */
}


GenerateArrivalsAdminHandler::~GenerateArrivalsAdminHandler()
{}


void GenerateArrivalsAdminHandler::update(AdminEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();

    // update the last event time
    if (entry->getType() == GENERATE_ARRIVAL_EVENT) {

        int dest = entry->getDestination();

        dnet::Vertex vertex = boost::vertex(dest, m_graph);
        dnet::VertexArrivalRateMap vertex_arrival_props_map =
            get(vertex_arrival_rate, m_graph);
    
        // generate a single event
        double arrival_rate = vertex_arrival_props_map[vertex];
        double startTime = entry->getArrival();

        // generate the events
        dsample::tGslRngSP arrival_rng = dsample::CRN::getInstance().get(m_arrivalRngs[dest]);
        EventGenerator::generateArrival(m_queue, arrival_rng, dest, startTime, arrival_rate);
    }
}




}
}
