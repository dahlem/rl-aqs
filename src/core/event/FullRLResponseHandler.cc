// Copyright (C) 2009, 2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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

/** @file FullRLResponseHandler.cc
 * Implementation of a basic FullRLResponse handler.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NDEBUG_EVENTS
# include <iostream>
#endif /* NDEBUG_EVENTS */

#include <algorithm>
#include <cmath>

#include <boost/foreach.hpp>

#include "Entry.hh"
namespace dcommon = des::common;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "Seeds.hh"
#include "CRN.hh"
namespace dsample = des::sampling;

#include "Policy.hh"
#include "EpsilonGreedy.hh"
#include "RLStateHelper.hh"
namespace drl = des::rl;

#include "events.hh"
#include "AckEvent.hh"
#include "FullRLResponseHandler.hh"
#include "GraphChannel.hh"
#include "ConfigChannel.hh"


namespace des
{
namespace core
{


FullRLResponseHandler::FullRLResponseHandler(
    DesBus &p_bus, drl::Policy &p_policy, double p_q_alpha, double p_q_lambda, double p_momentum)
    : m_graph((dynamic_cast<GraphChannel&> (p_bus.getChannel(id::GRAPH_CHANNEL))).getGraph()),
      m_q_lambda(p_q_lambda), m_policy(p_policy),
      m_state_representation(((dynamic_cast<dcore::ConfigChannel&> (p_bus.getChannel(dcore::id::CONFIG_CHANNEL))).getConfig()).rl_state_representation),
      qStatsSA(new dstats::OnlineStats[(((dynamic_cast<dcore::ConfigChannel&> (p_bus.getChannel(dcore::id::CONFIG_CHANNEL))).getConfig()).nn_outsource) ? (boost::num_vertices(m_graph)) : (boost::num_edges(m_graph))])
{
    dcore::desArgs_t config = (dynamic_cast<dcore::ConfigChannel&> (p_bus.getChannel(dcore::id::CONFIG_CHANNEL))).getConfig();
    boost::uint16_t hidden_neurons = config.nn_hidden_neurons;

    m_outsource = config.nn_outsource;
    m_regret_total = config.regret_absolute;
    m_incentive_deviate = config.incentive_deviate;
    m_nn_loss_serialise = config.nn_loss_serialise;

    boost::uint32_t seed = dsample::Seeds::getInstance().getSeed();
    boost::int32_t uniform_rng_index = dsample::CRN::getInstance().init(seed);
    dsample::CRN::getInstance().log(seed, "Neural Network uniform");

    vertex_next_action_map = get(vertex_next_action, m_graph);
    vertex_index_map = get(boost::vertex_index, m_graph);
    vertex_actual_reward_map = get(vertex_actual_reward, m_graph);
    vertex_regret_total_map = get(vertex_regret_absolute, m_graph);
    vertex_incentive_deviate_map = get(vertex_incentive_deviate, m_graph);
    vertex_nn_loss_map = get(vertex_v_nn_loss, m_graph);
    vertex_nn_loss_cv_map = get(vertex_v_nn_loss_cv, m_graph);
    edge_q_val_map = get(edge_q_val, m_graph);
    edge_index_map = get(edge_eindex, m_graph);
    edge_total_reward_map = get(edge_total_reward, m_graph);
    edge_nn_loss_map = get(edge_e_nn_loss, m_graph);
    edge_nn_loss_cv_map = get(edge_e_nn_loss_cv, m_graph);

    // init the neural network for each edge
    boost::uint16_t num_nets = (m_outsource) ? (boost::num_vertices(m_graph)) : (boost::num_edges(m_graph));

#ifndef NDEBUG_EVENTS
    std::cout << "** FullRLResponse" << std::endl;
    std::cout << "Create " << num_nets << " function approximators."  << std::endl;
#endif /* NDEBUG_EVENTS */

    for (boost::uint16_t i = 0; i < num_nets; ++i) {
#ifndef NDEBUG_EVENTS
        std::cout << "Create function approximator for edge: " << i << std::endl;
#endif /* NDEBUG_EVENTS */

        // create the neural network for edge i
        FFNetSP net = dnnet::NNetFactory::createNNet<dnnet::HTangent, dnnet::Identity>
            (m_state_representation.size(), hidden_neurons, 1, uniform_rng_index);

        ObjMseSP mse;
        if (config.nn_loss_policy == 1) {
            mse = dnnet::NNetFactory::createDefaultMSEObjective<dnnet::HTangent, dnnet::Identity>(net);
        } else if (config.nn_loss_policy == 2) {
            mse = dnnet::NNetFactory::createSlidingWindowMSEObjective<dnnet::HTangent, dnnet::Identity>(net, config.nn_window);
        }

        dnnet::TrainingSP training;
        if (config.nn_cg) {
            training = dnnet::NNetFactory::createConjugateGradientTraining<dnnet::HTangent, dnnet::Identity>(
                net, mse, p_q_alpha, 1e-6, config.nn_brent_iter);
        } else {
            training = dnnet::NNetFactory::createBackpropagationTraining<dnnet::HTangent, dnnet::Identity>(
                net, mse, p_q_alpha, p_momentum, 1e-6);
        }

        // store the neural network, objective function, and training method into vectors
        m_nets.push_back(net);
        m_objectives.push_back(mse);
        m_trainings.push_back(training);
    }

#ifndef NDEBUG_EVENTS
    std::cout << "Created " << m_nets.size() << " function approximators."  << std::endl;
    std::cout << "Created " << m_objectives.size() << " loss functions."  << std::endl;
    std::cout << "Created " << m_trainings.size() << " training algos."  << std::endl;
#endif /* NDEBUG_EVENTS */

    // allocate memory here for the input vector and the target
    m_inputs = DoubleSA(new double[m_state_representation.size()]);
    m_target = DoubleSA(new double[1]);
}


FullRLResponseHandler::~FullRLResponseHandler()
{
#ifndef NDEBUG_EVENTS
    std::cout << "** ~FullRLResponse" << std::endl;
#endif /* NDEBUG_EVENTS */

    m_trainings.clear();
    m_objectives.clear();
    m_nets.clear();
}


void FullRLResponseHandler::update(AckEvent *subject)
{
    dcommon::Entry *entry = subject->getEvent();

#ifndef NDEBUG_EVENTS
    std::cout << "** FullRLResponse for vertex: " << entry->getDestination() << std::endl;
    std::cout << "Event: " << const_cast <const dcommon::Entry&> (*entry) << std::endl;
#endif /* NDEBUG_EVENTS */

    dnet::Vertex vertex = boost::vertex(entry->getDestination(), m_graph);
    dnet::Graph::degree_size_type degree = boost::out_degree(vertex, m_graph);
    boost::uint16_t newAction = 0;

    // observe reward (the longer it takes the smaller the reward)
    double reward = entry->getReward();

    if (degree > 1) {
#ifndef NDEBUG_EVENTS
        std::cout << "Reward: " << reward << std::endl;
#endif /* NDEBUG_EVENTS */

        // choose new action based on new state
        drl::tValuesVec values(boost::out_degree(vertex, m_graph));

        int count = 0;
        BOOST_FOREACH(dnet::Edge e, (boost::out_edges(vertex, m_graph))) {
            drl::tValues value;
            int target_vertex = vertex_index_map[boost::target(e, m_graph)];
            value.first = target_vertex;

            // collect the input vector
            drl::RLStateHelper::fillStateVector(e, m_graph, m_state_representation, m_inputs);

            // calculate the neural network prediction given current state
            boost::uint16_t index = (m_outsource) ? (target_vertex) : (edge_index_map[e]);
            value.second = m_nets[index]->present(m_inputs)[0];
//            value.second = edge_q_val_map[e];

#ifndef NDEBUG_EVENTS
            std::cout << "Action-Value Pair: " << value.first << ", " << value.second << std::endl;
#endif /* NDEBUG_EVENTS */
            values[count++] = value;
        }

        // hook into total regret
        if (m_regret_total) {
            // update actual rewards
            vertex_actual_reward_map[vertex] += reward;

            // update rewards for all actions
            BOOST_FOREACH(drl::tValues value, values) {
                dnet::Edge e = boost::edge(vertex, boost::vertex(value.first, m_graph), m_graph).first;
                // take actual reward instead of estimated for the current action
                if (entry->getOrigin() == value.first) {
                    edge_total_reward_map[e] += reward;
                } else {
                    edge_total_reward_map[e] += value.second;
                }
            }

            // sort the values
            std::sort(values.begin(), values.end(), drl::val_greater);

            // calculate R_T
            drl::tValues value = values.front();
            dnet::Edge e = boost::edge(vertex, boost::vertex(value.first, m_graph), m_graph).first;
            vertex_regret_total_map[vertex] = edge_total_reward_map[e] - vertex_actual_reward_map[vertex];
        }

        // hook into incentive deviate
        if (m_incentive_deviate) {
            std::sort(values.begin(), values.end(), drl::val_greater);

            // calculate R_T
            drl::tValues value = values.front();
            vertex_incentive_deviate_map[vertex] = std::max(0.0, value.second - reward);
        }

        // retrieve new action
        drl::PAttr attr(0.0, entry->getReward(), entry->getArrival(),
                        drl::PolicyContext::learning());
        newAction = m_policy(entry->getDestination(), values, attr);

#ifndef NDEBUG_EVENTS
        std::cout << "New Action: " << newAction << std::endl;
#endif /* NDEBUG_EVENTS */

        // calculate new q-value
        dnet::Edge oldE = boost::edge(
            vertex, boost::vertex(entry->getOrigin(), m_graph), m_graph).first;
        dnet::Edge newE = boost::edge(
            vertex, boost::vertex(newAction, m_graph), m_graph).first;

        // calculate the target
        drl::RLStateHelper::fillStateVector(newE, m_graph, m_state_representation, m_inputs);
        boost::uint16_t index = (m_outsource) ? (newAction) : (edge_index_map[newE]);
        m_target[0] = reward + m_q_lambda * m_nets[index]->present(m_inputs)[0];
//         m_target[0] = reward + m_q_lambda * edge_q_val_map[newE];

        index = (m_outsource) ? (entry->getOrigin()) : (edge_index_map[oldE]);

#ifndef NDEBUG_EVENTS
        std::cout << "Train NN for edge: " << index << std::endl;
#endif /* NDEBUG_EVENTS */

        // train NN towards the target
        m_trainings[index]->train(m_target);

        // fill the state vector of the current edge
        drl::RLStateHelper::fillStateVector(oldE, m_graph, m_state_representation, m_inputs);

#ifndef NDEBUG_EVENTS
        std::cout << "Old Q-value: " << edge_q_val_map[oldE] << ", new Q-value: " << m_nets[index]->present(m_inputs)[0] << std::endl;
#endif /* NDEBUG_EVENTS */

        // update the q_value
        edge_q_val_map[oldE] = m_nets[index]->present(m_inputs)[0];

        if (m_nn_loss_serialise) {
            if (m_outsource) {
                vertex_nn_loss_map[boost::vertex(entry->getOrigin(), m_graph)] = m_objectives[entry->getOrigin()]->error();
                vertex_nn_loss_cv_map[boost::vertex(entry->getOrigin(), m_graph)] = sqrt(m_objectives[entry->getOrigin()]->error()) / edge_q_val_map[oldE];
            } else {
                edge_nn_loss_map[oldE] = m_objectives[edge_index_map[oldE]]->error();
                edge_nn_loss_cv_map[oldE] = sqrt(m_objectives[edge_index_map[oldE]]->error()) / edge_q_val_map[oldE];
            }
        }
    } else if (degree == 1) {
        dnet::Edge e = *(boost::out_edges(vertex, m_graph).first);
        newAction = vertex_index_map[boost::target(e, m_graph)];

        // calculate q-value
        boost::uint16_t index = (m_outsource) ? (newAction) : (edge_index_map[e]);
        qStatsSA[index].push(reward);

#ifndef NDEBUG_EVENTS
        std::cout << "Old Q-value: " << edge_q_val_map[e]
                  << ", new Q-value: " << qStatsSA[index].mean() << std::endl;
        std::cout << "Deterministic Target: " << newAction << std::endl;
#endif /* NDEBUG_EVENTS */

        edge_q_val_map[e] = qStatsSA[index].mean();
    }

    // set new action
    vertex_next_action_map[vertex] = newAction;
}


}
}
