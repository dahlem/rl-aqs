// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

/** @file Report.cc
 * Implementation of the report interface
 *
 * @author Dominik Dahlem
 */
#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <numeric>

#include <boost/cstdint.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/property_iter_range.hpp>

#include "Report.hh"

#include "WEvonet.hh"
namespace dnet = des::network;


namespace des
{
namespace core
{

void Report::accumResults(dnet::tGraphSP p_graph, sim_output *output)
{
    output->system_average_delay
        = meanDelay(p_graph);
    output->system_expected_average_num_in_queue
        = meanExpectedAverageNumberEvents(p_graph);
}

double Report::meanExpectedAverageNumberEvents(dnet::tGraphSP p_graph)
{
    dnet::VExpectedAverageNumEventsIterator it, it_end;
    boost::uint32_t size = boost::num_vertices(*p_graph);
    double result = 0.0;

    tie(it, it_end) = boost::get_property_iter_range(
        *p_graph, vertex_expected_average_number_event);

    result = std::accumulate(it, it_end, 0.0);

    return result / static_cast<double> (size);
}

double Report::meanDelay(dnet::tGraphSP p_graph)
{
    dnet::VAverageDelayQueueIterator it, it_end;
    boost::uint32_t size = boost::num_vertices(*p_graph);
    double result = 0.0;

    tie(it, it_end) = boost::get_property_iter_range(
        *p_graph, vertex_average_delay_in_queue);

    result = std::accumulate(it, it_end, 0.0);

    return result / static_cast<double> (size);
}


}
}
