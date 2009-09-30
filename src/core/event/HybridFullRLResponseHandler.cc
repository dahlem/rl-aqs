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

/** @file FullRLResponseHandler.cc
 * Implementation of a basic FullRLResponse handler.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef NDEBUG_EVENTS
# include <iostream>
#endif /* NDEBUG_EVENTS */

#include <boost/foreach.hpp>

#include <gsl/gsl_math.h>

#include "Entry.hh"
namespace dcommon = des::common;

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "Policy.hh"
#include "EpsilonGreedy.hh"
#include "RLStateHelper.hh"
namespace drl = des::rl;

#include "events.hh"
#include "AckEvent.hh"
#include "HybridFullRLResponseHandler.hh"


namespace des
{
namespace core
{


HybridFullRLResponseHandler::HybridFullRLResponseHandler(dnet::Graph &p_graph,
                                                         double p_q_alpha,
                                                         double p_q_lambda,
                                                         drl::Policy &p_policy,
                                                         std::vector<int> &p_state_representation,
                                                         boost::uint16_t p_hidden_neurons,
                                                         boost::int32_t p_uniform_rng_index,
                                                         bool p_cg,
                                                         boost::uint16_t p_loss_policy,
                                                         boost::uint16_t p_window,
                                                         boost::uint16_t p_brent_iter,
                                                         double p_momentum,
                                                         boost::uint16_t p_randomIter)
    : m_graph(p_graph), m_q_alpha(p_q_alpha), m_q_lambda(p_q_lambda), m_policy(p_policy),
      m_state_representation(p_state_representation), m_hidden_neurons(p_hidden_neurons),
      m_uniform_rng_index(p_uniform_rng_index), m_randomIter(p_randomIter),
      qStatsSA(new dstats::OnlineStats[boost::num_edges(m_graph)])
{
    vertex_next_action_map = get(vertex_next_action, m_graph);
    vertex_index_map = get(boost::vertex_index, m_graph);
    edge_q_val_map = get(edge_q_val, m_graph);
    edge_index_map = get(edge_eindex, m_graph);

    // init the neural network for each edge
    for (boost::uint16_t i = 0; i < boost::num_edges(m_graph); ++i) {
        // create the neural network for edge i
        FFNetSP net = dnnet::NNetFactory::createNNet<dnnet::HTangent, dnnet::Identity>
            (p_state_representation.size(), p_hidden_neurons, 1, m_uniform_rng_index);

        ObjMseSP mse;
        if (p_loss_policy == 1) {
            mse = dnnet::NNetFactory::createDefaultMSEObjective<dnnet::HTangent, dnnet::Identity>(net);
        } else if (p_loss_policy == 2) {
            mse = dnnet::NNetFactory::createSlidingWindowMSEObjective<dnnet::HTangent, dnnet::Identity>(net, p_window);
        }

        dnnet::TrainingSP training;
        if (p_cg) {
            training = dnnet::NNetFactory::createConjugateGradientTraining<dnnet::HTangent, dnnet::Identity>(
                net, mse, m_q_alpha, 1e-6, p_brent_iter);
        } else {
            training = dnnet::NNetFactory::createBackpropagationTraining<dnnet::HTangent, dnnet::Identity>(
                net, mse, m_q_alpha, p_momentum, 1e-6);
        }

        // store the neural network, objective function, and training method into vectors
        m_nets.push_back(net);
        m_objectives.push_back(mse);
        m_trainings.push_back(training);
    }

    // allocate memory here for the input vector and the target
    m_inputs = DoubleSA(new double[p_state_representation.size()]);
    m_target = DoubleSA(new double[1]);
}


HybridFullRLResponseHandler::~HybridFullRLResponseHandler()
{
    m_trainings.clear();
    m_objectives.clear();
    m_nets.clear();
}


void HybridFullRLResponseHandler::update(AckEvent *subject)
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
    double reward = 0.0;
    reward = (gsl_fcmp(entry->topArrival(), entry->getArrival(), 1e-9) == 0)
        ? (0.0)
        : (entry->topArrival() - entry->getArrival());

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
            int edge_index = edge_index_map[e];

            value.first = target_vertex;

            // only use the trained value, if the warmup phase is over
            if (qStatsSA[edge_index].getNumValues() < m_randomIter) {
                value.second = qStatsSA[edge_index].mean();
            } else {
                // collect the input vector
                drl::RLStateHelper::fillStateVector(e, m_graph, m_state_representation, m_inputs);
                // calculate the neural network prediction given current state
                value.second = m_nets[edge_index]->present(m_inputs)[0];
            }

#ifndef NDEBUG_EVENTS
            std::cout << "Action-Value Pair: " << value.first << ", " << value.second << std::endl;
#endif /* NDEBUG_EVENTS */
            values[count++] = value;
        }

        drl::PAttr attr(0.0, drl::PolicyContext::learning());
        newAction = m_policy(entry->getDestination(), values, attr);

#ifndef NDEBUG_EVENTS
        std::cout << "New Action: " << newAction << std::endl;
#endif /* NDEBUG_EVENTS */

        // calculate new q-value
        dnet::Edge oldE = boost::edge(
            vertex, boost::vertex(entry->getOrigin(), m_graph), m_graph).first;
        dnet::Edge newE = boost::edge(
            vertex, boost::vertex(newAction, m_graph), m_graph).first;
        int edge_index = edge_index_map[oldE];

        // calculate the online statistics only in the warm-up phase
        if (qStatsSA[edge_index].getNumValues() < m_randomIter) {
            qStatsSA[edge_index].push(reward);
        }

        // calculate the target
        drl::RLStateHelper::fillStateVector(newE, m_graph, m_state_representation, m_inputs);
        m_target[0] = reward + m_q_lambda * m_nets[edge_index_map[newE]]->present(m_inputs)[0];

#ifndef NDEBUG_EVENTS
        std::cout << "Train NN for edge: " << edge_index << std::endl;
#endif /* NDEBUG_EVENTS */

        // train NN towards the target
        m_trainings[edge_index]->train(m_target);

        // use the mean of past q-values for the warm-up phase
        // otherwise use the learned NN value
        double newQ = 0.0;
        if (qStatsSA[edge_index].getNumValues() < m_randomIter) {
            newQ = qStatsSA[edge_index].mean();
        } else {
            // fill the state vector of the current edge
            drl::RLStateHelper::fillStateVector(oldE, m_graph, m_state_representation, m_inputs);
            newQ = m_nets[edge_index]->present(m_inputs)[0];
        }

#ifndef NDEBUG_EVENTS
        std::cout << "Old Q-value: " << edge_q_val_map[oldE] << ", new Q-value: " << newQ << std::endl;
#endif /* NDEBUG_EVENTS */

        // update the q_value
        edge_q_val_map[oldE] = newQ;
    } else if (degree == 1) {
        dnet::Edge e = *(boost::out_edges(vertex, m_graph).first);
        newAction = vertex_index_map[boost::target(e, m_graph)];

        // calculate q-value
        boost::uint16_t edge_index = edge_index_map[e];
        qStatsSA[edge_index].push(reward);

#ifndef NDEBUG_EVENTS
        std::cout << "Old Q-value: " << edge_q_val_map[e]
                  << ", new Q-value: " << qStatsSA[edge_index].mean() << std::endl;
        std::cout << "Deterministic Target: " << newAction << std::endl;
#endif /* NDEBUG_EVENTS */

        edge_q_val_map[e] = qStatsSA[edge_index].mean();
    }

    // set new action
    vertex_next_action_map[vertex] = newAction;
}


}
}