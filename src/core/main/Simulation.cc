// Copyright (C) 2008-2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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

/** @file Simulation.cc
 * Implementation of the black-box simulation class.
 *
 * @author Dominik Dahlem
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#ifdef HAVE_MPI
# include <mpi.h>
#endif /* HAVE_MPI */

#include <iostream>
#include <sstream>
#include <utility>

#include <boost/cstdint.hpp>
#include <boost/foreach.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include <gsl/gsl_randist.h>

#include "AckEvent.hh"
#include "AckHandler.hh"
#include "AdminEvent.hh"
#include "ArrivalEvent.hh"
#include "ArrivalHandler.hh"
#include "Arrivals.hh"
#include "ArrivalsChannel.hh"
#include "BoltzmannPolicy.hh"
#include "CL.hh"
#include "CJYArrivals.hh"
#include "CoefficientVariationHandler.hh"
#include "CognitiveValueHandler.hh"
#include "ConfigChannel.hh"
#include "DefaultResponseHandler.hh"
#include "DepartureEvent.hh"
#include "DepartureHandler.hh"
#include "DesBus.hh"
#include "events.hh"
#include "EventGenerator.hh"
#include "EventProcessor.hh"
#include "ExpectedAverageEventInQueueHandler.hh"
#include "ExpertNormalHandler.hh"
#include "ExpertAbsoluteHandler.hh"
#include "ExpertPositiveHandler.hh"
#include "ExpertNegativeHandler.hh"
#include "FairActionLearner.hh"
#include "FullRLResponseHandler.hh"
#include "GenerateArrivalsHandler.hh"
#include "GenerateArrivalsAdminHandler.hh"
#include "GraphChannel.hh"
#include "GradientPolicyLearner.hh"
#include "HybridFullRLResponseHandler.hh"
#include "LastArrivalEvent.hh"
#include "LastEventHandler.hh"
#include "LeaveHandler.hh"
#include "LogGraphHandler.hh"
#include "NullUnprocessedEventsHandler.hh"
#include "NumEventsHandler.hh"
#include "PostEvent.hh"
#include "PreAnyEvent.hh"
#include "ProcessedEventsHandler.hh"
#include "QueueChannel.hh"
#include "Report.hh"
#include "ResponseStatsHandler.hh"
#include "RLResponseHandler.hh"
#include "RollingSystemStatsAdminHandler.hh"
#include "SerialiseArrivalsHandler.hh"
#include "Simulation.hh"
#include "SystemStatisticsHandler.hh"
#include "UnprocessedEventsHandler.hh"
#include "UtilisationHandler.hh"
#include "WeightedPolicyLearner.hh"

#include "Results.hh"
namespace dio = des::io;

#include "EpsilonGreedy.hh"
#include "OnPolicySelection.hh"
#include "Policy.hh"
#include "RandomSelection.hh"
#include "Selection.hh"
namespace drl = des::rl;

#include "CRN.hh"
#include "Seeds.hh"
namespace dsample = des::sampling;

#include "Entry.hh"
#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "DirectedGraph.hh"
#include "GraphException.hh"
#include "GraphUtil.hh"
namespace dnet = des::network;

#include "Backpropagation.hh"
#include "CL.hh"
#include "ConjugateGradient.hh"
#include "FeedforwardNetwork.hh"
#include "Identity.hh"
#include "HTangent.hh"
#include "Logistic.hh"
#include "MSE.hh"
#include "Statistics.hh"
namespace dnnet = des::nnet;


