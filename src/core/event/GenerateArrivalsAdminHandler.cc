// Copyright (C) 2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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
#include <boost/foreach.hpp>

#include "Entry.hh"
namespace dcommon = des::common;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "events.hh"
#include "EventGenerator.hh"
#include "GenerateArrivalsAdminHandler.hh"
#include "GraphChannel.hh"
#include "QueueChannel.hh"



namespace des
{
namespace core
{


GenerateArrivalsAdminHandler::GenerateArrivalsAdminHandler(
    DesBus& p_bus, Int32SA p_arrivalRngs)
    : m_arrivalRngs(p_arrivalRngs),
      m_graph((dynamic_cast<GraphChannel&> (p_bus.getChannel(id::GRAPH_CHANNEL))).getGraph()),
      m_queue((dynamic_cast<QueueChannel&> (p_bus.getChannel(id::QUEUE_CHANNEL))).getQueue())
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
#ifndef NDEBUG_EVENTS
    std::cout << "** GenerateArrivalsAdminHandler" << std::endl;
#endif /* NDEBUG */

    dcommon::Entry *entry = subject->getEvent();

    // update the last event time
    if (entry->getType() == GENERATE_ARRIVAL_EVENT) {
        // generate arrivals for each vertex
        boost::int32_t destination = 0;
        double startTime = entry->getArrival();

        dnet::VertexArrivalRateMap vertex_arrival_props_map =
            get(vertex_arrival_rate, m_graph);
        dnet::VertexIndexMap vertex_index_props_map =
            get(boost::vertex_index, m_graph);

#ifndef NDEBUG_EVENTS
        std::cout << "generate arrivals for" << std::endl;
#endif /* NDEBUG */

        BOOST_FOREACH(dnet::Vertex v, boost::vertices(m_graph)) {
            destination = vertex_index_props_map[v];

#ifndef NDEBUG_EVENTS
            std::cout << "... for vertex " << destination << std::endl;
#endif /* NDEBUG */

            // generate a single event
            double arrival_rate = vertex_arrival_props_map[v];

            // generate the events
            dsample::tGslRngSP arrival_rng = dsample::CRN::getInstance().get(m_arrivalRngs[destination]);
            EventGenerator::generateArrival(m_queue, arrival_rng, destination, startTime, arrival_rate);
        }
    }
}




}
}
