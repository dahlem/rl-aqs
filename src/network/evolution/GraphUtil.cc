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

/** @file WEvonet.cc
 * Implementation of the methods declared in WEvonet.hh.
 *
 * @author Dominik Dahlem
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <fstream>
#include <string>

#include <boost/graph/adjacency_list.hpp>

#include "GraphException.hh"
#include "GraphUtil.hh"


namespace des
{
namespace network
{


void GraphUtil::print(tGraphSP p_graph, const std::string& filename,
                            const GraphTypes graphType)
{
    switch (graphType) {
      case GRAPHVIZ:
          print_dot(p_graph, filename);
          break;
      case GRAPHML:
          print_graphml(p_graph, filename);
          break;
      default:
          print_dot(p_graph, filename);
          break;
    }
}


void GraphUtil::print_dot(tGraphSP p_graph, const std::string& filename)
{
    std::ofstream out(filename.c_str(), std::ios::out);

    if (out.is_open()) {
        boost::dynamic_properties dp = getProperties(p_graph);
        std::string node_id = "node_id";
        boost::write_graphviz(out, *p_graph, dp, node_id);
        out.close();
    }
}


void GraphUtil::print_graphml(tGraphSP p_graph, const std::string& filename)
{
    std::ofstream out(filename.c_str(), std::ios::out);

    if (out.is_open()) {
        boost::dynamic_properties dp = getProperties(p_graph);

        boost::write_graphml(out, *p_graph, dp, true);

        out.close();
    }
}


void GraphUtil::read(tGraphSP p_graph, const std::string& p_filename,
                           const GraphTypes p_graphType)
    throw (GraphException)
{
    switch (p_graphType) {
      case GRAPHVIZ:
          read_dot(p_graph, p_filename);
          break;
      case GRAPHML:
          read_graphml(p_graph, p_filename);
          break;
      default:
          read_graphml(p_graph, p_filename);
          break;
    }
}


void GraphUtil::read_graphml(tGraphSP p_graph, const std::string& p_filename)
    throw (GraphException)
{
    std::ifstream in(p_filename.c_str(), std::ifstream::in);

    if (in.is_open()) {
        boost::dynamic_properties dp = getProperties(p_graph);

        try {
            boost::read_graphml(in, (*p_graph.get()), dp);
        } catch (...) {
            throw GraphException(GraphException::GRAPH_READ_ERROR);
        }

        in.close();
    } else {
        throw GraphException(GraphException::GRAPH_INPUT_ERROR);
    }
}


void GraphUtil::read_dot(tGraphSP p_graph, const std::string& p_filename)
    throw (GraphException)
{
    std::ifstream in(p_filename.c_str(), std::ifstream::in);

    if (in.is_open()) {
        boost::dynamic_properties dp = getProperties(p_graph);

        try {
            boost::read_graphviz(in, (*p_graph.get()), dp);
        } catch (...) {
            throw GraphException(GraphException::GRAPH_READ_ERROR);
        }

        in.close();
    } else {
        throw GraphException(GraphException::GRAPH_INPUT_ERROR);
    }
}


boost::dynamic_properties GraphUtil::getProperties(tGraphSP p_graph)
{
    boost::dynamic_properties dp;
    dp.property(EDGE_WEIGHT, get(boost::edge_weight, *p_graph));
    dp.property(VERTEX_ID, get(boost::vertex_index, *p_graph));
    dp.property(SERVICE_RATE, get(vertex_service_rate, *p_graph));
    dp.property(ARRIVAL_RATE, get(vertex_arrival_rate, *p_graph));
    dp.property(BUSY, get(vertex_busy, *p_graph));
    dp.property(TIME_SERVICE_ENDS, get(vertex_time_service_ends, *p_graph));
    dp.property(NUMBER_IN_QUEUE, get(vertex_number_in_queue, *p_graph));
    dp.property(AVERAGE_DELAY_IN_QUEUE, get(vertex_average_delay_in_queue, *p_graph));
    dp.property(NUM_EVENTS, get(vertex_num_events, *p_graph));
    dp.property(UTILISATION, get(vertex_utilisation, *p_graph));
    dp.property(BDT, get(vertex_Bdt, *p_graph));
    dp.property(QDT, get(vertex_Qdt, *p_graph));
    dp.property(LAST_EVENT_TIME, get(vertex_last_event_time, *p_graph));
    dp.property(EXPECTED_AVERAGE_NUMBER_EVENT, get(vertex_expected_average_number_event, *p_graph));

    return dp;
}


}
}