namespace des
{
namespace core
{

typedef boost::shared_array <boost::int32_t> Int32SA;
typedef boost::shared_ptr<drl::Policy> tPolicySP;
typedef boost::shared_ptr<drl::Selection> tSelectionSP;
typedef boost::shared_ptr<DepartureHandler> tDepartureHandlerSP;
typedef boost::shared_ptr<RLResponseHandler> tRLResponseHandlerSP;
typedef boost::shared_ptr<FullRLResponseHandler> tFullRLResponseHandlerSP;
typedef boost::shared_ptr<HybridFullRLResponseHandler> tHybridFullRLResponseHandlerSP;
typedef boost::shared_ptr<DefaultResponseHandler> tDefaultResponseHandlerSP;
typedef boost::shared_ptr<ExpertNormalHandler> tExpertNormalHandlerSP;
typedef boost::shared_ptr<ExpertAbsoluteHandler> tExpertAbsoluteHandlerSP;
typedef boost::shared_ptr<ExpertPositiveHandler> tExpertPositiveHandlerSP;
typedef boost::shared_ptr<ExpertNegativeHandler> tExpertNegativeHandlerSP;
typedef boost::shared_ptr<ResponseStatsHandler> tResponseStatsHandlerSP;
typedef boost::shared_ptr<CognitiveValueHandler> tCognitiveValueHandlerSP;
typedef boost::shared_ptr<CoefficientVariationHandler> tCoefficientVariationHandlerSP;


Int32SA arrivalCRN(boost::uint16_t p_num_vertices)
{
#ifndef NDEBUG
    std::cout << "Init CRN for arrivals" << std::endl;
    std::cout.flush();
#endif /* NDEBUG */

    Int32SA arrivalCRNs = Int32SA(new boost::int32_t[p_num_vertices]);
    boost::uint32_t seed = 0;

    // init the crn for the arrival events
    for (boost::uint16_t i = 0; i < p_num_vertices; ++i) {
        seed = dsample::Seeds::getInstance().getSeed();
        arrivalCRNs[i] = dsample::CRN::getInstance().init(seed);
    }

    dsample::CRN::getInstance().log(
        arrivalCRNs[0], arrivalCRNs[p_num_vertices - 1], "arrival events");

    return arrivalCRNs;
}


Int32SA serviceCRN(boost::uint16_t p_num_vertices)
{
#ifndef NDEBUG
    std::cout << "Init CRN for servicing" << std::endl;
    std::cout.flush();
#endif /* NDEBUG */

    Int32SA serviceCRNs = Int32SA(new boost::int32_t[p_num_vertices]);
    boost::uint32_t seed = 0;

    // init the crn for the service events
    for (boost::uint16_t i = 0; i < p_num_vertices; ++i) {
        seed = dsample::Seeds::getInstance().getSeed();
        serviceCRNs[i] = dsample::CRN::getInstance().init(seed);
    }

    dsample::CRN::getInstance().log(
        serviceCRNs[0], serviceCRNs[p_num_vertices - 1], "service events");

    return serviceCRNs;
}


Int32SA departureCRN(boost::uint16_t p_num_vertices)
{
#ifndef NDEBUG
    std::cout << "Init CRN for departures" << std::endl;
    std::cout.flush();
#endif /* NDEBUG */

    Int32SA departureCRNs = Int32SA(new boost::int32_t[p_num_vertices]);
    boost::uint32_t seed = 0;

    // init the crn for the departure events
    for (boost::uint16_t i = 0; i < p_num_vertices; ++i) {
        seed = dsample::Seeds::getInstance().getSeed();
        departureCRNs[i] = dsample::CRN::getInstance().init(seed);
    }

    dsample::CRN::getInstance().log(
        departureCRNs[0], departureCRNs[p_num_vertices - 1], "departure events");

    return departureCRNs;
}


dnet::tGraphSP Simulation::createGraph(tDesArgsSP desArgs,
                                       boost::uint16_t net_size, boost::uint16_t max_edges,
                                       double edge_prob, double boost_arrival, double boost_edge)
{
    std::cout << "Generate the graph." << std::endl;
    dsample::tGslRngSP r1, r2, r3;
    boost::int32_t num_edges_rng_index = 0;
    boost::uint32_t seed = 0;
    dnet::tGraphSP graph;

    seed = dsample::Seeds::getInstance().getSeed();
    num_edges_rng_index = dsample::CRN::getInstance().init(seed);
    dsample::CRN::getInstance().log(seed, "number of edges");

    r1 = dsample::CRN::getInstance().get(num_edges_rng_index);

    if (desArgs->net_gen == 1) {
        std::cout << "Generate social graph..." << std::endl;
        std::cout << "Size: " << net_size << std::endl
                  << "Max edges: " << max_edges << std::endl
                  << "boost arrival: " << boost_arrival << std::endl
                  << "boost edge: " << boost_edge << std::endl
                  << "Fix edge weight: " << desArgs->edge_fixed << std::endl;

        boost::int32_t arrival_rng_index = 0;
        boost::int32_t uniform_rng_index = 0;

        seed = dsample::Seeds::getInstance().getSeed();
        arrival_rng_index = dsample::CRN::getInstance().init(seed);
        dsample::CRN::getInstance().log(seed, "vertex arrival rate");
        seed = dsample::Seeds::getInstance().getSeed();
        uniform_rng_index = dsample::CRN::getInstance().init(seed);
        dsample::CRN::getInstance().log(seed, "uniform");

        r2 = dsample::CRN::getInstance().get(uniform_rng_index);
        r3 = dsample::CRN::getInstance().get(arrival_rng_index);
        graph = dnet::WEvonet::createBBVGraph(net_size, desArgs->max_edges, desArgs->edge_fixed,
                                              desArgs->max_arrival, boost_arrival, boost_edge,
                                              r1, r2, r3);
    } else if (desArgs->net_gen == 2) {
        std::cout << "Generate Erdos-Renyi graph..." << std::endl
                  << "Max edges: " << max_edges << std::endl
                  << "boost arrival: " << boost_arrival << std::endl
                  << "boost edge: " << boost_edge << std::endl
                  << "Fix edge weight: " << desArgs->edge_fixed << std::endl
                  << "Edge prob.: " << edge_prob << std::endl;
        seed = dsample::Seeds::getInstance().getSeed();
        graph = dnet::WEvonet::createERGraph(net_size, desArgs->edge_fixed,
                                             desArgs->max_arrival, boost_arrival,
                                             boost_edge, r1, seed, edge_prob, max_edges);
    }

    std::cout << "Graph with " << boost::num_vertices(*graph) << " vertices generated." << std::endl;

    return graph;
}


#ifdef HAVE_MPI
void Simulation::simulate(MPI_Datatype &mpi_desargs, MPI_Datatype &mpi_desout,
                          MPI_Comm& group_comm, tDesArgsSP desArgs)
#else
    sim_output Simulation::operator()(tDesArgsSP desArgs)
#endif /* HAVE_MPI */
{
    boost::uint16_t sim_num = 0, rep_num = 0, num_vertices = 0;
    boost::uint16_t net_size = 0, max_edges = 0;
    double edge_prob = 0.0,
        rl_q_alpha = 0.0, rl_q_lambda = 0.0, rl_policy_epsilon = 0.0, rl_policy_boltz_t = 0.0,
        nn_momentum = 0.0, boost_arrival = 0.0, boost_edge = 0.0, rl_policy_wpl_eta = 0.0,
        cognitive_A_pos = 0.0, cognitive_A_neg = 0.0, cognitive_r_pos = 0.0, cognitive_r_neg = 0.0;

    // receive the input arguments via mpi
#ifdef HAVE_MPI
    tSimArgsMPI simArgs;
    MPI_Status status;
    int rc = 0;
    int rank = 0;
    int *minRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // store the minimum rank of a group
    minRank = new int[1];

    while (true) {
# ifndef NDEBUG
        std::cout << "Receive task from master." << std::endl;
        std::cout.flush();
# endif /* NDEBUG */
        rc = MPI_Recv(&simArgs, 1, mpi_desargs, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (rc != MPI_SUCCESS) {
            std::cerr << "Error receiving tasks from master." << std::endl;
            MPI_Abort(MPI_COMM_WORLD, 913);
        }

        // kill yourself, if job's done
        if (status.MPI_TAG == KILL_PILL) {
# ifndef NDEBUG
            std::cout << "Receive kill pill." << std::endl;
            std::cout.flush();
# endif /* NDEBUG */
            break;
        }
        sim_num = simArgs.sim_num;
        rep_num = simArgs.rep_num;
        net_size = simArgs.net_size;
        max_edges = simArgs.max_edges;
        edge_prob = simArgs.edge_prob;
        rl_q_alpha = simArgs.rl_q_alpha;
        rl_q_lambda = simArgs.rl_q_lambda;
        rl_policy_epsilon = simArgs.rl_policy_epsilon;
        rl_policy_boltz_t = simArgs.rl_policy_boltzmann_t;
        nn_momentum = simArgs.nn_momentum;
        boost_arrival = simArgs.boost_arrival;
        boost_edge = simArgs.boost_edge;
        rl_policy_wpl_eta = simArgs.rl_policy_wpl_eta;
        cognitive_A_pos = simArgs.cognitive_A_pos;
        cognitive_A_neg = simArgs.cognitive_A_neg;
        cognitive_r_pos = simArgs.cognitive_r_pos;
        cognitive_r_neg = simArgs.cognitive_r_neg;
#else
        sim_num = desArgs->sim_num;
        rep_num = desArgs->rep_num;
        net_size = desArgs->net_size;
        max_edges = desArgs->max_edges;
        edge_prob = desArgs->edge_prob;
        rl_q_alpha = desArgs->rl_q_alpha;
        rl_q_lambda = desArgs->rl_q_lambda;
        rl_policy_epsilon = desArgs->rl_policy_epsilon;
        rl_policy_boltz_t = desArgs->rl_policy_boltzmann_t;
        nn_momentum = desArgs->nn_momentum;
        boost_arrival = desArgs->boost_arrival;
        boost_edge = desArgs->boost_edge;
        rl_policy_wpl_eta = desArgs->rl_policy_wpl_eta;
        cognitive_A_pos = desArgs->cognitive_A_pos;
        cognitive_A_neg = desArgs->cognitive_A_neg;
        cognitive_r_pos = desArgs->cognitive_r_pos;
        cognitive_r_neg = desArgs->cognitive_r_neg;

#endif /* HAVE_MPI */

# ifndef NDEBUG
        std::cout << "Simulation Parameters: " << std::endl
                  << "Sim Num: " << sim_num << ", rep num: " << rep_num << std::endl
                  << "RL alpha: " << rl_q_alpha << ", RL lambda: " << rl_q_lambda << std::endl
                  << "RL Policy epsilon: " << rl_policy_epsilon << std::endl
                  << "RL Policy Boltz T: " << rl_policy_boltz_t << std::endl
                  << "RL Policy WPL eta: " << rl_policy_wpl_eta << std::endl
                  << "Cognitive A(+): " << cognitive_A_pos << std::endl
                  << "Cognitive A(-): " << cognitive_A_neg << std::endl
                  << "Cognitive r(+): " << cognitive_r_pos << std::endl
                  << "Cognitive r(-): " << cognitive_r_neg << std::endl
                  << "NN Momentum: " << nn_momentum << std::endl;
        std::cout.flush();
# endif /* NDEBUG */

        DesBus dbus;
        dnet::tGraphSP graph(new dnet::Graph);
        dcommon::LadderQueue queue;

        if (desArgs->graph_filename != "") {
            // read the graph
            try {
                dnet::GraphUtil::read(*graph, desArgs->graph_filename, dnet::GraphUtil::GRAPHML);
                desArgs->net_size = boost::num_vertices(*graph);
            } catch (dnet::GraphException &ge) {
                std::cerr << "Error: Cannot open graph file " << desArgs->graph_filename
                          << "!" << std::endl;
#ifdef HAVE_MPI
                MPI_Abort(MPI_COMM_WORLD, 917);
#else
                exit(0);
#endif /* HAVE_MPI */

            }
        } else {
            if (desArgs->graph_single) {
                std::stringstream graphRepositorySS;
                graphRepositorySS << desArgs->results_dir << "/../repository/" << desArgs->sim_dir;
                std::string graphRepositoryDir = graphRepositorySS.str();
                graphRepositorySS << "/graph_dv" << boost_arrival << "_de" << boost_edge << ".gml";
                std::string graphRepository = graphRepositorySS.str();

                if (fs::exists(graphRepository)) {
                    std::cout << "Read existing graph file: " << graphRepository << std::endl;
                    dnet::GraphUtil::read(*graph, graphRepository, dnet::GraphUtil::GRAPHML);
                } else {
#ifdef HAVE_MPI
                    rc = MPI_Allreduce(&rank, minRank, 1, MPI_INT, MPI_MIN, group_comm);
                    if (rc != MPI_SUCCESS) {
                        std::cerr << "Error finding minimum rank in group." << std::endl;
                        MPI_Abort(MPI_COMM_WORLD, 918);
                    }

                    if (rank == *minRank) {
#endif /* HAVE_MPI */
                        std::cout << "Create graph: " << graphRepository << std::endl;
                        // create the repository if it does not exist already
                        if (!fs::exists(graphRepositoryDir)) {
                            fs::create_directories(graphRepositoryDir);
                        }
                        // generate the graph
                        graph = Simulation::createGraph(desArgs, net_size,  max_edges,
                                                        edge_prob, boost_arrival, boost_edge);
                        dnet::GraphUtil::print(*graph, graphRepository, dnet::GraphUtil::GRAPHML);
#ifdef HAVE_MPI
                    }

                    // wait for the graph to be serialised
                    MPI_Barrier(group_comm);

                    // everyone in the group read the graph
                    dnet::GraphUtil::read(*graph, graphRepository, dnet::GraphUtil::GRAPHML);
#endif /* HAVE_MPI */
                }
            } else {
                // generate the graph
                graph = Simulation::createGraph(desArgs, net_size,  max_edges,
                                                edge_prob, boost_arrival, boost_edge);
            }
        }

        // if feature selection is enabled to facilitate self-organising topologies
        if (desArgs->so_topology_features) {
            boost::uint32_t seed = dsample::Seeds::getInstance().getSeed();
            boost::uint32_t rng_rand_feature_index = dsample::CRN::getInstance().init(seed);
            dsample::CRN::getInstance().log(seed, "randomise feature choice.");
            dsample::tGslRngSP rng = dsample::CRN::getInstance().get(rng_rand_feature_index);

            dnet::WEvonet::establishFeatures(graph, rng);
        }

        num_vertices = boost::num_vertices(*graph);

        // instantiate the channels (order of addition is important)
        QueueChannel queueChannel(queue);
        GraphChannel graphChannel(*graph);
        ConfigChannel configChannel(*desArgs);

        dbus.addChannel(queueChannel);
        dbus.addChannel(graphChannel);
        dbus.addChannel(configChannel);

        // instantiate the arrival channel
        ArrivalsChannel *arrivalChannel = NULL;
        Arrivals *arrivals = NULL;

        if (desArgs->mfrw) {
            if (desArgs->mfrw_single) {
                std::stringstream mfrwSS;
                mfrwSS << desArgs->results_dir;
                std::string mfrwDir = mfrwSS.str();
                mfrwSS << "/mfrw.dat";
                std::string mfrwFile = mfrwSS.str();

                if (fs::exists(mfrwFile)) {
                    // read the MFRW from file
                    std::cout << "Read generated MFRW: " << mfrwFile << std::endl;
                    arrivals = new CJYArrivals(dbus, mfrwFile, true);
                    arrivals->generate(false);
                } else {
#ifdef HAVE_MPI
                    rc = MPI_Allreduce(&rank, minRank, 1, MPI_INT, MPI_MIN, group_comm);
                    if (rc != MPI_SUCCESS) {
                        std::cerr << "Error finding minimum rank in group." << std::endl;
                        MPI_Abort(MPI_COMM_WORLD, 918);
                    }

                    if (rank == *minRank) {
#endif /* HAVE_MPI */
                        std::cout << "Generate MFRW: " << mfrwFile << std::endl;
                        // create the repository if it does not exist already
                        if (!fs::exists(mfrwDir)) {
                            fs::create_directories(mfrwDir);
                        }

                        // generate the MFRW and serialise it
                        arrivals = new CJYArrivals(dbus, mfrwFile, false);
                        arrivals->generate(true);
#ifdef HAVE_MPI
                    }

                    // wait for the MFRW to be serialised
                    MPI_Barrier(group_comm);

                    // everyone, but the master in the group read the MFRW
                    if (rank != *minRank) {
                        arrivals = new CJYArrivals(dbus, mfrwFile, true);
                        arrivals->generate(false);
                    }
#endif /* HAVE_MPI */
                }
            } else {
                std::cout << "Generated MFRW." << std::endl;
                // generate the MFRW
                std::string mfrwFile("");
                arrivals = new CJYArrivals(dbus, mfrwFile, false);
                arrivals->generate(false);
            }

            arrivalChannel = new ArrivalsChannel(arrivals);
            dbus.addChannel(*arrivalChannel);


#ifndef NDEBUG_EVENTS
            std::cout << "Generate serialise events..." << std::endl;
#endif /* NDEBUG */

            // generate serialise arrival events here
            EventGenerator::generateAdminEventType(queue, 0.0, SERIALISE_ARRIVAL_EVENT);
        } else {
            arrivalChannel = new ArrivalsChannel;
            dbus.addChannel(*arrivalChannel);
        }

        // init the crn for the arrival events
        Int32SA arrivalCRNs = arrivalCRN(num_vertices);

        // init the crn for the service events
        Int32SA serviceCRNs = serviceCRN(num_vertices);

        // init the crn for the departure uniform rv
        Int32SA departureCRNs = departureCRN(num_vertices);

#ifndef NDEBUG
        std::cout << "Generate events..." << std::endl;
#endif /* NDEBUG */

        EventGenerator::generateAdminEventType(queue, 0.0, GENERATE_ARRIVAL_EVENT);

        if (desArgs->log_graphs) {
            EventGenerator::generateAdminEventType(queue, 0.0, LOG_GRAPH_EVENT);
        }

        // configure the results directory
        std::stringstream baseDir;
        baseDir << desArgs->results_dir << "/" << sim_num << "/" << rep_num;
        std::string resultsBaseDir = baseDir.str();

        // ADMIN EVENTS
        // instantiate the admin handlers and register with admin event handler
        GenerateArrivalsAdminHandler generateArrivalsAdminHandler(dbus, arrivalCRNs);
        SerialiseArrivalsHandler serialiseArrivalsHandler(dbus);
        LogGraphHandler logGraphHandler(dbus, resultsBaseDir);
        RollingSystemStatsAdminHandler *rollingSystemStatsAdmin = NULL;

        // attach the handlers to the events
        // the order of the handlers is important
        AdminEvent adminEvent;
        adminEvent.attach(serialiseArrivalsHandler);
        adminEvent.attach(generateArrivalsAdminHandler);
        adminEvent.attach(logGraphHandler);

        if (desArgs->system_stats_steps > 0) {
            rollingSystemStatsAdmin = new RollingSystemStatsAdminHandler(dbus);
            adminEvent.attach(*rollingSystemStatsAdmin);
            EventGenerator::generateAdminEventType(queue, 0.0, SYSTEM_STATISTICS_EVENT);
        }

        // PREANY & POST EVENT
        // only register the logging handlers, if they are configured.
        dio::Results processed_events(desArgs->events_processed, resultsBaseDir);
        dio::Results unprocessed_events(desArgs->events_unprocessed, resultsBaseDir);

        ProcessedEventsHandler processedEventsHandler(processed_events);
        UnprocessedEventsHandler unprocessedEventsHandler(dbus, unprocessed_events);
        NullUnprocessedEventsHandler nullUnprocessedEventsHandler(dbus);
        SystemStatisticsHandler systemStatisticsHandler(dbus);
        PreAnyEvent preAnyEvent;
        PostEvent postEvent;

        postEvent.attach(systemStatisticsHandler);

        if (desArgs->log_events) {
            preAnyEvent.attach(processedEventsHandler);
            postEvent.attach(unprocessedEventsHandler);
        } else {
            // handle unprocessed events, i.e., just drop them off the queue here
            postEvent.attach(nullUnprocessedEventsHandler);
        }

        // POSTANY EVENT
        LastEventHandler lastEventHandler(dbus);
        UtilisationHandler utilisationHandler(dbus);
        ExpectedAverageEventInQueueHandler expectedAverageEventInQueueHandler(dbus);
        PostAnyEvent postAnyEvent;
        postAnyEvent.attach(utilisationHandler);
        postAnyEvent.attach(expectedAverageEventInQueueHandler);
        postAnyEvent.attach(lastEventHandler);


        // ARRIVAL EVENT
        ArrivalHandler arrivalHandler(dbus, serviceCRNs);
        NumEventsHandler numEventsHandler(dbus);
        ArrivalEvent arrivalEvent;
        arrivalEvent.attach(numEventsHandler);
        arrivalEvent.attach(arrivalHandler);

        // LAST ARRIVAL EVENT
        GenerateArrivalsHandler generateArrivalsHandler(dbus, arrivalCRNs);
        LastArrivalEvent lastArrivalEvent;
        lastArrivalEvent.attach(generateArrivalsHandler);

        // LEAVE EVENTS
        LeaveHandler leaveHandler(dbus);
        LeaveEvent leaveEvent;
        leaveEvent.attach(leaveHandler);


        // ACK & DEPARTURE EVENT
        DepartureEvent departureEvent;
        AckHandler ackHandler(dbus);
        AckEvent ackEvent;

        // configure the cognitive value handler
        // tCognitiveValueHandlerSP cognitiveValHandler;
        // if (desArgs->rl_policy == 5) {
        //     cognitiveValHandler = tCognitiveValueHandlerSP(new CognitiveValueHandler(dbus));
        //     ackEvent.attach(*cognitiveValHandler);
        // }

        // configure the expert metrics
        tExpertNormalHandlerSP expertNormalHandler;
        tExpertAbsoluteHandlerSP expertAbsoluteHandler;
        tExpertPositiveHandlerSP expertPositiveHandler;
        tExpertNegativeHandlerSP expertNegativeHandler;
        tResponseStatsHandlerSP responseStatsHandler;

        if (desArgs->expert_normal || desArgs->expert_absolute
            || desArgs->expert_positive || desArgs->expert_negative) {
            responseStatsHandler = tResponseStatsHandlerSP(new ResponseStatsHandler(dbus));
            ackEvent.attach(*responseStatsHandler);
            expertNormalHandler = tExpertNormalHandlerSP(new ExpertNormalHandler(dbus));
            ackEvent.attach(*expertNormalHandler);
            expertAbsoluteHandler = tExpertAbsoluteHandlerSP(new ExpertAbsoluteHandler(dbus));
            ackEvent.attach(*expertAbsoluteHandler);
            expertPositiveHandler = tExpertPositiveHandlerSP(new ExpertPositiveHandler(dbus));
            ackEvent.attach(*expertPositiveHandler);
            expertNegativeHandler = tExpertNegativeHandlerSP(new ExpertNegativeHandler(dbus));
            ackEvent.attach(*expertNegativeHandler);
        }

        // configure reinforcement learning
        tPolicySP pol;
        tSelectionSP selection;
        tDepartureHandlerSP departureHandler;
        tRLResponseHandlerSP rlResponseHandler;
        tFullRLResponseHandlerSP fullRlResponseHandler;
        tHybridFullRLResponseHandlerSP hybridFullRlResponseHandler;
        tDefaultResponseHandlerSP defaultResponseHandler;

        if (desArgs->rl) {
            if (desArgs->rl_policy == 1) {
                pol = tPolicySP(new drl::EpsilonGreedy(dbus));
            } else if (desArgs->rl_policy == 2) {
                pol = tPolicySP(new drl::BoltzmannPolicy(dbus));
            } else if (desArgs->rl_policy == 3) {
                pol = tPolicySP(new drl::WeightedPolicyLearner(dbus));
            } else if (desArgs->rl_policy == 4) {
                pol = tPolicySP(new drl::FairActionLearner(dbus));
            } else if (desArgs->rl_policy == 5) {
                pol = tPolicySP(new drl::GradientPolicyLearner(dbus));
            }

            // configure the on-policy selection
            selection = tSelectionSP(new drl::OnPolicySelection(*pol, dbus));
            departureHandler = tDepartureHandlerSP(new DepartureHandler(dbus, *selection));
            departureEvent.attach(*departureHandler);

            if (desArgs->rl_state_representation.size() > 0) {
                // configure the full reinforcement handler with function approximation
                if (desArgs->rl_hybrid) {
                    hybridFullRlResponseHandler = tHybridFullRLResponseHandlerSP(
                        new HybridFullRLResponseHandler(dbus, *pol, rl_q_alpha, rl_q_lambda, nn_momentum));
                    ackEvent.attach(*hybridFullRlResponseHandler);
                } else {
                    fullRlResponseHandler = tFullRLResponseHandlerSP(
                        new FullRLResponseHandler(dbus, *pol, rl_q_alpha, rl_q_lambda, nn_momentum));
                    ackEvent.attach(*fullRlResponseHandler);
                }
            } else {
                // configure the simple on-policy SARSA control RL handler
                rlResponseHandler = tRLResponseHandlerSP(
                    new RLResponseHandler(dbus, rl_q_alpha, rl_q_lambda, *pol));
                ackEvent.attach(*rlResponseHandler);
            }
        } else {
            pol = tPolicySP(new drl::DummyPolicy());
            selection = tSelectionSP(
                new drl::RandomSelection(dbus, *pol, departureCRNs));
            departureHandler = tDepartureHandlerSP(new DepartureHandler(dbus, *selection));
            departureEvent.attach(*departureHandler);

            // configure default response handler
            defaultResponseHandler = tDefaultResponseHandlerSP(new DefaultResponseHandler(dbus));
            ackEvent.attach(*defaultResponseHandler);
        }

        ackEvent.attach(ackHandler);

        tCoefficientVariationHandlerSP coeffVarHandler;
        if (desArgs->coefficient_variation) {
            coeffVarHandler = tCoefficientVariationHandlerSP(new CoefficientVariationHandler(dbus));
            ackEvent.attach(*coeffVarHandler);
        }

        // instantiate the event processor and set the events
        EventProcessor processor(dbus, adminEvent, preAnyEvent, postAnyEvent, arrivalEvent,
                                 departureEvent, postEvent, lastArrivalEvent, ackEvent, leaveEvent);

        // process the events
#ifndef NDEBUG
        std::cout << "Process events" << std::endl;
#endif /* NDEBUG */
        processor.process();
        sim_output output;

        Report::accumResults(graph, &output);
        output.simulation_id = sim_num;
        output.replications = rep_num;

        // cleanup the arrivals
        if (arrivals != NULL) {
            delete arrivals;
        }
        if (arrivalChannel != NULL) {
            delete arrivalChannel;
        }
        if (rollingSystemStatsAdmin != NULL) {
            delete rollingSystemStatsAdmin;
        }

#ifdef HAVE_MPI
        // send the result back
# ifndef NDEBUG
        std::cout << "Send result back" << std::endl;
        std::cout << "Simulation: " << output.simulation_id << ", replications: "
                  << output.replications << std::endl;
        std::cout.flush();
# endif /* NDEBUG */
        rc = MPI_Send(&output, 1, mpi_desout, 0, status.MPI_TAG, MPI_COMM_WORLD);
        if (rc != MPI_SUCCESS) {
            std::cerr << "Error sending result to master." << std::endl;
            MPI_Abort(MPI_COMM_WORLD, 914);
        }
    }

    delete [] minRank;

#else
    return output;
#endif /* HAVE_MPI */
}


}
}
