// Copyright (C) 2008, 2009, 2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file DirectedGraph.hh
 * Declaration of the directed graph and properties using the boost graph library.
 *
 * @author Dominik Dahlem
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __DES_NETWORK_DIRECTED_GRAPH_HH__
#define __DES_NETWORK_DIRECTED_GRAPH_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/property_iter_range.hpp>

#include <gsl/gsl_rng.h>


/** @enum vertex_service_rate_t
 * This enum extends the vertex properties by a service_rate attribute.
 */
enum vertex_service_rate_t { vertex_service_rate = 1111 };


/** @enum vertex_arrival_rate_t
 * This enum extends the vertex properties by a arrival_rate attribute.
 */
enum vertex_arrival_rate_t { vertex_arrival_rate = 1112 };


/** @enum vertex_busy_t
 * This enum extends the vertex properties by an busy attribute.
 */
enum vertex_busy_t { vertex_busy = 1113 };


/** @enum vertex_time_service_ends_t
 * This enum extends the vertex properties by a time service ends attribute.
 */
enum vertex_time_service_ends_t { vertex_time_service_ends = 1114 };


/** @enum vertex_number_in_queue_t
 * This enum extends the vertex properties by a number in queue attribute.
 */
enum vertex_number_in_queue_t { vertex_number_in_queue = 1115 };


/** @enum vertex_average_delay_in_queue_t
 * This enum extends the vertex properties by a average delay in queue attribute.
 */
enum vertex_average_delay_in_queue_t { vertex_average_delay_in_queue = 1116 };


/** @enum vertex_num_events_t
 * This enum extends the vertex properties by a number of events attribute.
 */
enum vertex_num_events_t { vertex_num_events = 1117 };


/** @enum vertex_Bdt_t
 * This enum extends the vertex properties by a fractional utilisation attribute.
 */
enum vertex_Bdt_t { vertex_Bdt = 1118 };


/** @enum vertex_Qdt_t
 * This enum extends the vertex properties by a fractional expected number of events in queue attribute.
 */
enum vertex_Qdt_t { vertex_Qdt = 1119 };


/** @enum vertex_utilisation_t
 * This enum extends the vertex properties by a utilisation attribute.
 */
enum vertex_utilisation_t { vertex_utilisation = 1120 };


/** @enum vertex_utilisation_t
 * This enum extends the vertex properties by a expected average number of events in queue attribute.
 */
enum vertex_expected_average_number_event_t { vertex_expected_average_number_event = 1121 };


/** @enum vertex_last_event_time_t
 * This enum extends the vertex properties by a last event time attribute.
 */
enum vertex_last_event_time_t { vertex_last_event_time = 1122 };


/** @enum vertex_num_events_processed_t
 * This enum extends the vertex properties by a number of a events processed attribute.
 */
enum vertex_num_events_processed_t { vertex_num_events_processed = 1123 };


/** @enum vertex_mean_reward_t
 * This enum extends the vertex properties by a mean delay attribute.
 */
enum vertex_mean_reward_t { vertex_mean_reward = 1124 };


/** @enum edge_q_val_t
 * This enum extends the vertex properties by a q-value attribute.
 */
enum edge_q_val_t { edge_q_val = 1125 };


/** @enum edge_index_t
 * This enum extends the edge properties by an index attribute
 */
enum edge_eindex_t { edge_eindex = 1126 };


/** @enum graph_generator_t
 * This enum extends the graph properties by a generator argument
 */
enum graph_generator_t { graph_generator = 1127 };


/** @enum vertex_next_action_t
 * This enum extends the vertex properties by a next action argument
 */
enum vertex_next_action_t { vertex_next_action = 1128 };


/** @enum vertex_next_event_time_t
 * This enum extends the vertex properties by a next event time argument,
 * which is used when event generation phases are enabled.
 */
enum vertex_next_event_time_t { vertex_next_event_time = 1129 };


/** @enum vertex_avg_event_in_system_time_t
 * This enum extends the vertex properties by a next event time argument,
 * which is used when event generation phases are enabled.
 */
enum vertex_avg_event_in_system_time_t { vertex_avg_event_in_system_time = 1130 };


/** @enum vertex_expert_normal_t
 * This enum extends the vertex properties by an expert metric (normal) argument
 */
enum vertex_expert_normal_t { vertex_expert_normal = 1131 };


/** @enum vertex_expert_absolute_t
 * This enum extends the vertex properties by an expert metric (absolute) argument
 */
enum vertex_expert_absolute_t { vertex_expert_absolute = 1132 };


