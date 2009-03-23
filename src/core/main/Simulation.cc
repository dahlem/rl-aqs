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
#include <boost/graph/adjacency_list.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

#include <gsl/gsl_randist.h>

#include "AckEvent.hh"
#include "AckHandler.hh"
#include "AdminEvent.hh"
#include "ArrivalEvent.hh"
#include "ArrivalHandler.hh"
#include "BoltzmannPolicy.hh"
#include "CL.hh"
#include "DepartureEvent.hh"
#include "DepartureHandler.hh"
#include "events.hh"
#include "EventGenerator.hh"
#include "EventProcessor.hh"
#include "ExpectedAverageEventInQueueHandler.hh"
#include "GenerateEventHandler.hh"
#include "LastArrivalEvent.hh"
#include "LastEventHandler.hh"
#include "LogGraphHandler.hh"
#include "NullUnprocessedEventsHandler.hh"
#include "NumEventsHandler.hh"
#include "PostEvent.hh"
#include "PreAnyEvent.hh"
#include "ProcessedEventsHandler.hh"
#include "Report.hh"
#include "ResponseStatsHandler.hh"
#include "RLResponseHandler.hh"
#include "Simulation.hh"
#include "UnprocessedEventsHandler.hh"
#include "UtilisationHandler.hh"

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


