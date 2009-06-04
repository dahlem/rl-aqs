// Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <fstream>
#include <string>

#include <boost/cstdint.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "GraphException.hh"
#include "GraphUtil.hh"


namespace des
{
namespace network
{


void GraphUtil::print(Graph &p_graph, const std::string& filename,
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


void GraphUtil::print_dot(Graph &p_graph, const std::string& filename)
{
    std::ofstream out(filename.c_str(), std::ios::out);

    if (out.is_open()) {
        boost::dynamic_properties dp = getProperties(p_graph);
        std::string node_id = "id";
        boost::write_graphviz(out, p_graph, dp, node_id);
        out.close();
    }
}


void GraphUtil::print_graphml(Graph &p_graph, const std::string& filename)
{
    std::ofstream out(filename.c_str(), std::ios::out);

    if (out.is_open()) {
        boost::dynamic_properties dp = getProperties(p_graph);
        boost::write_graphml(out, p_graph, get(boost::vertex_index, p_graph), dp, false);
        out.close();
    }
}


void GraphUtil::read(Graph &p_graph, const std::string& p_filename,
                     const GraphTypes p_graphType)
    throw (GraphException)
{
    switch (p_graphType) {
      case GRAPHML:
          read_graphml(p_graph, p_filename);
          break;
      case GRAPHVIZ:
          read_dot(p_graph, p_filename);
          break;
      default:
          read_graphml(p_graph, p_filename);
          break;
    }
}


void GraphUtil::read_graphml(Graph &p_graph, const std::string& p_filename)
    throw (GraphException)
{
#ifndef NDEBUG
    std::cout << "Read graphml file." << std::endl;
#endif /* NDEBUG */
    std::ifstream in(p_filename.c_str(), std::ifstream::in);

    if (in.is_open()) {
        boost::dynamic_properties dp = getProperties(p_graph);

        try {
            boost::read_graphml(in, p_graph, dp);
#ifndef NDEBUG
            std::cout << "Successfully read graph." << std::endl;
            std::cout << "Vertices: " << boost::num_vertices(p_graph) << std::endl;
#endif /* NDEBUG */
        } catch (...) {
            throw GraphException(GraphException::GRAPH_READ_ERROR);
        }

        in.close();
    } else {
        throw GraphException(GraphException::GRAPH_INPUT_ERROR);
    }
}


void GraphUtil::read_dot(Graph &p_graph, const std::string& p_filename)
    throw (GraphException)
{
#ifndef NDEBUG
    std::cout << "Read graphviz file." << std::endl;
#endif /* NDEBUG */
    std::ifstream in(p_filename.c_str(), std::ifstream::in);

    if (in.is_open()) {
        boost::dynamic_properties dp = getProperties(p_graph);

        try {
            boost::read_graphviz(in, p_graph, dp);
#ifndef NDEBUG
            std::cout << "Successfully read graph." << std::endl;
#endif /* NDEBUG */
        } catch (...) {
            throw GraphException(GraphException::GRAPH_READ_ERROR);
        }

        in.close();
    } else {
        throw GraphException(GraphException::GRAPH_INPUT_ERROR);
    }
}


boost::dynamic_properties GraphUtil::getProperties(Graph &p_graph)
{
    boost::dynamic_properties dp;
    dp.property(EDGE_WEIGHT, get(boost::edge_weight, p_graph));
    dp.property(VERTEX_ID, get(boost::vertex_index, p_graph));
    dp.property(SERVICE_RATE, get(vertex_service_rate, p_graph));
    dp.property(ARRIVAL_RATE, get(vertex_arrival_rate, p_graph));
    dp.property(BUSY, get(vertex_busy, p_graph));
    dp.property(TIME_SERVICE_ENDS, get(vertex_time_service_ends, p_graph));
    dp.property(NUMBER_IN_QUEUE, get(vertex_number_in_queue, p_graph));
    dp.property(AVERAGE_DELAY_IN_QUEUE, get(vertex_average_delay_in_queue, p_graph));
    dp.property(NUM_EVENTS, get(vertex_num_events, p_graph));
    dp.property(UTILISATION, get(vertex_utilisation, p_graph));
    dp.property(BDT, get(vertex_Bdt, p_graph));
    dp.property(QDT, get(vertex_Qdt, p_graph));
    dp.property(LAST_EVENT_TIME, get(vertex_last_event_time, p_graph));
    dp.property(EXPECTED_AVERAGE_NUMBER_EVENT, get(vertex_expected_average_number_event, p_graph));
    dp.property(NUM_EVENTS_PROCESSED, get(vertex_num_events_processed, p_graph));
    dp.property(MEAN_REWARD, get(vertex_mean_reward, p_graph));
    dp.property(Q_VALUE, get(edge_q_val, p_graph));
    dp.property(EDGE_INDEX, get(edge_eindex, p_graph));
    dp.property(NEXT_ACTION, get(vertex_next_action, p_graph));
    dp.property(NEXT_EVENT_TIME, get(vertex_next_event_time, p_graph));
    dp.property(AVG_EVENT_IN_SYSTEM_TIME, get(vertex_avg_event_in_system_time, p_graph));
    dp.property(EXPERT_NORMAL, get(vertex_expert_normal, p_graph));
    dp.property(EXPERT_ABSOLUTE, get(vertex_expert_absolute, p_graph));
    dp.property(EXPERT_POSITIVE, get(vertex_expert_positive, p_graph));
    dp.property(EXPERT_NEGATIVE, get(vertex_expert_negative, p_graph));

    boost::ref_property_map<Graph*, boost::uint16_t>
        graphGenerator(get_property(p_graph, graph_generator));
    dp.property(GRAPH_GENERATOR, graphGenerator);

    return dp;
}


}
}
