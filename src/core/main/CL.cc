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

/** @file CL.cc
 * Implementation of the command-line parsing of the main routine for DES.
 *
 * @author Dominik Dahlem
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include <boost/cstdint.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>
namespace po = boost::program_options;

#include "WEvonet.hh"
namespace dnet = des::network;

#include "CL.hh"



namespace des
{
namespace core
{


CL::CL()
{
    opt_desc = tOptDescSP(new po::options_description());

    // Declare the supported options.
    po::options_description opt_general("General Configuration");
    opt_general.add_options()
        (HELP.c_str(), "produce help message")
        (VERS.c_str(), "show the version")
        ;

    po::options_description opt_app("I/O Configuration");
    opt_app.add_options()
        (GRAPH.c_str(), po::value <std::string>(), "set the graph for the event simulator.")
        (SEEDS.c_str(), po::value <std::string>(), "set the seeds for the event simulator.")
        (RESULTS.c_str(), po::value <std::string>()->default_value("./results"), "set directory for the results of the event simulator.")
        (LOG_GRAPHS.c_str(), po::value <bool>()->default_value(true), "log the graphs.")
        (LOG_GRAPH_RATE.c_str(), po::value <boost::int32_t>()->default_value(0), "set the graph generation rate.")
        (LOG_EVENTS.c_str(), po::value <bool>()->default_value(true), "log the events.")
        ;

    po::options_description opt_des("Simulation Configuration");
    opt_des.add_options()
        (STOPTIME.c_str(), po::value <double>()->default_value(100.0), "set the stop time of the event simulator.")
        (GENERATIONS.c_str(), po::value <boost::int32_t>()->default_value(-1), "set the number of generations for the event simulator.")
        (SIZE.c_str(), po::value<int>()->default_value(10), "set the size of the network")
        (GENERATOR.c_str(), po::value <int>()->default_value(1), "Network generator (1=BBV, 2=Erdoes-Renyi).")
        (MAX_ARRIVAL.c_str(), po::value <double>()->default_value(1.0), "Max. arrival rate.")
        (BOOST_ARRIVAL.c_str(), po::value <double>()->default_value(1.0), "Boost the arrival rate.")
        (BOOST_EDGE.c_str(), po::value <double>()->default_value(1.0), "Boost the edge weight.")
        (MAX_EDGES.c_str(), po::value<boost::uint32_t>()->default_value(UINT_MAX), "set the maximum number of edges to connect a new vertex")
        (ADD_SIM.c_str(), po::value<std::string>()->default_value(std::string("")), "add to existing experiments")
        (SIMULATION_DIR.c_str(), po::value<std::string>()->default_value(std::string("")), "simulation results directory")
        ;

    po::options_description opt_soc("Social Network Configuration");
    opt_soc.add_options()
        (WEIGHT_FIXED.c_str(), po::value <double>()->default_value(-1.0), "fix the edge weights (-1=dont't fix).")
        ;

    po::options_description opt_rand("Erdoes-Renyi Network Configuration");
    opt_rand.add_options()
        (EDGE_PROB.c_str(), po::value <double>()->default_value(0.05), "probability of having an edge (u,v).")
        ;
    po::options_description opt_ci("Confidence Interval Configuration");
    opt_ci.add_options()
        (WITH_CI.c_str(), po::value <bool>()->default_value(false), "Obtain a specified experiment precision.")
        (REPLICATIONS.c_str(), po::value <boost::uint16_t>()->default_value(2), "Initial number of replications.")
        (ALPHA.c_str(), po::value <double>()->default_value(0.05), "100(1-alpha) percent confidence interval.")
        (ERROR.c_str(), po::value <double>()->default_value(0.1), "Relative error threshold for the CI calculations.")
        ;

    po::options_description opt_lhs("Latin Hypercube Configuration");
    opt_lhs.add_options()
        (WITH_LHS.c_str(), po::value <bool>()->default_value(false), "Perform LHS sampling for the experiments.")
        (LHS_OPTIMAL.c_str(), po::value <bool>()->default_value(false), "Perform optimal LHS sampling.")
        (LHS_R.c_str(), po::value <boost::uint16_t>()->default_value(100), "set the number of optimisation iterations for oLHS.")
        (SIMULATIONS.c_str(), po::value <boost::uint32_t>()->default_value(1), "set the number of simulations to run.")
        (MINSIZE.c_str(), po::value <boost::uint16_t>()->default_value(
            std::numeric_limits<boost::uint16_t>::max()), "set the min. network size.")
        (MAXSIZE.c_str(), po::value <boost::uint16_t>()->default_value(
            std::numeric_limits<boost::uint16_t>::max()), "set the max. network size.")
        (MIN_MAX_EDGES.c_str(), po::value <boost::uint32_t>()->default_value(
            std::numeric_limits<boost::uint32_t>::max()), "set the min. maximum number of edges.")
        (MAX_MAX_EDGES.c_str(), po::value <boost::uint32_t>()->default_value(
            std::numeric_limits<boost::uint32_t>::max()), "set the max. maximum number of edges.")
        (MIN_EDGE_PROB.c_str(), po::value <double>()->default_value(
            std::numeric_limits<double>::max()), "set the min. probability of having edge (u,v).")
        (MAX_EDGE_PROB.c_str(), po::value <double>()->default_value(
            std::numeric_limits<double>::max()), "set the max. probability of having edge (u,v).")
        (MIN_BOOST_ARRIVAL.c_str(), po::value <double>()->default_value(
            std::numeric_limits<double>::max()), "set the min. rate to boost the arrival rate.")
        (MAX_BOOST_ARRIVAL.c_str(), po::value <double>()->default_value(
            std::numeric_limits<double>::max()), "set the max. rate to boost the arrival rate.")
        (MIN_BOOST_EDGE.c_str(), po::value <double>()->default_value(
            std::numeric_limits<double>::max()), "set the min. rate to boost the edge weight.")
        (MAX_BOOST_EDGE.c_str(), po::value <double>()->default_value(
            std::numeric_limits<double>::max()), "set the max. rate to boost the edge weight.")
        (MIN_RL_Q_ALPHA.c_str(), po::value <double>()->default_value(
            std::numeric_limits<double>::max()), "set the min. learning rate for q-learning.")
        (MAX_RL_Q_ALPHA.c_str(), po::value <double>()->default_value(
            std::numeric_limits<double>::max()), "set the max. learning rate for q-learning.")
        (MIN_RL_Q_LAMBDA.c_str(), po::value <double>()->default_value(
            std::numeric_limits<double>::max()), "set the min. discount rate for q-learning.")
        (MAX_RL_Q_LAMBDA.c_str(), po::value <double>()->default_value(
            std::numeric_limits<double>::max()), "set the max. discount rate for q-learning.")
        (MIN_RL_POLICY_EPSILON.c_str(), po::value <double>()->default_value(
            std::numeric_limits<double>::max()), "set the min. epsilon for epsilon-greedy policy.")
        (MAX_RL_POLICY_EPSILON.c_str(), po::value <double>()->default_value(
            std::numeric_limits<double>::max()), "set the max. epsilon for epsilon-greedy policy.")
        ;

    po::options_description opt_rl("RL Configuration");
    opt_rl.add_options()
        (RL.c_str(), po::value <bool>()->default_value(false), "Enable Reinforcement Learning.")
        (RL_RESPONSE_ALPHA.c_str(), po::value <std::string>(), "Reward Levels for response Time.")
        (RL_RESPONSE_REWARD.c_str(), po::value <std::string>(), "Reward Scalars above respective levels.")
        (RL_Q_ALPHA.c_str(), po::value <double>()->default_value(0.1), "Learning Rate.")
        (RL_Q_LAMBDA.c_str(), po::value <double>()->default_value(0.1), "Action-value Rate.")
        (RL_POLICY.c_str(), po::value <boost::uint16_t>()->default_value(1), "Policy (1=Epsilon-Greedy, 2=Boltzmann).")
        (CL_RL_STATE_IDS.c_str(), po::value <std::string>()->default_value(""), "State representation.")
        (CL_NN_HIDDENLAYER_NEURONS.c_str(), po::value <boost::uint16_t>()->default_value(5), "Hidden number of Neurons.")
        (CL_NN_LOSS_POLICY.c_str(), po::value <boost::uint16_t>()->default_value(1), "Loss Policy (1=default, 2=sliding window).")
        (CL_NN_WINDOW.c_str(), po::value <boost::uint16_t>()->default_value(100), "Sliding Window Size.")
        (CL_NN_BRENT_ITER.c_str(), po::value <boost::uint16_t>()->default_value(500), "Brent Iterations in the CG Training.")
        (CL_NN_MOMENTUM.c_str(), po::value <double>()->default_value(1.0), "Momentum for the Backpropagation Training.")
        (CL_NN_CG.c_str(), po::value <bool>()->default_value(true), "Training method (1=CG, 2=BP).")
       ;

    po::options_description opt_rl_policy_epsilon("RL Epsilon Policy Configuration");
    opt_rl_policy_epsilon.add_options()
        (RL_POLICY_EPSILON.c_str(), po::value <double>()->default_value(
            0.1), "Epsilon-greedy value.")
        ;

    po::options_description opt_rl_policy_boltzmann("RL Boltzmann Policy Configuration");
    opt_rl_policy_boltzmann.add_options()
        (RL_POLICY_BOLTZMANN_T.c_str(), po::value <double>()->default_value(
            100.0), "Temperator for Boltzmann policy.")
        ;

    po::options_description opt_expert("Expert Metrics Configuration");
    opt_expert.add_options()
        (CL_EXPERT_NORMAL.c_str(), po::value <bool>()->default_value(false), "Set the normal expert metric.")
        (CL_EXPERT_ABSOLUTE.c_str(), po::value <bool>()->default_value(false), "Set the absolute expert metric.")
        (CL_EXPERT_POSITIVE.c_str(), po::value <bool>()->default_value(false), "Set the positive expert metric.")
        (CL_EXPERT_NEGATIVE.c_str(), po::value <bool>()->default_value(false), "Set the negative expert metric.")
        ;

    po::options_description opt_debug("Debug Configuration");
    opt_debug.add_options()
        (TRACE.c_str(), po::value <bool>()->default_value(false), "Set debugging.")
        (VERTEX.c_str(), po::value <boost::int32_t>(), "The source vertex to trace the event for.")
        ;

    opt_desc->add(opt_general);
    opt_desc->add(opt_app);
    opt_desc->add(opt_des);
    opt_desc->add(opt_soc);
    opt_desc->add(opt_rand);
    opt_desc->add(opt_ci);
    opt_desc->add(opt_lhs);
    opt_desc->add(opt_rl);
    opt_desc->add(opt_rl_policy_epsilon);
    opt_desc->add(opt_rl_policy_boltzmann);
    opt_desc->add(opt_expert);
    opt_desc->add(opt_debug);
}


int CL::parse(int argc, char *argv[], tDesArgsSP desArgs)
{
    po::variables_map vm;

    po::store(po::parse_command_line(argc, argv, (*opt_desc.get())), vm);
    po::notify(vm);

    if (vm.count(HELP)) {
        std::cout << (*opt_desc.get()) << std::endl;
        return EXIT_FAILURE;
    }

    if (vm.count(VERS)) {
        std::cout << argv[0] << " " << PACKAGE_VERSION << std::endl;
        std::cout << PACKAGE_NAME << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "1) I/O Configuration" << std::endl;

    if (vm.count(GRAPH.c_str())) {
        desArgs->graph_filename = vm[GRAPH.c_str()].as <std::string>();
        if (fs::exists(desArgs->graph_filename)) {
            std::cout << "Read graph from filename "
                      << desArgs->graph_filename << "." << std::endl;
        } else {
            std::cerr << "Error: Graph file " << desArgs->graph_filename
                      << " does not exist!" << std::endl;

            return EXIT_FAILURE;
        }
    }

    if (vm.count(SEEDS.c_str())) {
        desArgs->seeds_filename = vm[SEEDS.c_str()].as <std::string>();
        if (fs::exists(desArgs->seeds_filename)) {
            std::cout << "Read seeds from filename "
                      << desArgs->seeds_filename << "." << std::endl;
        } else {
            std::cerr << "Error: Seeds file " << desArgs->seeds_filename
                      << " does not exist!" << std::endl;

            return EXIT_FAILURE;
        }
    }
#ifdef HAVE_MPI
    else {
        std::cerr << "Require a seeds file!" << std::endl;
        return EXIT_FAILURE;
    }
#endif /* HAVE_MPI */

    if (vm.count(RESULTS.c_str())) {
        desArgs->results_dir = vm[RESULTS.c_str()].as <std::string>();
    }
    std::cout << "Set the results directory "
              << desArgs->results_dir << "." << std::endl;

    if (vm.count(LOG_GRAPHS.c_str())) {
        desArgs->log_graphs = vm[LOG_GRAPHS.c_str()].as <bool>();
    }
    std::cout << "Log the graphs "
              << desArgs->log_graphs << "." << std::endl;

    if (vm.count(LOG_GRAPH_RATE.c_str())) {
        desArgs->graph_rate = vm[LOG_GRAPH_RATE.c_str()].as <boost::int32_t>();
    }
    std::cout << "Set the graph generation rate to "
              << desArgs->graph_rate << "." << std::endl;

    if (vm.count(LOG_EVENTS.c_str())) {
        desArgs->log_events = vm[LOG_EVENTS.c_str()].as <bool>();
    }
    std::cout << "Log the events "
              << desArgs->log_events << "." << std::endl;


    std::cout << std::endl << "2) Simulation Configuration" << std::endl;
    if (vm.count(GENERATOR.c_str())) {
        desArgs->net_gen = vm[GENERATOR.c_str()].as <int>();
    }
    std::cout << "Generate network type "
              << desArgs->net_gen << "." << std::endl;

    if (vm.count(ADD_SIM.c_str())) {
        desArgs->add_sim = vm[ADD_SIM.c_str()].as <std::string>();
    }
    std::cout << "Add to existing experiments "
              << desArgs->add_sim << "." << std::endl;

    if (vm.count(SIMULATION_DIR.c_str())) {
        desArgs->sim_dir = vm[SIMULATION_DIR.c_str()].as <std::string>();
    }
    std::cout << "Simulation results directory "
              << desArgs->sim_dir << "." << std::endl;

    if (vm.count(STOPTIME.c_str())) {
        desArgs->stop_time = vm[STOPTIME.c_str()].as <double>();
    }
    std::cout << "Stopping time set to " << desArgs->stop_time << "." << std::endl;

    if (vm.count(MAX_ARRIVAL.c_str())) {
        desArgs->max_arrival = vm[MAX_ARRIVAL.c_str()].as <double>();
    }
    std::cout << "Max. arrival rate set to " << desArgs->max_arrival << "." << std::endl;

    if (vm.count(BOOST_ARRIVAL.c_str())) {
        desArgs->boost_arrival = vm[BOOST_ARRIVAL.c_str()].as <double>();
    }
    std::cout << "Boost the arrival rate " << desArgs->boost_arrival << "." << std::endl;

    if (vm.count(BOOST_EDGE.c_str())) {
        desArgs->boost_edge = vm[BOOST_EDGE.c_str()].as <double>();
    }
    std::cout << "Boost the edge weight " << desArgs->boost_edge << "." << std::endl;

    if (vm.count(GENERATIONS.c_str())) {
        desArgs->generations = vm[GENERATIONS.c_str()].as <boost::int32_t>();
        std::cout << "Number of generations set to " << desArgs->generations << "." << std::endl;
    } else {
        std::cout << "No generations." << std::endl;
    }

    if (vm.count(SIZE.c_str())) {
        desArgs->net_size = vm[SIZE.c_str()].as<int>();
    }
    std::cout << "Size of network set to "
              << desArgs->net_size << "." << std::endl;

    if (vm.count(MAX_EDGES.c_str())) {
        desArgs->max_edges = vm[MAX_EDGES.c_str()].as<boost::uint32_t>();
    }
    std::cout << "Maximum number of edges is set to "
              << desArgs->max_edges << "." << std::endl;

    if (desArgs->net_gen == 1) {
        std::cout << std::endl << "3) Social Network Configuration" << std::endl;
        if (vm.count(WEIGHT_FIXED.c_str())) {
            desArgs->edge_fixed = vm[WEIGHT_FIXED.c_str()].as <double>();
        }
        std::cout << "Fix the edge weight at "
                  << desArgs->edge_fixed << "." << std::endl;
    } else {
        std::cout << std::endl << "3) Erdoes-Renyi Network Configuration" << std::endl;
        if (vm.count(EDGE_PROB.c_str())) {
            desArgs->edge_prob = vm[EDGE_PROB.c_str()].as <double>();
        }
        std::cout << "The probability of having in edge (u,v) is "
                  << desArgs->edge_prob << " (only for ER graphs)." << std::endl;
    }


    std::cout << std::endl << "4) Reinforcement Learning Configuration" << std::endl;
    if (vm.count(RL.c_str())) {
        desArgs->rl = vm[RL.c_str()].as <bool>();
    }
    std::cout << "RL enabled: " << desArgs->rl << std::endl;
    if (desArgs->rl) {
        if (vm.count(RL_Q_ALPHA.c_str())) {
            desArgs->rl_q_alpha = vm[RL_Q_ALPHA.c_str()].as <double>();
        }
        std::cout << "Learning Rate: " << desArgs->rl_q_alpha << "." << std::endl;
        if (vm.count(RL_Q_LAMBDA.c_str())) {
            desArgs->rl_q_lambda = vm[RL_Q_LAMBDA.c_str()].as <double>();
        }
        std::cout << "Action-Value Discount Rate: " << desArgs->rl_q_lambda << "." << std::endl;
        if (vm.count(RL_POLICY.c_str())) {
            desArgs->rl_policy = vm[RL_POLICY.c_str()].as <boost::uint16_t>();
        }
        std::cout << "RL Selection Policy: " << desArgs->rl_policy << "." << std::endl;

        if (desArgs->rl_policy == 1) {
            if (vm.count(RL_POLICY_EPSILON.c_str())) {
                desArgs->rl_policy_epsilon = vm[RL_POLICY_EPSILON.c_str()].as <double>();
            }
            std::cout << "RL Epsilon: " << desArgs->rl_policy_epsilon << "." << std::endl;
        } else if (desArgs->rl_policy == 2) {
            if (vm.count(RL_POLICY_BOLTZMANN_T.c_str())) {
                desArgs->rl_policy_boltzmann_t = vm[RL_POLICY_BOLTZMANN_T.c_str()].as <double>();
            }
            std::cout << "RL Boltzmann T: " << desArgs->rl_policy_boltzmann_t << "." << std::endl;
        }

        if (vm.count(CL_NN_LOSS_POLICY.c_str())) {
            desArgs->nn_loss_policy = vm[CL_NN_LOSS_POLICY.c_str()].as <boost::uint16_t>();
        }
        std::cout << "NN Loss Policy: " << desArgs->nn_loss_policy << "." << std::endl;

        if (vm.count(CL_NN_WINDOW.c_str())) {
            desArgs->nn_window = vm[CL_NN_WINDOW.c_str()].as <boost::uint16_t>();
        }
        std::cout << "NN sliding window size: " << desArgs->nn_window << "." << std::endl;

        if (vm.count(CL_NN_BRENT_ITER.c_str())) {
            desArgs->nn_brent_iter = vm[CL_NN_BRENT_ITER.c_str()].as <boost::uint16_t>();
        }
        std::cout << "NN Brent Iterations: " << desArgs->nn_brent_iter << "." << std::endl;

        if (vm.count(CL_NN_MOMENTUM.c_str())) {
            desArgs->nn_momentum = vm[CL_NN_MOMENTUM.c_str()].as <double>();
        }
        std::cout << "NN BP Momentum: " << desArgs->nn_momentum << "." << std::endl;

        if (vm.count(CL_NN_CG.c_str())) {
            desArgs->nn_cg = vm[CL_NN_CG.c_str()].as <bool>();
        }
        std::cout << "NN Training Method: " << desArgs->nn_cg << "." << std::endl;

        // parse the command-line
        std::string ids;
        if (vm.count(CL_RL_STATE_IDS.c_str())) {
            ids = vm[CL_RL_STATE_IDS.c_str()].as <std::string>();
        }

        // tokenize and store in vector as ints
        boost::tokenizer<> tok(ids);
        BOOST_FOREACH(std::string id, tok)
        {
            desArgs->rl_state_representation.push_back(boost::lexical_cast<int>(id));
        }

        std::cout << "The " << desArgs->rl_state_representation.size() << " state IDs are: " << std::endl;
        BOOST_FOREACH(int id, desArgs->rl_state_representation)
        {
            std::cout << id << " ";
        }
        std::cout << std::endl;

        if (vm.count(CL_NN_HIDDENLAYER_NEURONS.c_str())) {
            desArgs->nn_hidden_neurons = vm[CL_NN_HIDDENLAYER_NEURONS.c_str()].as <boost::uint16_t>();
        }
        std::cout << "Hidden Neurons: " << desArgs->nn_hidden_neurons << "." << std::endl;
    }

    std::cout << std::endl << "5) Confidence Interval Configuration" << std::endl;
    if (vm.count(WITH_CI.c_str())) {
        desArgs->confidence = vm[WITH_CI.c_str()].as <bool>();
    }
    std::cout << "Confidence Interval enabled: " << desArgs->confidence << std::endl;

    if (desArgs->confidence == 1) {
        if (vm.count(REPLICATIONS.c_str())) {
            desArgs->replications = vm[REPLICATIONS.c_str()].as <boost::uint16_t>();
        }
        std::cout << "Number of replications: " << desArgs->replications << std::endl;

        if (vm.count(ALPHA.c_str())) {
            desArgs->alpha = vm[ALPHA.c_str()].as <double>();
        }
        std::cout << "Confidence interval (in %): " << 100 * (1 - desArgs->alpha) << std::endl;

        if (vm.count(ERROR.c_str())) {
            desArgs->error = vm[ERROR.c_str()].as <double>();
        }
        std::cout << "Reliative error (in %): " << 100 * (desArgs->error) << std::endl;
    }


    std::cout << std::endl << "6) Latin Hypercube Configuration" << std::endl;
    if (vm.count(WITH_LHS.c_str())) {
        desArgs->lhs = vm[WITH_LHS.c_str()].as <bool>();
    }
    std::cout << "LHS enabled: " << desArgs->lhs << std::endl;

    if (desArgs->lhs == 1) {
        if (vm.count(LHS_OPTIMAL.c_str())) {
            desArgs->lhs_optimal = vm[LHS_OPTIMAL.c_str()].as <bool>();
        }
        std::cout << "oLHS enabled: " << desArgs->lhs_optimal << std::endl;

        if (vm.count(SIMULATIONS.c_str())) {
            desArgs->simulations = vm[SIMULATIONS.c_str()].as <boost::uint32_t>();
        }
        std::cout << "Number of simulations set to " << desArgs->simulations << "." << std::endl;

        if (vm.count(LHS_R.c_str())) {
            desArgs->lhs_r = vm[LHS_R.c_str()].as <boost::uint16_t>();
        }
        std::cout << "Number of oLHS iterations set to " << desArgs->lhs_r << "." << std::endl;

        if (vm.count(MIN_BOOST_ARRIVAL.c_str())) {
            desArgs->min_boost_arrival = vm[MIN_BOOST_ARRIVAL.c_str()].as <double>();
        }
        std::cout << "Min. boost rate for the arrival rate " << desArgs->min_boost_arrival << "." << std::endl;

        if (vm.count(MIN_BOOST_EDGE.c_str())) {
            desArgs->min_boost_edge = vm[MIN_BOOST_EDGE.c_str()].as <double>();
        }
        std::cout << "Min. boost rate for the edge weight " << desArgs->min_boost_edge << "." << std::endl;

        if (vm.count(MAX_BOOST_ARRIVAL.c_str())) {
            desArgs->max_boost_arrival = vm[MAX_BOOST_ARRIVAL.c_str()].as <double>();
        }
        std::cout << "Max. boost rate for the arrival rate " << desArgs->max_boost_arrival << "." << std::endl;

        if (vm.count(MAX_BOOST_EDGE.c_str())) {
            desArgs->max_boost_edge = vm[MAX_BOOST_EDGE.c_str()].as <double>();
        }
        std::cout << "Max. boost rate for the edge weight " << desArgs->max_boost_edge << "." << std::endl;

        if (vm.count(MINSIZE.c_str())) {
            desArgs->min_size = vm[MINSIZE.c_str()].as <boost::uint16_t>();
        }
        std::cout << "Minimum network size " << desArgs->min_size << "." << std::endl;

        if (vm.count(MAXSIZE.c_str())) {
            desArgs->max_size = vm[MAXSIZE.c_str()].as <boost::uint16_t>();
        }
        std::cout << "Maximum network size " << desArgs->max_size << "." << std::endl;

        if (vm.count(MIN_MAX_EDGES.c_str())) {
            desArgs->min_max_edges = vm[MIN_MAX_EDGES.c_str()].as <boost::uint32_t>();
        }
        std::cout << "Minimum max. number of edges set to " << desArgs->min_max_edges << "." << std::endl;

        if (vm.count(MAX_MAX_EDGES.c_str())) {
            desArgs->max_max_edges = vm[MAX_MAX_EDGES.c_str()].as <boost::uint32_t>();
        }
        std::cout << "Maximum max. number of edges set to " << desArgs->max_max_edges << "." << std::endl;

        if (vm.count(MIN_EDGE_PROB.c_str())) {
            desArgs->min_edge_prob = vm[MIN_EDGE_PROB.c_str()].as <double>();
        }
        std::cout << "Minimum probability of having in edge (u,v) set to " << desArgs->min_edge_prob << "." << std::endl;

        if (vm.count(MAX_EDGE_PROB.c_str())) {
            desArgs->max_edge_prob = vm[MAX_EDGE_PROB.c_str()].as <double>();
        }
        std::cout << "Maximum probability of having in edge (u,v) set to " << desArgs->max_edge_prob << "." << std::endl;

        if (vm.count(MIN_RL_Q_ALPHA.c_str())) {
            desArgs->min_rl_q_alpha = vm[MIN_RL_Q_ALPHA.c_str()].as <double>();
        }
        std::cout << "Minimum RL learning rate set to " << desArgs->min_rl_q_alpha << "." << std::endl;

        if (vm.count(MAX_RL_Q_ALPHA.c_str())) {
            desArgs->max_rl_q_alpha = vm[MAX_RL_Q_ALPHA.c_str()].as <double>();
        }
        std::cout << "Maximum RL learning rate set to " << desArgs->max_rl_q_alpha << "." << std::endl;

        if (vm.count(MIN_RL_Q_LAMBDA.c_str())) {
            desArgs->min_rl_q_lambda = vm[MIN_RL_Q_LAMBDA.c_str()].as <double>();
        }
        std::cout << "Minimum RL discount rate set to " << desArgs->min_rl_q_lambda << "." << std::endl;

        if (vm.count(MAX_RL_Q_LAMBDA.c_str())) {
            desArgs->max_rl_q_lambda = vm[MAX_RL_Q_LAMBDA.c_str()].as <double>();
        }
        std::cout << "Maximum RL discount rate set to " << desArgs->max_rl_q_lambda << "." << std::endl;

        if (vm.count(MIN_RL_POLICY_EPSILON.c_str())) {
            desArgs->min_rl_policy_epsilon = vm[MIN_RL_POLICY_EPSILON.c_str()].as <double>();
        }
        std::cout << "Minimum epsilon set to " << desArgs->min_rl_policy_epsilon << "." << std::endl;

        if (vm.count(MAX_RL_POLICY_EPSILON.c_str())) {
            desArgs->max_rl_policy_epsilon = vm[MAX_RL_POLICY_EPSILON.c_str()].as <double>();
        }
        std::cout << "Maximum epsilon set to " << desArgs->max_rl_policy_epsilon << "." << std::endl;
    }

    std::cout << std::endl << "7) Expert Metrics Configuration" << std::endl;
    if (vm.count(CL_EXPERT_NORMAL.c_str())) {
        desArgs->expert_normal = vm[CL_EXPERT_NORMAL.c_str()].as <bool>();
    }
    std::cout << "Expert Normal enabled: " << desArgs->expert_normal << std::endl;

    if (vm.count(CL_EXPERT_ABSOLUTE.c_str())) {
        desArgs->expert_absolute = vm[CL_EXPERT_ABSOLUTE.c_str()].as <bool>();
    }
    std::cout << "Expert Absolute enabled: " << desArgs->expert_absolute << std::endl;

    if (vm.count(CL_EXPERT_POSITIVE.c_str())) {
        desArgs->expert_positive = vm[CL_EXPERT_POSITIVE.c_str()].as <bool>();
    }
    std::cout << "Expert Positive enabled: " << desArgs->expert_positive << std::endl;

    if (vm.count(CL_EXPERT_NEGATIVE.c_str())) {
        desArgs->expert_negative = vm[CL_EXPERT_NEGATIVE.c_str()].as <bool>();
    }
    std::cout << "Expert Negative enabled: " << desArgs->expert_negative << std::endl;

    std::cout << std::endl << "8) Output Files" << std::endl;
    desArgs->events_unprocessed = "events_unprocessed.dat";
    desArgs->events_processed = "events_processed.dat";

    std::cout << "Unprocessed events\t" << desArgs->events_unprocessed << std::endl;
    std::cout << "Processed events\t" << desArgs->events_processed << std::endl;

    if (vm.count(TRACE.c_str())) {
        desArgs->trace_event = vm[TRACE.c_str()].as <bool>();
    }
    if (desArgs->trace_event) {
        std::cout << std::endl << "9) Debug Configuration" << std::endl;
        if (vm.count(VERTEX.c_str())) {
            desArgs->vertex = vm[VERTEX.c_str()].as <boost::int32_t>();
            std::cout << std::endl << "Trace vertex " << desArgs->vertex << "." << std::endl;
        } else {
            std::cout << "A vertex needs to be specified to trace the event." << std::endl;
            return EXIT_FAILURE;
        }
    }

    std::cout << "******************************" << std::endl << std::endl;

    verify(desArgs);

    return EXIT_SUCCESS;
}

void CL::verify(tDesArgsSP desArgs)
{
    // switch on replication management via specified confidence bands
    if (desArgs->lhs) {
        desArgs->confidence = true;
        std::cout << "Switch on replication management via specified confidence bands." << std::endl;
    }
}


}
}