/** @enum vertex_expert_positive_t
 * This enum extends the vertex properties by an expert metric (positive) argument
 */
enum vertex_expert_positive_t { vertex_expert_positive = 1133 };


/** @enum vertex_expert_negative_t
 * This enum extends the vertex properties by an expert metric (negative) argument
 */
enum vertex_expert_negative_t { vertex_expert_negative = 1134 };


/** @enum vertex_regret_absolute_t
 * This enum extends the vertex properties by an absolute regret argument
 */
enum vertex_regret_absolute_t { vertex_regret_absolute = 1135 };


/** @enum vertex_incentive_deviate_t
 * This enum extends the vertex properties by an incentive to deviate argument
 */
enum vertex_incentive_deviate_t { vertex_incentive_deviate = 1136 };


/** @enum edge_total_reward_t
 * This enum extends the edge properties by a total reward attribute
 */
enum edge_total_reward_t { edge_total_reward = 1137 };


/** @enum vertex_actual_reward_t
 * This enum extends the vertex properties by an actual reward argument
 */
enum vertex_actual_reward_t { vertex_actual_reward = 1138 };


/** @enum vertex_best_response_t
 * This enum extends the vertex properties by an best response argument
 */
enum vertex_best_response_t { vertex_best_response = 1139 };


/** @enum vertex_v_nn_loss_t
 * This enum extends the vertex properties by an best response argument
 */
enum vertex_v_nn_loss_t { vertex_v_nn_loss = 1140 };


/** @enum edge_e_nn_loss_t
 * This enum extends the edge properties by a total reward attribute
 */
enum edge_e_nn_loss_t { edge_e_nn_loss = 1141 };


/** @enum edge_emotion_t
 * This enum extends the edge properties by an emotion attribute
 */
enum edge_emotion_t { edge_emotion = 1142 };


/** @enum edge_rmin_t
 * This enum extends the edge properties by a min. reward attribute
 */
enum edge_rmin_t { edge_rmin = 1143 };


/** @enum edge_rmax_t
 * This enum extends the edge properties by a max reward attribute
 */
enum edge_rmax_t { edge_rmax = 1144 };


/** @enum edge_e_pos_t
 * This enum extends the edge properties by a e(+) attribute
 */
enum edge_e_pos_t { edge_e_pos = 1145 };


/** @enum edge_e_neg_t
 * This enum extends the edge properties by a e(-) attribute
 */
enum edge_e_neg_t { edge_e_neg = 1146 };


enum graph_system_delay_t { graph_system_delay = 1147 };
enum graph_system_num_events_t { graph_system_num_events = 1148 };
enum graph_system_processing_time_t { graph_system_processing_time = 1149 };


/** @enum vertex_coeff_var_t
 * This enum extends the vertex properties by a coefficient of variation argument
 */
enum vertex_coeff_var_t { vertex_coeff_var = 1150 };



/** @enum edge_e_nn_loss_cv_t
 * This enum extends the edge properties by a NN loss coefficient of variation attribute
 */
enum edge_e_nn_loss_cv_t { edge_e_nn_loss_cv = 1151 };


/** @enum vertex_coeff_var_t
 * This enum extends the vertex properties by a NN loss coefficient of variation argument
 */
enum vertex_v_nn_loss_cv_t { vertex_v_nn_loss_cv = 1152 };


