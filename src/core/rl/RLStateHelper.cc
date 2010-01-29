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

/** @file RLStateHelper.cc
 * Implementation of the RL state helper
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <limits>

#include <boost/foreach.hpp>

#include "RLStateHelper.hh"


namespace des
{
namespace rl
{


void RLStateHelper::fillStateVector(dnet::Edge &p_edge, dnet::Graph &p_graph,
                                    std::vector<int> &p_stateRepresentation,
                                    DoubleSA p_inputs)
{
    dnet::Vertex source = boost::source(p_edge, p_graph);
    dnet::Vertex target = boost::target(p_edge, p_graph);
    
    for (boost::uint16_t i = 0; i < p_stateRepresentation.size(); ++i) {
        switch (p_stateRepresentation[i]) {
          case 1: 
          {
              
              dnet::VertexExpectedAverageNumberEventMap vertex_expected_average_number_event_map
                  = get(vertex_expected_average_number_event, p_graph);
              p_inputs[i] = vertex_expected_average_number_event_map[source];
              
              break;
          }
          case 2:
          {
              dnet::VertexNumberInQueueMap vertex_number_in_queue_map
                  = get(vertex_number_in_queue, p_graph);
              p_inputs[i] = vertex_number_in_queue_map[source];
              
              break;
          }
          case 3:
          {
              dnet::VertexUtilisationMap vertex_utilisation_map
                  = get(vertex_utilisation, p_graph);
              p_inputs[i] = vertex_utilisation_map[source];
              
              break;
          }
          case 4:
          {
              dnet::VertexAvgEventInSystemTimeMap vertex_avg_event_in_system_time_map
                  = get(vertex_avg_event_in_system_time, p_graph);
              p_inputs[i] = vertex_avg_event_in_system_time_map[source];
              
              break;
          }
          case 5:
          {
              dnet::VertexAverageDelayInQueueMap vertex_average_delay_in_queue_map
                  = get(vertex_average_delay_in_queue, p_graph);
              p_inputs[i] = vertex_average_delay_in_queue_map[source];
              
              break;
          }
          case 50:
          {
              dnet::VertexExpertNormalMap vertex_expert_normal_map
                  = get(vertex_expert_normal, p_graph);
              p_inputs[i] = vertex_expert_normal_map[target];
              
              break;
          }
          case 51:
          {
              dnet::VertexExpertAbsoluteMap vertex_expert_absolute_map
                  = get(vertex_expert_absolute, p_graph);
              p_inputs[i] = vertex_expert_absolute_map[target];
              
              break;
          }
          case 52:
          {
              dnet::VertexExpertPositiveMap vertex_expert_positive_map
                  = get(vertex_expert_positive, p_graph);
              p_inputs[i] = vertex_expert_positive_map[target];
              
              break;
          }
          case 53:
          {
              dnet::VertexExpertNegativeMap vertex_expert_negative_map
                  = get(vertex_expert_negative, p_graph);
              p_inputs[i] = vertex_expert_negative_map[target];
              
              break;
          }
          case 54:
          {
              dnet::VertexExpectedAverageNumberEventMap vertex_expected_average_number_event_map
                  = get(vertex_expected_average_number_event, p_graph);
              p_inputs[i] = vertex_expected_average_number_event_map[target];
              
              break;
          }
          case 55:
          {
              dnet::VertexNumberInQueueMap vertex_number_in_queue_map
                  = get(vertex_number_in_queue, p_graph);
              p_inputs[i] = vertex_number_in_queue_map[target];
              
              break;
          }
          case 56:
          {
              dnet::VertexUtilisationMap vertex_utilisation_map
                  = get(vertex_utilisation, p_graph);
              p_inputs[i] = vertex_utilisation_map[target];
              
              break;
          }
          case 57:
          {
              dnet::VertexAvgEventInSystemTimeMap vertex_avg_event_in_system_time_map
                  = get(vertex_avg_event_in_system_time, p_graph);
              p_inputs[i] = vertex_avg_event_in_system_time_map[target];
              
              break;
          }
          case 58:
          {
              dnet::VertexAverageDelayInQueueMap vertex_average_delay_in_queue_map
                  = get(vertex_average_delay_in_queue, p_graph);
              p_inputs[i] = vertex_average_delay_in_queue_map[target];
              
              break;
          }
          case 59:
          {
              dnet::EdgeQValueMap edge_q_val_map = get(edge_q_val, p_graph);
              double qMax = std::numeric_limits<double>::min();

              // find the max Q-Value of the target node
              BOOST_FOREACH(dnet::Edge e, (boost::out_edges(target, p_graph))) {
                  if (edge_q_val_map[e] > qMax) {
                      qMax = edge_q_val_map[e];
                  }
              }

              p_inputs[i] = qMax;
              
              break;
          }
          case 60:
          {
              dnet::EdgeQValueMap edge_q_val_map = get(edge_q_val, p_graph);
              double qMin = std::numeric_limits<double>::max();

              // find the max Q-Value of the target node
              BOOST_FOREACH(dnet::Edge e, (boost::out_edges(target, p_graph))) {
                  if (edge_q_val_map[e] < qMin) {
                      qMin = edge_q_val_map[e];
                  }
              }

              p_inputs[i] = qMin;
              
              break;
          }
          default:
              break;
        }
    }
}


}
}
