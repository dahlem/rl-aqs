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

/** @file GenerateArrivalsHandler.cc
 * Implementation of a generate arrivals handler.
 */
#include <boost/graph/adjacency_list.hpp>
#include <boost/iterator/filter_iterator.hpp>

#include "Entry.hh"
namespace dcommon = des::common;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "GenerateArrivalsHandler.hh"
#include "EventGenerator.hh"
#include "GraphChannel.hh"
#include "QueueChannel.hh"



namespace des
{
namespace core
{


GenerateArrivalsHandler::GenerateArrivalsHandler(
    DesBus &p_bus, Int32SA p_arrivalRngs)
    : m_graph((dynamic_cast<GraphChannel&> (p_bus.getChannel(id::GRAPH_CHANNEL))).getGraph()),
      m_queue((dynamic_cast<QueueChannel&> (p_bus.getChannel(id::QUEUE_CHANNEL))).getQueue()),
      m_arrivalRngs(p_arrivalRngs)
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


GenerateArrivalsHandler::~GenerateArrivalsHandler()
{}


void GenerateArrivalsHandler::update(LastArrivalEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();

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