// install the vertex service rate property
namespace boost
{
BOOST_INSTALL_PROPERTY(vertex, service_rate);
BOOST_INSTALL_PROPERTY(vertex, arrival_rate);
BOOST_INSTALL_PROPERTY(vertex, busy);
BOOST_INSTALL_PROPERTY(vertex, time_service_ends);
BOOST_INSTALL_PROPERTY(vertex, number_in_queue);
BOOST_INSTALL_PROPERTY(vertex, average_delay_in_queue);
BOOST_INSTALL_PROPERTY(vertex, num_events);
BOOST_INSTALL_PROPERTY(vertex, Bdt);
BOOST_INSTALL_PROPERTY(vertex, Qdt);
BOOST_INSTALL_PROPERTY(vertex, utilisation);
BOOST_INSTALL_PROPERTY(vertex, expected_average_number_event);
BOOST_INSTALL_PROPERTY(vertex, last_event_time);
BOOST_INSTALL_PROPERTY(vertex, num_events_processed);
BOOST_INSTALL_PROPERTY(vertex, mean_reward);
BOOST_INSTALL_PROPERTY(vertex, next_action);
BOOST_INSTALL_PROPERTY(vertex, next_event_time);
BOOST_INSTALL_PROPERTY(vertex, avg_event_in_system_time);
BOOST_INSTALL_PROPERTY(vertex, expert_normal);
BOOST_INSTALL_PROPERTY(vertex, expert_absolute);
BOOST_INSTALL_PROPERTY(vertex, expert_positive);
BOOST_INSTALL_PROPERTY(vertex, expert_negative);
BOOST_INSTALL_PROPERTY(vertex, regret_absolute);
BOOST_INSTALL_PROPERTY(vertex, incentive_deviate);
BOOST_INSTALL_PROPERTY(vertex, actual_reward);
BOOST_INSTALL_PROPERTY(vertex, best_response);
BOOST_INSTALL_PROPERTY(vertex, v_nn_loss);
BOOST_INSTALL_PROPERTY(vertex, coeff_var);
BOOST_INSTALL_PROPERTY(vertex, v_nn_loss_cv);
BOOST_INSTALL_PROPERTY(graph, generator);
BOOST_INSTALL_PROPERTY(edge, q_val);
BOOST_INSTALL_PROPERTY(edge, eindex);
BOOST_INSTALL_PROPERTY(edge, total_reward);
BOOST_INSTALL_PROPERTY(edge, e_nn_loss);
BOOST_INSTALL_PROPERTY(edge, emotion);
BOOST_INSTALL_PROPERTY(edge, rmin);
BOOST_INSTALL_PROPERTY(edge, rmax);
BOOST_INSTALL_PROPERTY(edge, e_pos);
BOOST_INSTALL_PROPERTY(edge, e_neg);
BOOST_INSTALL_PROPERTY(edge, e_nn_loss_cv);
BOOST_INSTALL_PROPERTY(graph, system_delay);
BOOST_INSTALL_PROPERTY(graph, system_num_events);
BOOST_INSTALL_PROPERTY(graph, system_processing_time);
}