namespace des
{
namespace core
{

typedef boost::shared_array <boost::int32_t> Int32SA;


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


#ifdef HAVE_MPI
void Simulation::simulate(MPI_Datatype &mpi_desargs, MPI_Datatype &mpi_desout,
                          tDesArgsSP desArgs)
#else
sim_output Simulation::simulate(tDesArgsSP desArgs)
#endif /* HAVE_MPI */
{
    boost::uint16_t sim_num, rep_num, num_vertices;
    boost::uint16_t net_size, max_edges;
    double edge_prob, rl_q_alpha, rl_q_lambda, rl_policy_epsilon, rl_policy_boltz_t;

    // receive the input arguments via mpi
#ifdef HAVE_MPI
    tSimArgsMPI simArgs;
    MPI_Status status;
    int rc;

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

#endif /* HAVE_MPI */

        dnet::tGraphSP graph(new dnet::Graph);
        dcommon::tQueueSP queue(new dcommon::LadderQueue);

        if (desArgs->graph_filename != "") {
            // read the graph
            try {
                dnet::GraphUtil::read(graph, desArgs->graph_filename, dnet::GraphUtil::GRAPHML);
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
            // generate the graph
            std::cout << "Generate the graph." << std::endl;
            dsample::tGslRngSP r1, r2, r3;
            boost::int32_t num_edges_rng_index;
            boost::uint32_t seed = 0;

            seed = dsample::Seeds::getInstance().getSeed();
            num_edges_rng_index = dsample::CRN::getInstance().init(seed);
            dsample::CRN::getInstance().log(seed, "number of edges");

            r1 = dsample::CRN::getInstance().get(num_edges_rng_index);

            if (desArgs->net_gen == 1) {
                std::cout << "Generate social graph..." << std::endl;
                std::cout << "Size: " << desArgs->net_size << std::endl
                          << "Max edges: " << max_edges << std::endl
                          << "Fix edge weight: " << desArgs->edge_fixed << std::endl;

                boost::int32_t arrival_rng_index;
                boost::int32_t uniform_rng_index;

                seed = dsample::Seeds::getInstance().getSeed();
                arrival_rng_index = dsample::CRN::getInstance().init(seed);
                dsample::CRN::getInstance().log(seed, "vertex arrival rate");
                seed = dsample::Seeds::getInstance().getSeed();
                uniform_rng_index = dsample::CRN::getInstance().init(seed);
                dsample::CRN::getInstance().log(seed, "uniform");

                r2 = dsample::CRN::getInstance().get(uniform_rng_index);
                r3 = dsample::CRN::getInstance().get(arrival_rng_index);
                graph = dnet::WEvonet::createBBVGraph(desArgs->net_size, desArgs->max_edges, desArgs->edge_fixed,
                                                      desArgs->max_arrival, desArgs->boost_arrival, desArgs->boost_edge,
                                                      r1, r2, r3);
            } else if (desArgs->net_gen == 2) {
                std::cout << "Generate Erdos-Renyi graph..." << std::endl;
                seed = dsample::Seeds::getInstance().getSeed();
                graph = dnet::WEvonet::createERGraph(desArgs->net_size, desArgs->edge_fixed,
                                                     desArgs->max_arrival, desArgs->boost_arrival,
                                                     desArgs->boost_edge, r1, seed, edge_prob, max_edges);
            }
        }

#ifndef NDEBUG
        std::cout << "Simulation: " << sim_num << ", Replication: " << rep_num << std::endl;
        std::cout.flush();
#endif /* NDEBUG */

        num_vertices = boost::num_vertices(*graph);

        // init the crn for the arrival events
        Int32SA arrivalCRNs = arrivalCRN(num_vertices);

        // init the crn for the service events
        Int32SA serviceCRNs = serviceCRN(num_vertices);

        // init the crn for the departure uniform rv
        Int32SA departureCRNs = departureCRN(num_vertices);

        dnet::VertexIndexMap vertex_index_props_map =
            get(boost::vertex_index, *graph);
        dnet::VertexArrivalRateMap vertex_arrival_props_map =
            get(vertex_arrival_rate, *graph);

        // generate events for each vertex in the graph
        double stopTimeAdj;

        // find out whether we only generate the events in phases
        if (desArgs->generations < 0) {
            stopTimeAdj = desArgs->stop_time;
        } else {
            // calculate the phases
            stopTimeAdj = desArgs->stop_time / desArgs->generations;
        }

        boost::int32_t destination;
        double arrival_rate;

        // generate events over this graph
        if (desArgs->trace_event) {
            int count = 0;

            // filter the graph to find the vertex
            std::pair <dnet::VertexIterator, dnet::VertexIterator> v_iter;
            v_iter = boost::vertices(*graph);

            typedef boost::filter_iterator<dnet::exists_vertex_index<dnet::VertexIndexMap>, dnet::VertexIterator>
                FilterIter;

            dnet::exists_vertex_index<dnet::VertexIndexMap>
                predicate(vertex_index_props_map, desArgs->vertex);
            FilterIter filter_iter_first(predicate, v_iter.first, v_iter.second);
            FilterIter filter_iter_last(predicate, v_iter.second, v_iter.second);

            for (; filter_iter_first != filter_iter_last; ++filter_iter_first) {
                if (count == 0) {
                    // generate a single event
                    destination = vertex_index_props_map[*filter_iter_first];
                    arrival_rate = vertex_arrival_props_map[*filter_iter_first];

                    dsample::tGslRngSP arrival_rng = dsample::CRN::getInstance().get(
                        arrivalCRNs[destination]);
                    EventGenerator::generate(
                        queue, arrival_rng, destination, arrival_rate);
                } else {
                    std::cout << "Error: Expected a single vertex to be traced!" << std::endl;
                    break;
                }

                count++;
            }
        } else {
            double graphGenRate = desArgs->stop_time;

            std::pair <dnet::VertexIterator, dnet::VertexIterator> p;
            dsample::tGslRngSP arrival_rng;

#ifndef NDEBUG
            std::cout << "Generate events" << std::endl;
#endif /* NDEBUG */

            for (p = boost::vertices(*graph); p.first != p.second; ++p.first) {
                destination = vertex_index_props_map[*p.first];
                arrival_rate = vertex_arrival_props_map[*p.first];

                arrival_rng = dsample::CRN::getInstance().get(
                    arrivalCRNs[destination]);
                EventGenerator::generate(
                    graph, queue, arrival_rng, destination, arrival_rate, stopTimeAdj);
            }

            if (desArgs->log_graphs) {
                if (desArgs->graph_rate > 1) {
                    graphGenRate = desArgs->stop_time / desArgs->graph_rate;
                }

                EventGenerator::generateLogGraph(
                    queue, graphGenRate, desArgs->stop_time);
            }
        }

        // configure the results directory
        std::stringstream baseDir;
        baseDir << desArgs->results_dir << "/" << sim_num << "/" << rep_num;
        std::string resultsBaseDir = baseDir.str();

        // instantiate the events & handlers
        tAdminEventSP adminEvent(new AdminEvent);
        tPreAnyEventSP preAnyEvent(new PreAnyEvent);
        tPostAnyEventSP postAnyEvent(new PostAnyEvent);
        tArrivalEventSP arrivalEvent(new ArrivalEvent);
        tDepartureEventSP departureEvent(new DepartureEvent);
        tPostEventSP postEvent(new PostEvent);
        tLastArrivalEventSP lastArrivalEvent(new LastArrivalEvent);
        tAckEventSP ackEvent(new AckEvent);
        tLeaveEventSP leaveEvent(new LeaveEvent);

        tLogGraphHandlerSP logGraphHandler(
            new LogGraphHandler(resultsBaseDir, graph));

        tArrivalHandlerSP arrivalHandler(
            new ArrivalHandler(queue, graph, serviceCRNs));
        tNumEventsHandlerSP numEventsHandler(
            new NumEventsHandler(graph));
        tLastEventHandlerSP lastEventHandler(
            new LastEventHandler(graph));
        tUtilisationHandlerSP utilisationHandler(
            new UtilisationHandler(graph));
        tExpectedAverageEventInQueueHandlerSP expectedAverageEventInQueueHandler(
            new ExpectedAverageEventInQueueHandler(graph));
        tAckHandlerSP ackHandler(
            new AckHandler(queue));

        // we only need to register an event generation handler, if there are > 1 phases
        if (desArgs->generations > 1) {
            tGenerateEventHandlerSP generateEventHandler(
                new GenerateEventHandler(
                    graph, arrivalCRNs, desArgs->generations, queue, desArgs->stop_time));
            lastArrivalEvent->attach(generateEventHandler);
        }

        // attach the handlers to the events
        // the order of the handlers is important
        adminEvent->attach(logGraphHandler);

        // only register the logging handlers, if they are configured.
        if (desArgs->log_events) {
            dio::tResultsSP processed_events(
                new dio::Results(desArgs->events_processed, resultsBaseDir));
            dio::tResultsSP unprocessed_events(
                new dio::Results(desArgs->events_unprocessed, resultsBaseDir));

            tProcessedEventsHandlerSP processedEventsHandler(
                new ProcessedEventsHandler(processed_events));
            tUnprocessedEventsHandlerSP unprocessedEventsHandler(
                new UnprocessedEventsHandler(unprocessed_events, queue));

            preAnyEvent->attach(processedEventsHandler);
            postEvent->attach(unprocessedEventsHandler);
        } else {
            // handle unprocessed events, i.e., just drop them off the queue here
            tNullUnprocessedEventsHandlerSP nullUnprocessedEventsHandler(
                new NullUnprocessedEventsHandler(queue));
            postEvent->attach(nullUnprocessedEventsHandler);
        }

        arrivalEvent->attach(numEventsHandler);
        arrivalEvent->attach(arrivalHandler);

        // configure reinforcement learning
        if (desArgs->rl) {
            drl::tPolicySP pol;

            if (desArgs->rl_policy == 1) {
                boost::uint32_t seed = dsample::Seeds::getInstance().getSeed();
                boost::uint32_t pol_epsilon_rng_index
                    = dsample::CRN::getInstance().init(seed);
                dsample::CRN::getInstance().log(seed, "epsilon policy");
                dsample::tGslRngSP r1
                    = dsample::CRN::getInstance().get(pol_epsilon_rng_index);

                boost::uint32_t pol_uniform_rng_index
                    = dsample::CRN::getInstance().init(seed);
                dsample::CRN::getInstance().log(seed, "epsilon uniform");
                dsample::tGslRngSP r2
                    = dsample::CRN::getInstance().get(pol_uniform_rng_index);

                pol = drl::tPolicySP(new drl::EpsilonGreedy(desArgs->rl_policy_epsilon, r1, r2));
            } else if (desArgs->rl_policy == 2) {
                boost::uint32_t seed = dsample::Seeds::getInstance().getSeed();
                boost::uint32_t pol_uniform_rng_index
                    = dsample::CRN::getInstance().init(seed);
                dsample::CRN::getInstance().log(seed, "boltzmann uniform");
                dsample::tGslRngSP r1
                    = dsample::CRN::getInstance().get(pol_uniform_rng_index);

                pol = drl::tPolicySP(new drl::BoltzmannPolicy(desArgs->rl_policy_boltzmann_t, r1));
            }

            // configure the on-policy selection
            drl::tSelectionSP selection = drl::tSelectionSP(
                new drl::OnPolicySelection(pol, graph));
            tDepartureHandlerSP departureHandler(
                new DepartureHandler(queue, graph, selection));
            departureEvent->attach(departureHandler);

            // configure the simple on-policy SARSA control RL handler
            tRLResponseHandlerSP rlResponseHandler(
                new RLResponseHandler(
                    graph, desArgs->response_alpha, desArgs->response_reward, desArgs->response_levels,
                    rl_q_alpha, rl_q_lambda, pol));
            ackEvent->attach(rlResponseHandler);
        } else {
            drl::tPolicySP pol = drl::tPolicySP();
            drl::tSelectionSP selection = drl::tSelectionSP(
                new drl::RandomSelection(pol, graph, departureCRNs));
            tDepartureHandlerSP departureHandler(
                new DepartureHandler(queue, graph, selection));
            departureEvent->attach(departureHandler);
        }

        ackEvent->attach(ackHandler);

        postAnyEvent->attach(utilisationHandler);
        postAnyEvent->attach(expectedAverageEventInQueueHandler);
        postAnyEvent->attach(lastEventHandler);

        // instantiate the event processor and set the events
        tEventProcessorSP processor(
            new EventProcessor(queue, adminEvent, preAnyEvent, postAnyEvent, arrivalEvent,
                               departureEvent, postEvent, lastArrivalEvent, ackEvent,
                               leaveEvent, desArgs->stop_time));

        // process the events
#ifndef NDEBUG
        std::cout << "Process events" << std::endl;
#endif /* NDEBUG */
        processor->process();
        sim_output output;

        Report::accumResults(graph, &output);
        output.simulation_id = sim_num;
        output.replications = rep_num;

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
#else
    return output;
#endif /* HAVE_MPI */
}


}
}
