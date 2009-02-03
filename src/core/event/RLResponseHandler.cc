// Copyright (C) 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file RLResponseHandler.cc
 * Implementation of a basic RLResponse handler.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NDEBUG_EVENTS
# include <iostream>
#endif /* NDEBUG_EVENTS */

#include "Entry.hh"
namespace dcommon = des::common;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "Stats.hh"
namespace dstats = des::statistics;

#include "events.hh"
#include "AckEvent.hh"
#include "RLResponseHandler.hh"


namespace des
{
namespace core
{


RLResponseHandler::RLResponseHandler(dnet::tGraphSP p_graph, boost::shared_array<double> p_alpha,
                                     boost::shared_array<double> p_r, boost::uint16_t p_levels)
    : m_graph(p_graph), m_alpha(p_alpha), m_r(p_r), m_levels(p_levels)
{
    vertex_var_response_map = get(vertex_var_response, *m_graph);
    vertex_mean_response_map = get(vertex_mean_response, *m_graph);
    vertex_num_events_processed_map = get(vertex_num_events_processed, *m_graph);
}


RLResponseHandler::~RLResponseHandler()
{}


void RLResponseHandler::update(AckEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();

#ifndef NDEBUG_EVENTS
    std::cout << "** RLResponse for vertex: " << entry->getDestination() << std::endl;
    std::cout << "Event: " << const_cast <const dcommon::Entry&> (*entry) << std::endl;
#endif /* NDEBUG_EVENTS */

    dnet::Vertex vertex = boost::vertex(entry->getDestination(), *m_graph);
    double size = vertex_num_events_processed_map[vertex];

    // only learn, if we have a properly sample size
    if (size > 1) {
        double xbar = vertex_mean_response_map[vertex];
        double var = vertex_var_response_map[vertex] / (size - 1);
        double x = entry->getArrival() - entry->topArrival();
        bool done = false;
        double reward = 0.0;
        
        // check below mean
        for (int i = (m_levels - 1); i >= 0; --i) {
            if (dstats::Stats::isBelow(x, xbar, var, size, m_alpha[i])) {
                reward = - m_r[i];
                done = true;
                break;
            }
        }

        if (!done) {
            // check below mean
            for (int i = (m_levels - 1); i >= 0; --i) {
                if (dstats::Stats::isAbove(x, xbar, var, size, m_alpha[i])) {
                    reward = m_r[i];
                    break;
                }
            }
        }
#ifndef NDEBUG_EVENTS
        std::cout << "Reward: " << reward << std::endl;
    } else {
        std::cout << "Sample size too small: " << size << std::endl;
#endif /* NDEBUG_EVENTS */
    }
}


}
}