namespace des { namespace network {


/** @typedef VertexServiceRateProperty
 * Specifies the property for the vertex service rate
 */
typedef boost::property <vertex_service_rate_t, double> VertexServiceRateProperty;

/** @typedef VertexArrivalRateProperty
 * Specifies the property for the vertex arrival rate
 */
typedef boost::property <vertex_arrival_rate_t, double, VertexServiceRateProperty> VertexArrivalRateProperty;

/** @typedef VertexArrivalRateProperty
 * Specifies the property for the vertex number in queue
 */
typedef boost::property <vertex_number_in_queue_t, int, VertexArrivalRateProperty> VertexNumberInQueueProperty;

/** @typedef VertexAverageDelayInQueueProperty
 * Specifies the property for the vertex average delay in queue
 */
typedef boost::property <vertex_average_delay_in_queue_t, double, VertexNumberInQueueProperty> VertexAverageDelayInQueueProperty;

/** @typedef VertexBusyProperty
 * Specifies the property for the vertex busy flag
 */
typedef boost::property <vertex_busy_t, bool, VertexAverageDelayInQueueProperty> VertexBusyProperty;

/** @typedef VertexTimeServiceEndsProperty
 * Specifies the property for the time service ends attribute of a vertex
 */
typedef boost::property <vertex_time_service_ends_t, double, VertexBusyProperty> VertexTimeServiceEndsProperty;

/** @typedef VertexNumEventsProperty
 * Specifies the property for the number of events attribute of a vertex
 */
typedef boost::property <vertex_num_events_t, int, VertexTimeServiceEndsProperty> VertexNumEventsProperty;

/** @typedef VertexUtilisationProperty
 * Specifies the property for the utlisation attribute of a vertex
 */
typedef boost::property <vertex_utilisation_t, double, VertexNumEventsProperty> VertexUtilisationProperty;

/** @typedef VertexBdtProperty
 * Specifies the property for the fractional utlisation attribute of a vertex
 */
typedef boost::property <vertex_Bdt_t, double, VertexUtilisationProperty> VertexBdtProperty;

/** @typedef VertexQdtProperty
 * Specifies the property for the fractional expected number of events in queue attribute of a vertex
 */
typedef boost::property <vertex_Qdt_t, double, VertexBdtProperty> VertexQdtProperty;

/** @typedef VertexExpectedAverageNumberEventProperty
 * Specifies the property for the expected average number of events in queue attribute of a vertex
 */
typedef boost::property <vertex_expected_average_number_event_t, double, VertexQdtProperty> VertexExpectedAverageNumberEventProperty;

/** @typedef VertexLastEventTimeProperty
 * Specifies the property for the last event time attribute of a vertex
 */
typedef boost::property <vertex_last_event_time_t, double, VertexExpectedAverageNumberEventProperty> VertexLastEventTimeProperty;

/** @typedef VertexNumProcessedEventsProperty
 * Specifies the property for the number of processed events attribute of a vertex
 */
typedef boost::property <vertex_num_events_processed_t, int, VertexLastEventTimeProperty> VertexNumProcessedEventsProperty;

/** @typedef VertexMeanRewardProperty
 * Specifies the property for the mean delay of a vertex
 */
typedef boost::property <vertex_mean_reward_t, double, VertexNumProcessedEventsProperty> VertexMeanRewardProperty;

/** @typedef VertexNextActionProperty
 * Specifies the property for the next action of a vertex
 */
typedef boost::property <vertex_next_action_t, int, VertexMeanRewardProperty> VertexNextActionProperty;

/** @typedef VertexNextEventTimeProperty
 * Specifies the property for the next event time of a vertex
 */
typedef boost::property <vertex_next_event_time_t, double, VertexNextActionProperty> VertexNextEventTimeProperty;

/** @typedef VertexLeaveAvgResponseTimeProperty
 * Specifies the property for the average leave response of a vertex
 */
typedef boost::property <vertex_avg_event_in_system_time_t, double, VertexNextEventTimeProperty> VertexAvgEventInSystemTimeProperty;

/** @typedef VertexExpertNormalProperty
 * Specifies the property for the normal expert metric of a vertex
 */
typedef boost::property <vertex_expert_normal_t, double, VertexAvgEventInSystemTimeProperty> VertexExpertNormalProperty;

/** @typedef VertexExpertAbsoluteProperty
 * Specifies the property for the absolute expert metric of a vertex
 */
typedef boost::property <vertex_expert_absolute_t, double, VertexExpertNormalProperty> VertexExpertAbsoluteProperty;

/** @typedef VertexExpertPositiveProperty
 * Specifies the property for the positive expert metric of a vertex
 */
typedef boost::property <vertex_expert_positive_t, double, VertexExpertAbsoluteProperty> VertexExpertPositiveProperty;

/** @typedef VertexExpertNegativeProperty
 * Specifies the property for the negative expert metric of a vertex
 */
typedef boost::property <vertex_expert_negative_t, double, VertexExpertPositiveProperty> VertexExpertNegativeProperty;

/** @typedef VertexRegretAbsoluteProperty
 * Specifies the property for the absolute regret metric of a vertex
 */
typedef boost::property <vertex_regret_absolute_t, double, VertexExpertNegativeProperty> VertexRegretAbsoluteProperty;

/** @typedef VertexIncentiveDeviateProperty
 * Specifies the property for the incentive to deviate metric of a vertex
 */
typedef boost::property <vertex_incentive_deviate_t, double, VertexRegretAbsoluteProperty> VertexIncentiveDeviateProperty;

/** @typedef VertexActualRewardProperty
 * Specifies the property for the actual reward metric of a vertex
 */
typedef boost::property <vertex_actual_reward_t, double, VertexIncentiveDeviateProperty> VertexActualRewardProperty;

/** @typedef VertexBestResponseProperty
 * Specifies the property for the best response metric of a vertex
 */
typedef boost::property <vertex_best_response_t, double, VertexActualRewardProperty> VertexBestResponseProperty;

/** @typedef VertexNNLossProperty
 * Specifies the property for the loss value of the NN of a vertex
 */
typedef boost::property <vertex_v_nn_loss_t, double, VertexBestResponseProperty> VertexNNLossProperty;

/** @typedef VertexCoeffVarProperties
 * This type definition assembles all the properties for the vertices of the graph
 */
typedef boost::property <vertex_coeff_var_t, double, VertexNNLossProperty> VertexCoeffVarProperties;

/** @typedef VertexNNLossCVProperties
 * Specifies the property for the coefficient of variation of the loss value of the NN
 */
typedef boost::property <vertex_v_nn_loss_cv_t, double, VertexCoeffVarProperties> VertexNNLossCVProperties;

/** @typedef VertexProperties
 * This type definition assembles all the properties for the vertices of the graph
 */
typedef boost::property <boost::vertex_index_t, int, VertexNNLossCVProperties> VertexProperties;

/** @typedef EdgeWeightProperty
 * Specifies the property for the edge weight
 */
typedef boost::property <boost::edge_weight_t, double> EdgeWeightProperty;

/** @typedef EdgeQValueProperty
 * Specifies the property for the probability of using edge i
 */
typedef boost::property <edge_q_val_t, double, EdgeWeightProperty> EdgeQValueProperty;

/** @typedef EdgeTotalRewardProperty
 * Specifies the property for the total reward
 */
typedef boost::property <edge_total_reward_t, double, EdgeQValueProperty> EdgeTotalRewardProperty;

/** @typedef EdgeNNLossProperty
 * Specifies the property for the loss value of the NN
 */
typedef boost::property <edge_e_nn_loss_t, double, EdgeTotalRewardProperty> EdgeNNLossProperty;

/** @typedef EdgeEmotionProperty
 * Specifies the property for the emotion of the edge
 */
typedef boost::property <edge_emotion_t, double, EdgeNNLossProperty> EdgeEmotionProperty;

/** @typedef EdgeRMinProperty
 * Specifies the property for the min. reward
 */
typedef boost::property <edge_rmin_t, double, EdgeEmotionProperty> EdgeRminProperty;

/** @typedef EdgeRmaxProperty
 * Specifies the property for the max. reward
 */
typedef boost::property <edge_rmax_t, double, EdgeRminProperty> EdgeRmaxProperty;

/** @typedef EdgeEPosProperty
 * Specifies the property for the e(+)
 */
typedef boost::property <edge_e_pos_t, double, EdgeRmaxProperty> EdgeEPosProperty;

/** @typedef EdgeENegProperty
 * Specifies the property for the e(-)
 */
typedef boost::property <edge_e_neg_t, double, EdgeEPosProperty> EdgeENegProperty;

/** @typedef EdgeNNLossCVProperties
 * Specifies the property for the coefficient of variation of the loss value of the NN
 */
typedef boost::property <edge_e_nn_loss_cv_t, double, EdgeENegProperty> EdgeNNLossCVProperties;

/** @typedef EdgeIndexProperty
 * Specifies the property for the edge index
 */
typedef boost::property <edge_eindex_t, int, EdgeNNLossCVProperties> EdgeIndexProperty;

/** @typedef GraphGeneratorProperty
 * Specifies the property for graph generator
 */
typedef boost::property <graph_generator_t, int> GraphGeneratorProperty;

/** @typedef GraphSystemDelayProperty
 * Specifies the property for the system delay
 */
typedef boost::property <graph_system_delay_t, double, GraphGeneratorProperty> GraphSystemDelayProperty;

/** @typedef GraphSystemDelayProperty
 * Specifies the property for the system number of events
 */
typedef boost::property <graph_system_num_events_t, double, GraphSystemDelayProperty> GraphSystemNumEventsProperty;

/** @typedef GraphSystemProcessingTimeProperty
 * Specifies the property for the total processing time of events in the system
 */
typedef boost::property <graph_system_processing_time_t, double, GraphSystemNumEventsProperty> GraphSystemProcessingTimeProperty;


/** @typedef Graph
 * Specifies the Graph as an adjacency list. The edges are represented in a
 * set which means that no parallel edges are allowed. However, of the available
 * STL containers, this is the most expensive in terms of space complexity. The Vertices
 * are stored in a STL list to allow quick addition and removal with constant time
 * complexity.
 */
typedef boost::adjacency_list<boost::setS,
                              boost::listS,
                              boost::bidirectionalS,
                              VertexProperties,
                              EdgeIndexProperty,
                              GraphSystemProcessingTimeProperty> Graph;

/** @typedef Vertex
 * Specifies the vertex descriptor of a graph
 */
typedef boost::graph_traits <Graph>::vertex_descriptor Vertex;

/** @typedef Edge
 * Specifies the edge descriptor of a graph
 */
typedef boost::graph_traits <Graph>::edge_descriptor Edge;

/** @typedef VertexNumberInQueueMap
 * Specifies the map that stores the vertex service rate property
 */
typedef boost::property_map <Graph, vertex_number_in_queue_t>::type VertexNumberInQueueMap;

/** @typedef VertexAverageDelayInQueueMap
 * Specifies the map that stores the vertex average delay in queue property
 */
typedef boost::property_map <Graph, vertex_average_delay_in_queue_t>::type VertexAverageDelayInQueueMap;

/** @typedef VertexServiceRateMap
 * Specifies the map that stores the vertex service rate property
 */
typedef boost::property_map <Graph, vertex_service_rate_t>::type VertexServiceRateMap;

/** @typedef VertexArrivalRateMap
 * Specifies the map that stores the vertex arrival rate property
 */
typedef boost::property_map <Graph, vertex_arrival_rate_t>::type VertexArrivalRateMap;

/** @typedef VertexIndexMap
 * Specifies the map that stores the vertex indeces
 */
typedef boost::property_map <Graph, boost::vertex_index_t>::type VertexIndexMap;

/** @typedef VertexBusyMap
 * Specifies the map that stores the vertex busy flag
 */
typedef boost::property_map <Graph, vertex_busy_t>::type VertexBusyMap;

/** @typedef VertexTimeServiceEndsMap
 * Specifies the map that stores the vertex time service ends attribute
 */
typedef boost::property_map <Graph, vertex_time_service_ends_t>::type VertexTimeServiceEndsMap;

/** @typedef VertexNumEventsMap
 * Specifies the map that stores the vertex number of events attribute
 */
typedef boost::property_map <Graph, vertex_num_events_t>::type VertexNumEventsMap;

/** @typedef VertexUtilisationMap
 * Specifies the map that stores the vertex utilisation attribute
 */
typedef boost::property_map <Graph, vertex_utilisation_t>::type VertexUtilisationMap;

/** @typedef VertexBdtMap
 * Specifies the map that stores the vertex fractional utilisation attribute
 */
typedef boost::property_map <Graph, vertex_Bdt_t>::type VertexBdtMap;

/** @typedef VertexQdtMap
 * Specifies the map that stores the vertex fractional expected number of events in the queue attribute
 */
typedef boost::property_map <Graph, vertex_Qdt_t>::type VertexQdtMap;

/** @typedef VertexLastEventTimeMap
 * Specifies the map that stores the vertex expected average number events attribute
 */
typedef boost::property_map <Graph, vertex_expected_average_number_event_t>::type VertexExpectedAverageNumberEventMap;

/** @typedef VertexLastEventTimeMap
 * Specifies the map that stores the vertex last event time attribute
 */
typedef boost::property_map <Graph, vertex_last_event_time_t>::type VertexLastEventTimeMap;

/** @typedef VertexNumEventsProcessedMap
 * Specifies the map that stores the vertex number of processed events attribute
 */
typedef boost::property_map <Graph, vertex_num_events_processed_t>::type VertexNumEventsProcessedMap;

/** @typedef VertexMeanRewardMap
 * Specifies the map that stores the vertex mean delay attribute
 */
typedef boost::property_map <Graph, vertex_mean_reward_t>::type VertexMeanRewardMap;

/** @typedef VertexNextActionMap
 * Specifies the map that stores the vertex next action attribute
 */
typedef boost::property_map <Graph, vertex_next_action_t>::type VertexNextActionMap;

/** @typedef VertexNextEventTimeMap
 * Specifies the map that stores the vertex next event time attribute
 */
typedef boost::property_map <Graph, vertex_next_event_time_t>::type VertexNextEventTimeMap;

/** @typedef VertexAvgEventInSystemTimeMap
 * Specifies the map that stores the vertex attribute avg_event_in_system_time
 */
typedef boost::property_map <Graph, vertex_avg_event_in_system_time_t>::type VertexAvgEventInSystemTimeMap;

/** @typedef VertexExpertNormalMap
 * Specifies the map that stores the vertex attribute expert_normal
 */
typedef boost::property_map <Graph, vertex_expert_normal_t>::type VertexExpertNormalMap;

/** @typedef VertexExpertAbsoluteMap
 * Specifies the map that stores the vertex attribute expert_absolute
 */
typedef boost::property_map <Graph, vertex_expert_absolute_t>::type VertexExpertAbsoluteMap;

/** @typedef VertexExpertPositiveMap
 * Specifies the map that stores the vertex attribute expert_positive
 */
typedef boost::property_map <Graph, vertex_expert_positive_t>::type VertexExpertPositiveMap;

/** @typedef VertexExpertNegativeMap
 * Specifies the map that stores the vertex attribute expert_negative
 */
typedef boost::property_map <Graph, vertex_expert_negative_t>::type VertexExpertNegativeMap;

/** @typedef VertexRegretAbsoluteMap
 * Specifies the map that stores the vertex attribute regret_absolute
 */
typedef boost::property_map <Graph, vertex_regret_absolute_t>::type VertexRegretAbsoluteMap;

/** @typedef VertexIncentiveDeviateMap
 * Specifies the map that stores the vertex attribute incentive_deviate
 */
typedef boost::property_map <Graph, vertex_incentive_deviate_t>::type VertexIncentiveDeviateMap;

/** @typedef VertexActualRewardMap
 * Specifies the map that stores the vertex attribute actual_reward
 */
typedef boost::property_map <Graph, vertex_actual_reward_t>::type VertexActualRewardMap;

/** @typedef VertexBestResponseMap
 * Specifies the map that stores the vertex attribute best_response
 */
typedef boost::property_map <Graph, vertex_best_response_t>::type VertexBestResponseMap;

/** @typedef VertexNNLossMap
 * Specifies the map that stores the vertex attribute nn_loss
 */
typedef boost::property_map <Graph, vertex_v_nn_loss_t>::type VertexNNLossMap;

/** @typedef VertexCoeffVarMap
 * Specifies the map that stores the vertex attribute coeff_var
 */
typedef boost::property_map <Graph, vertex_coeff_var_t>::type VertexCoeffVarMap;

/** @typedef VertexCoeffVarMap
 * Specifies the map that stores the vertex attribute coeff_var
 */
typedef boost::property_map <Graph, vertex_v_nn_loss_cv_t>::type VertexNNLossCVMap;

/** @typedef EdgeQValueMap
 * Specifies the map that stores the edge q-value attribute
 */
typedef boost::property_map <Graph, edge_q_val_t>::type EdgeQValueMap;

/** @typedef EdgeIndexMap
 * Specifies the map that stores the edge index attribute
 */
typedef boost::property_map <Graph, edge_eindex_t>::type EdgeIndexMap;

/** @typedef EdgeTotalRewardMap
 * Specifies the map that stores the total reward attribute
 */
typedef boost::property_map <Graph, edge_total_reward_t>::type EdgeTotalRewardMap;

/** @typedef EdgeNNLossMap
 * Specifies the map that stores the nn_loss attribute
 */
typedef boost::property_map <Graph, edge_e_nn_loss_t>::type EdgeNNLossMap;

/** @typedef EdgeWeightMap
 * Specifies the edge weight property
 */
typedef boost::property_map <Graph, boost::edge_weight_t>::type EdgeWeightMap;

/** @typedef EdgeEmotionMap
 * Specifies the edge emotion property
 */
typedef boost::property_map <Graph, edge_emotion_t>::type EdgeEmotionMap;

/** @typedef EdgeRMinMap
 * Specifies the edge rmin property
 */
typedef boost::property_map <Graph, edge_rmin_t>::type EdgeRMinMap;

/** @typedef EdgeRMaxMap
 * Specifies the edge rmax property
 */
typedef boost::property_map <Graph, edge_rmax_t>::type EdgeRMaxMap;

/** @typedef EdgeEPosMap
 * Specifies the edge e(+) property
 */
typedef boost::property_map <Graph, edge_e_pos_t>::type EdgeEPosMap;

/** @typedef EdgeENegMap
 * Specifies the edge e(-) property
 */
typedef boost::property_map <Graph, edge_e_neg_t>::type EdgeENegMap;

/** @typedef EdgeNNLossCVMap
 * Specifies the edge coefficient of variation of the NN loss property
 */
typedef boost::property_map <Graph, edge_e_nn_loss_cv_t>::type EdgeNNLossCVMap;

/** @typedef VServiceIterator
 * Specifies the iterator for the vertex service rates
 */
typedef boost::graph_property_iter_range <Graph, vertex_service_rate_t>::iterator VServiceIterator;

/** @typedef VArrivalIterator
 * Specifies the iterator for the vertex arrival rates
 */
typedef boost::graph_property_iter_range <Graph, vertex_arrival_rate_t>::iterator VArrivalIterator;

/** @typedef VIndexIterator
 * Specifies the iterator for the vertex arrival rates
 */
typedef boost::graph_property_iter_range <Graph, boost::vertex_index_t>::iterator VIndexIterator;

/** @typedef VExpectedAverageNumEventsIterator
 * Specifies the iterator for the vertex expected average number of events in queue
 */
typedef boost::graph_property_iter_range <Graph, vertex_expected_average_number_event_t>::iterator VExpectedAverageNumEventsIterator;

/** @typedef VAverageDelayQueueIterator
 * Specifies the iterator for the vertex average delay in queue
 */
typedef boost::graph_property_iter_range <Graph, vertex_average_delay_in_queue_t>::iterator VAverageDelayQueueIterator;

/** @typedef VAvgEventInSystemTimeIterator
 * Specifies the iterator for the vertex average event in system time
 */
typedef boost::graph_property_iter_range <Graph, vertex_avg_event_in_system_time_t>::iterator VAvgEventInSystemTimeIterator;

/** @typedef EQValIterator
 * Specifies the iterator for the edge q value
 */
typedef boost::graph_property_iter_range <Graph, edge_q_val_t>::iterator EQValIterator;

/** @typedef OutEdgeIterator
 * Specifies the iterator for the out degree edges
 */
typedef boost::graph_traits <Graph>::out_edge_iterator OutEdgeIterator;

/** @typedef VertexIterator
 * Specifies the iterator for the vertices
 */
typedef boost::graph_traits <Graph>::vertex_iterator VertexIterator;

/** @typedef tGraphSP
 * Specifies shared pointer to the Graph object
 */
typedef boost::shared_ptr <Graph> tGraphSP;

/** @typedef tGslRngSP
 * Specifies shared pointer to the GSL random number generator objects
 */
typedef boost::shared_ptr <gsl_rng> tGslRngSP;


/** @struct exists_vertex_index
 * This is a struct defining a predicate function to filter the graph, where the subset of the
 * original graph only contains a vertex matching a specified index.
 */
template <typename VertexIndexMap>
struct exists_vertex_index {
    exists_vertex_index()
        {}

