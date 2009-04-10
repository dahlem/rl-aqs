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

/** @file UtilisationHandler.cc
 * Implementation of a basic utilisation handler.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NDEBUG_EVENTS
# include <iostream>
#endif /* NDEBUG_EVENTS */

#include <gsl/gsl_math.h>

#include "events.hh"
#include "UtilisationHandler.hh"
namespace dcore = des::core;

#include "Entry.hh"
namespace dcommon = des::common;

#include "WEvonet.hh"
namespace dnet = des::network;


dcore::UtilisationHandler::UtilisationHandler(dnet::Graph &p_graph)
    : m_graph(p_graph)
{
    vertex_Bdt_map = get(vertex_Bdt, m_graph);
    vertex_busy_map = get(vertex_busy, m_graph);
    vertex_utilisation_map = get(vertex_utilisation, m_graph);
    vertex_last_event_time_map = get(vertex_last_event_time, m_graph);
}


dcore::UtilisationHandler::~UtilisationHandler()
{}


void dcore::UtilisationHandler::update(dcore::PostAnyEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();

    // update the last event time
    if ((entry->getType() == LAST_ARRIVAL_EVENT) ||
        (entry->getType() == ARRIVAL_EVENT) ||
        (entry->getType() == DEPARTURE_EVENT)) {

        dnet::Vertex vertex = boost::vertex(entry->getDestination(), m_graph);
        double b_i = 0.0;

#ifndef NDEBUG_EVENTS
        std::cout << "** Update utilisation for vertex: " << entry->getDestination() << std::endl;
#endif /* NDEBUG_EVENTS */

        if (vertex_busy_map[vertex]) {
            double diff = (gsl_fcmp(entry->getArrival(), vertex_last_event_time_map[vertex], 1e-9) == 0)
                ? (0.0)
                : (entry->getArrival() - vertex_last_event_time_map[vertex]);
            b_i = diff;
        }

#ifndef NDEBUG_EVENTS
        std::cout << "b_i: " << b_i << std::endl;
#endif /* NDEBUG_EVENTS */

        // \hat(u) = \fract{\int_{0}^{T(n)}B(t)dt}{T(n)}
        // Eq. 1.6 in Simulation, Modeling and Analysis by Law, Kelton
        vertex_Bdt_map[vertex] += b_i;
        vertex_utilisation_map[vertex] = vertex_Bdt_map[vertex] / entry->getArrival();
    }
}