    exists_vertex_index(VertexIndexMap p_indexMap, int p_index)
        : m_indexMap(p_indexMap), m_index(p_index)
        {}

    template <typename Vertex>
    bool operator()(const Vertex& v) const
        {
            return m_index == get(m_indexMap, v);
        }

    VertexIndexMap m_indexMap;
    int m_index;
};


template <typename EdgesToBeRemoved>
struct cycle_detector : public boost::dfs_visitor<>
{
    cycle_detector(EdgesToBeRemoved &edgesToBeRemoved)
        : m_edgesToBeRemoved(edgesToBeRemoved) { }

    template <class Edge, class Graph>
    void back_edge(Edge e, Graph &g) {
        m_edgesToBeRemoved.push_back(e);
    }

protected:
    EdgesToBeRemoved &m_edgesToBeRemoved;

};


const std::string EDGE_WEIGHT                       = "weight";
const std::string VERTEX_ID                         = "vindex";
const std::string SERVICE_RATE                      = "service_rate";
const std::string ARRIVAL_RATE                      = "arrival_rate";
const std::string BUSY                              = "busy";
const std::string TIME_SERVICE_ENDS                 = "time_service_ends";
const std::string NUMBER_IN_QUEUE                   = "number_in_queue";
const std::string AVERAGE_DELAY_IN_QUEUE            = "average_delay_in_queue";
const std::string NUM_EVENTS                        = "num_events";
const std::string UTILISATION                       = "utilisation";
const std::string BDT                               = "Bdt";
const std::string QDT                               = "Qdt";
const std::string LAST_EVENT_TIME                   = "last_event_time";
const std::string EXPECTED_AVERAGE_NUMBER_EVENT     = "expected_average_number_event";
const std::string NUM_EVENTS_PROCESSED              = "num_events_processed";
const std::string MEAN_REWARD                       = "mean_reward";
const std::string Q_VALUE                           = "q_value";
const std::string EDGE_INDEX                        = "eindex";
const std::string NEXT_ACTION                       = "next_action";
const std::string GRAPH_GENERATOR                   = "graph_generator";
const std::string AVG_EVENT_IN_SYSTEM_TIME          = "avg_event_in_system_time";
const std::string EXPERT_NORMAL                     = "expert_normal";
const std::string EXPERT_ABSOLUTE                   = "expert_absolute";
const std::string EXPERT_POSITIVE                   = "expert_positive";
const std::string EXPERT_NEGATIVE                   = "expert_negative";
const std::string REGRET_ABSOLUTE                   = "regret_absolute";
const std::string INCENTIVE_DEVIATE                 = "incentive_deviate";
const std::string ACTUAL_REWARD                     = "actual_reward";
const std::string BEST_RESPONSE                     = "best_response";
const std::string VERTEX_NN_LOSS                    = "vertex_nn_loss";
const std::string VERTEX_COEFF_VAR                  = "vertex_coeff_var";
const std::string VERTEX_V_NN_LOSS_CV               = "vertex_v_nn_loss_cv";
const std::string EDGE_TOTAL_REWARD                 = "total_reward";
const std::string EDGE_NN_LOSS                      = "edge_nn_loss";
const std::string EDGE_EMOTION                      = "edge_emotion";
const std::string EDGE_RMIN                         = "edge_rmin";
const std::string EDGE_RMAX                         = "edge_rmax";
const std::string EDGE_E_POS                        = "edge_e_pos";
const std::string EDGE_E_NEG                        = "edge_e_neg";
const std::string EDGE_E_NN_LOSS_CV                 = "e_nn_loss_cv";
const std::string GRAPH_SYSTEM_DELAY                = "system_delay";
const std::string GRAPH_SYSTEM_NUM_EVENTS           = "system_num_events";
const std::string GRAPH_SYSTEM_PROCESSING           = "system_processing";


    }
}


#endif
