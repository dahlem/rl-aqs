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

/** @file SimulationMPI.hh
 * This header declares the simulation method, so that it can be considered a
 * black-box simulation by the main method.
 *
 * @author Dominik Dahlem
 */
#ifndef __DES_CORE_SIMULATIONMPI_HH__
#define __DES_CORE_SIMULATIONMPI_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <cstring>
#include <sstream>
#include <string>
#include <vector>

#ifdef HAVE_MPI
# include <mpi.h>
#endif /* HAVE_MPI */

#include <boost/cstdint.hpp>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

#include "Results.hh"
#include "FsUtils.hh"
namespace dio = des::io;

#include "CRN.hh"
#include "Seeds.hh"
#include "LHS.hh"
namespace dsample = des::sampling;

#include "CI.hh"
#include "OnlineStats.hh"
namespace dstats = des::statistics;

#include "common.hh"
#include "CL.hh"
#include "LhsUtils.hh"



namespace des
{
namespace core
{

typedef boost::shared_array<bool> tBoolSA;


/** @class SimulationMPI
 * Class encapsulating the logic to start an Latin-Hypercube Design of experiments.
 */
class SimulationMPI
{
public:

    /** @fn static sim_output simulate()
     * Conduct the simulation to be within a given confidence interval.
     *
     * @return the simulation output variables
     */
    static void simulate(tDesArgsSP p_desArgs, MPI_Datatype &mpi_desargs, MPI_Datatype &mpi_desout)
        {
            MPI_Status status;
            int jobs = 1;
            int rc = 0;
            int runs = 0, already_run = 0;
            int nodes = 0;

            // 1. perform lhs
            // calculate the number of simulations
            if (p_desArgs->lhs_optimal) {
                runs = (1 << p_desArgs->simulations) + 1;
            } else {
                runs = p_desArgs->simulations;
            }

            gsl_vector *min, *max;
            gsl_matrix *sample;
            int *simReplications = new int[runs];
            int *simInitReplications = new int[runs];
            int *simActiveReplications = new int[runs];

            // keep track of the significance of the experiments and their replication count
            tBoolSA areExpsSignificant = tBoolSA(new bool[runs]);
            for (boost::uint16_t i = 0; i < runs; ++i) {
                areExpsSignificant[i] = false;
                simReplications[i] = p_desArgs->init_replications;
                simInitReplications[i] = p_desArgs->init_replications;
                simActiveReplications[i] = p_desArgs->init_replications;
            }

            // if more nodes are available allocate those to the replications
            MPI_Comm_size(MPI_COMM_WORLD, &nodes);
            boost::uint16_t freeNodes = nodes - (runs * p_desArgs->init_replications + 1);
#ifndef NDEBUG
            std::cout << "Number of additional nodes: " << freeNodes  << std::endl;
#endif /* NDEBUG */

            for (boost::uint16_t i = 0; i < freeNodes; ++i) {
#ifndef NDEBUG
                std::cout << "Increment number of replicas for sim: " << (i%runs)+1 << std::endl;
#endif /* NDEBUG */

                simReplications[i%runs]++;
                simInitReplications[i%runs]++;
                simActiveReplications[i%runs]++;
            }

            // online statistics for the experiments
            std::vector<dstats::OnlineStats> avgDelays(runs);
            std::vector<dstats::OnlineStats> avgNumEvents(runs);
            std::vector<dstats::OnlineStats> totalQs(runs);
            std::vector<int> idleNodes;
            std::vector<int> lazyNodes;

            // 2. init the crn for the lhs permutation
            boost::uint32_t seed = dsample::Seeds::getInstance().getSeed();
            boost::int32_t rng_index = dsample::CRN::getInstance().init(seed);
            dsample::CRN::getInstance().log(seed, "LHS permutation");
            dsample::tGslRngSP rng = dsample::CRN::getInstance().get(rng_index);
            int dimensions = LhsUtils::dimensions(p_desArgs);

            std::stringstream outDir, simDir, csv_line;
            std::vector <dio::Results*> replica_results;

            simDir << p_desArgs->results_dir << "/";

            std::stringstream *sim_results_lines = new std::stringstream[runs];

            std::string dir = simDir.str();
            std::string file = "simulations.dat";

            dio::Results sim_results(file, dir);

#ifndef NDEBUG
            std::cout << "The number of LHS dimensions is " << dimensions << std::endl;
            std::cout << "Network Size Index: " << LhsUtils::getNetSizeIndex(p_desArgs) << std::endl;
            std::cout << "Max. Edges Index: " << LhsUtils::getMaxEdgesIndex(p_desArgs) << std::endl;
            std::cout << "Edge Prob. Index: " << LhsUtils::getEdgeProbIndex(p_desArgs) << std::endl;
            std::cout << "Arrival Boost Index: " << LhsUtils::getVertexBoostIndex(p_desArgs) << std::endl;
            std::cout << "Edge Boost Index: " << LhsUtils::getEdgeBoostIndex(p_desArgs) << std::endl;
            std::cout << "RL Q alpha Index: " << LhsUtils::getRLAlphaIndex(p_desArgs) << std::endl;
            std::cout << "RL Q Lambda Index: " << LhsUtils::getRLLambdaIndex(p_desArgs) << std::endl;
            std::cout << "RL epsilon Index: " << LhsUtils::getRLEpsilonIndex(p_desArgs) << std::endl;
            std::cout << "NN momentum Index: " << LhsUtils::getNNMomentumIndex(p_desArgs) << std::endl;
            std::cout << "RL WPL eta Index: " << LhsUtils::getRlWplEtaIndex(p_desArgs) << std::endl;
            std::cout << "CPL A(+) Index: " << LhsUtils::getCplAPosIndex(p_desArgs) << std::endl;
            std::cout << "CPL A(-) Index: " << LhsUtils::getCplANegIndex(p_desArgs) << std::endl;
            std::cout << "CPL r(+) Index: " << LhsUtils::getCplRPosIndex(p_desArgs) << std::endl;
            std::cout << "CPL r(+) Index: " << LhsUtils::getCplRNegIndex(p_desArgs) << std::endl;
#endif /* NDEBUG */

            if (dimensions > 0) {
                min = gsl_vector_calloc(dimensions);
                max = gsl_vector_calloc(dimensions);

                if (LhsUtils::getNetSizeIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getNetSizeIndex(p_desArgs), p_desArgs->min_size);
                    gsl_vector_set(max, LhsUtils::getNetSizeIndex(p_desArgs), p_desArgs->max_size);
                }
                if (LhsUtils::getMaxEdgesIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getMaxEdgesIndex(p_desArgs), p_desArgs->min_max_edges);
                    gsl_vector_set(max, LhsUtils::getMaxEdgesIndex(p_desArgs), p_desArgs->max_max_edges);
                }
                if (LhsUtils::getEdgeProbIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getEdgeProbIndex(p_desArgs), p_desArgs->min_edge_prob);
                    gsl_vector_set(max, LhsUtils::getEdgeProbIndex(p_desArgs), p_desArgs->max_edge_prob);
                }
                if (LhsUtils::getVertexBoostIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getVertexBoostIndex(p_desArgs), p_desArgs->min_boost_arrival);
                    gsl_vector_set(max, LhsUtils::getVertexBoostIndex(p_desArgs), p_desArgs->max_boost_arrival);
                }
                if (LhsUtils::getEdgeBoostIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getEdgeBoostIndex(p_desArgs), p_desArgs->min_boost_edge);
                    gsl_vector_set(max, LhsUtils::getEdgeBoostIndex(p_desArgs), p_desArgs->max_boost_edge);
                }
                if (LhsUtils::getRLAlphaIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getRLAlphaIndex(p_desArgs), p_desArgs->min_rl_q_alpha);
                    gsl_vector_set(max, LhsUtils::getRLAlphaIndex(p_desArgs), p_desArgs->max_rl_q_alpha);
                }
                if (LhsUtils::getRLLambdaIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getRLLambdaIndex(p_desArgs), p_desArgs->min_rl_q_lambda);
                    gsl_vector_set(max, LhsUtils::getRLLambdaIndex(p_desArgs), p_desArgs->max_rl_q_lambda);
                }
                if (LhsUtils::getRLEpsilonIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getRLEpsilonIndex(p_desArgs), p_desArgs->min_rl_policy_epsilon);
                    gsl_vector_set(max, LhsUtils::getRLEpsilonIndex(p_desArgs), p_desArgs->max_rl_policy_epsilon);
                }
                if (LhsUtils::getNNMomentumIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getNNMomentumIndex(p_desArgs), p_desArgs->min_nn_momentum);
                    gsl_vector_set(max, LhsUtils::getNNMomentumIndex(p_desArgs), p_desArgs->max_nn_momentum);
                }
                if (LhsUtils::getRlWplEtaIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getRlWplEtaIndex(p_desArgs), p_desArgs->min_rl_policy_wpl_eta);
                    gsl_vector_set(max, LhsUtils::getRlWplEtaIndex(p_desArgs), p_desArgs->max_rl_policy_wpl_eta);
                }
                if (LhsUtils::getCplAPosIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getCplAPosIndex(p_desArgs), p_desArgs->min_cognitive_A_pos);
                    gsl_vector_set(max, LhsUtils::getCplAPosIndex(p_desArgs), p_desArgs->max_cognitive_A_pos);
                }
                if (LhsUtils::getCplANegIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getCplANegIndex(p_desArgs), p_desArgs->min_cognitive_A_neg);
                    gsl_vector_set(max, LhsUtils::getCplANegIndex(p_desArgs), p_desArgs->max_cognitive_A_neg);
                }
                if (LhsUtils::getCplRPosIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getCplRPosIndex(p_desArgs), p_desArgs->min_cognitive_r_pos);
                    gsl_vector_set(max, LhsUtils::getCplRPosIndex(p_desArgs), p_desArgs->max_cognitive_r_pos);
                }
                if (LhsUtils::getCplAPosIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getCplRNegIndex(p_desArgs), p_desArgs->min_cognitive_r_neg);
                    gsl_vector_set(max, LhsUtils::getCplRNegIndex(p_desArgs), p_desArgs->max_cognitive_r_neg);
                }

                if (p_desArgs->lhs_optimal) {
#ifndef NDEBUG
                    std::cout << "Perform optimal LHS sampling..." << std::endl;
#endif /* NDEBUG */
                    dsample::LHS::sample(rng.get(), min, max, p_desArgs->simulations, &sample, p_desArgs->lhs_r);
                } else {
#ifndef NDEBUG
                    std::cout << "Perform LHS sampling..." << std::endl;
#endif /* NDEBUG */
                    dsample::LHS::sample(rng.get(), min, max, p_desArgs->simulations, &sample);
                }

                // serialise design matrix
                std::string designFile = "design.dat";
                LhsUtils::serialiseDesign(p_desArgs, sample, designFile, dir);
            }

            // adjust the simulation number by already_run
            if (!p_desArgs->add_sim.empty()) {
                already_run = dio::FsUtils::directories(dir) + 1;
            } else {
                csv_line << "sim_num," << ARGS_HEADER << ",actual_reps,meanDelay,varDelay,meanAvgNumEvents,varAvgNumEvents,meanAvgEventInSystem,varAvgEventInSystem";
                sim_results.print(csv_line);
                already_run = 0;
            }
            csv_line.str("");

            // 3. run experiment
            for (boost::uint16_t i = 0; i < runs; ++i) {
                // start initial number of experiments
                // copy the desArgs into the mpi desargs
                tSimArgsMPI desArgsMPI;

                // set the i-th experiment conditions
                desArgsMPI.sim_num = i + 1 + already_run;
                assignParams(p_desArgs, desArgsMPI, sample);

                for (desArgsMPI.rep_num = 1; desArgsMPI.rep_num <= simInitReplications[i]; ++desArgsMPI.rep_num) {
                    // send the desargs to the slave nodes
#ifndef NDEBUG
                    std::cout << "Sending job: " << jobs << ". Simulation " << desArgsMPI.sim_num
                              << " and replication " << desArgsMPI.rep_num << "." << std::endl;
                    std::cout.flush();
#endif /* NDEBUG */
                    rc = MPI_Send(&desArgsMPI, 1, mpi_desargs, jobs, jobs, MPI_COMM_WORLD);
                    if (rc != MPI_SUCCESS) {
                        std::cerr << "Error sending task to slave." << std::endl;
                        MPI_Abort(MPI_COMM_WORLD, 916);
                    }
                    jobs++;
                }

                // prepare the replica results
                outDir << p_desArgs->results_dir << "/" << desArgsMPI.sim_num;

                dir = outDir.str();
                outDir.str("");
                file = "replica_results.dat";

#ifndef NDEBUG
                std::cout << "Creating file " << dir << "/" << file << std::endl;
                std::cout.flush();
#endif /* NDEBUG */

                dio::Results *replica_output = new dio::Results(file, dir);

                csv_line << "sim_num,rep_num,systemDelay,systemAvgNumEvents,systemTotalQ";
                replica_output->print(csv_line);
                csv_line.str("");

                replica_results.push_back(replica_output);

                // prepare the overall results
                sim_results_lines[i] << desArgsMPI.sim_num << ","
                                     << p_desArgs->stop_time << ","
                                     << (p_desArgs->graph_rate + 1) << ","
                                     << p_desArgs->max_arrival << ","
                                     << desArgsMPI.boost_arrival << ","
                                     << desArgsMPI.boost_edge << ","
                                     << p_desArgs->confidence << ","
                                     << p_desArgs->alpha << ","
                                     << p_desArgs->error << ","
                                     << p_desArgs->replications << ","
                                     << p_desArgs->net_gen << ","
                                     << desArgsMPI.net_size << ","
                                     << desArgsMPI.max_edges << ","
                                     << desArgsMPI.edge_prob << ","
                                     << p_desArgs->edge_fixed << ","
                                     << p_desArgs->rl << ","
                                     << p_desArgs->rl_ci << ","
                                     << desArgsMPI.rl_q_alpha << ","
                                     << desArgsMPI.rl_q_lambda << ","
                                     << p_desArgs->rl_policy << ","
                                     << desArgsMPI.rl_policy_epsilon << ","
                                     << desArgsMPI.rl_policy_boltzmann_t << ","
                                     << p_desArgs->rl_hybrid << ","
                                     << p_desArgs->rl_hybrid_warmup << ","
                                     << desArgsMPI.nn_momentum << ","
                                     << desArgsMPI.rl_policy_wpl_eta << ","
                                     << desArgsMPI.cognitive_A_pos << ","
                                     << desArgsMPI.cognitive_A_neg << ","
                                     << desArgsMPI.cognitive_r_pos << ","
                                     << desArgsMPI.cognitive_r_neg;
            }

            // 4. continue with as many experiments as needed
            while (!areAllSignificant(areExpsSignificant, runs)) {
                sim_output *output = new sim_output[1];

#ifndef NDEBUG
                std::cout << "Receive results." << std::endl;
                std::cout.flush();
#endif /* NDEBUG */

                // receive results
                rc = MPI_Recv(output, 1, mpi_desout, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                if (rc != MPI_SUCCESS) {
                    std::cerr << "Error receiving results from slave." << std::endl;
                    MPI_Abort(MPI_COMM_WORLD, 916);
                }

                // add node to the idle vector
                idleNodes.push_back(status.MPI_SOURCE);

#ifndef NDEBUG
                std::cout << "Received simulation: " << output->simulation_id << ", replications: "
                          << output->replications << std::endl << std::flush;
#endif /* NDEBUG */
                // update fields
                avgDelays[output->simulation_id - 1].push(output->system_average_delay);
                avgNumEvents[output->simulation_id - 1].push(output->system_expected_average_num_in_queue);
                totalQs[output->simulation_id - 1].push(output->system_total_q);
#ifndef NDEBUG
                std::cout << "Updated statistics for simulation " << output->simulation_id
                          << " and replication " << output->replications << std::endl
                          << "replications received: " << avgDelays[output->simulation_id - 1].getNumValues()
                          << ", " << (simReplications[output->simulation_id - 1] - avgDelays[output->simulation_id - 1].getNumValues())
                          << " more to go." << std::endl << std::flush;
#endif /* NDEBUG */

                if (avgDelays[output->simulation_id - 1].getNumValues() >= simReplications[output->simulation_id - 1]) {
                    // test whether this result is significant
                    bool isConfident =
                        dstats::CI::isConfidentWithPrecision(
                            avgDelays[output->simulation_id - 1].mean(),
                            avgDelays[output->simulation_id - 1].variance(),
                            avgDelays[output->simulation_id - 1].getNumValues(),
                            p_desArgs->alpha, p_desArgs->error)
                        &&
                        dstats::CI::isConfidentWithPrecision(
                            avgNumEvents[output->simulation_id - 1].mean(),
                            avgNumEvents[output->simulation_id - 1].variance(),
                            avgNumEvents[output->simulation_id - 1].getNumValues(),
                            p_desArgs->alpha, p_desArgs->error)
                        &&
                        dstats::CI::isConfidentWithPrecision(
                            totalQs[output->simulation_id - 1].mean(),
                            totalQs[output->simulation_id - 1].variance(),
                            totalQs[output->simulation_id - 1].getNumValues(),
                            p_desArgs->alpha, p_desArgs->error)
                        &&
                        (avgDelays[output->simulation_id - 1].getNumValues() >= p_desArgs->replications);

#ifndef NDEBUG
                    std::cout << "****** Performance Update ******" << std::endl
                              << "Simulation ID: " << output->simulation_id << ", replication number: " << output->replications << std::endl
                              << "Delay (mu/sigma):                     " << avgDelays[output->simulation_id - 1].mean() << "/" << avgDelays[output->simulation_id - 1].variance() << std::endl
                              << "Avg. Num. Events (mu/sigma):          " << avgNumEvents[output->simulation_id - 1].mean() << "/" << avgNumEvents[output->simulation_id - 1].variance() << std::endl
                              << "Avg. Event in System Time (mu/sigma): " << totalQs[output->simulation_id - 1].mean() << "/" << totalQs[output->simulation_id - 1].variance() << std::endl
                              << std::endl << std::flush;
#endif /* NDEBUG */

                    // if not send another replica
                    if (!isConfident) {
                        // do progressive parallel job execution
                        int moreJobs = (simInitReplications[output->simulation_id - 1] > idleNodes.size()) ? idleNodes.size() : (simActiveReplications[output->simulation_id - 1] + lazyNodes.size());
                        simActiveReplications[output->simulation_id - 1] = moreJobs;

#ifndef NDEBUG
                        std::cout << "Simulation " << output->simulation_id << ", replications: "
                                  << avgDelays[output->simulation_id - 1].getNumValues() << " is not confident." << std::endl
                                  << "Start " << moreJobs << " replications" << std::endl;
                        std::cout.flush();
#endif /* NDEBUG */

                        // allocate more jobs; first the number of replications that belong to a simulation batch anyway
                        for (int i = 0; i < (simActiveReplications[output->simulation_id - 1] - lazyNodes.size()); ++i) {
                            // update the experiment arguments
                            tSimArgsMPI desArgsMPI;

                            desArgsMPI.rep_num = avgDelays[output->simulation_id - 1].getNumValues() + 1 + i;
                            desArgsMPI.sim_num = output->simulation_id;

                            assignParams(p_desArgs, desArgsMPI, sample);

                            int destination = idleNodes.back();

                            rc = MPI_Send(&desArgsMPI, 1, mpi_desargs, destination, jobs, MPI_COMM_WORLD);
                            if (rc != MPI_SUCCESS) {
                                std::cerr << "Error sending task to slave." << std::endl;
                                MPI_Abort(MPI_COMM_WORLD, 916);
                            }

                            // remove the node from the idle vector
                            idleNodes.pop_back();
                            simReplications[output->simulation_id - 1]++;
                            jobs++;
                        }

                        // second, all the lazy nodes are allocated
                        for (int i = (simActiveReplications[output->simulation_id - 1] - lazyNodes.size()); i < moreJobs; ++i) {
                            // update the experiment arguments
                            tSimArgsMPI desArgsMPI;

                            desArgsMPI.rep_num = avgDelays[output->simulation_id - 1].getNumValues() + 1 + i;
                            desArgsMPI.sim_num = output->simulation_id;

                            assignParams(p_desArgs, desArgsMPI, sample);

                            int destination = lazyNodes.back();

                            rc = MPI_Send(&desArgsMPI, 1, mpi_desargs, destination, jobs, MPI_COMM_WORLD);
                            if (rc != MPI_SUCCESS) {
                                std::cerr << "Error sending task to slave." << std::endl;
                                MPI_Abort(MPI_COMM_WORLD, 916);
                            }

                            // remove the node from the idle vector
                            lazyNodes.pop_back();
                            simReplications[output->simulation_id - 1]++;
                            jobs++;
                        }
                    } else {
                        // transfer the previously active nodes to a lazy list
                        for (boost::uint16_t i = 0; i < simActiveReplications[output->simulation_id - 1]; ++i) {
                            lazyNodes.push_back(idleNodes.back());
                            idleNodes.pop_back();
                        }

#ifndef NDEBUG
                        std::cout << "Simulation " << output->simulation_id << ", replications: "
                                  << avgDelays[output->simulation_id - 1].getNumValues() << " is confident" << std::endl;
                        std::cout.flush();
#endif /* NDEBUG */

                        areExpsSignificant[output->simulation_id - 1] = true;

                        // write the overall results
                        sim_results_lines[output->simulation_id - 1]
                            << "," << avgDelays[output->simulation_id - 1].getNumValues() << ","
                            << avgDelays[output->simulation_id - 1].mean() << ","
                            << avgDelays[output->simulation_id - 1].variance() << ","
                            << avgNumEvents[output->simulation_id - 1].mean() << ","
                            << avgNumEvents[output->simulation_id - 1].variance() << ","
                            << totalQs[output->simulation_id - 1].mean() << ","
                            << totalQs[output->simulation_id - 1].variance();
                    }
                }

#ifndef NDEBUG
                std::cout << "Simulation " << output->simulation_id << ", replications: "
                          << avgDelays[output->simulation_id - 1].getNumValues() << " write replication results" << std::endl;
                std::cout.flush();
#endif /* NDEBUG */

                // write replica results
                csv_line.str("");
                csv_line << output->simulation_id << ","
                         << avgDelays[output->simulation_id - 1].getNumValues() << ","
                         << output->system_average_delay << ","
                         << output->system_expected_average_num_in_queue << ","
                         << output->system_total_q;

                replica_results[output->simulation_id - 1]->print(csv_line);
#ifndef NDEBUG
                std::cout << "Simulation " << output->simulation_id << ", replications: "
                          << avgDelays[output->simulation_id - 1].getNumValues() << " wrote replication results" << std::endl;
                std::cout.flush();
#endif /* NDEBUG */

                delete[] output;
            }

#ifndef NDEBUG
            std::cout << "Write the simulation output." << std::endl;
            std::cout.flush();
#endif /* NDEBUG */

            for (boost::uint16_t i = 0; i < runs; ++i) {
                sim_results.print(sim_results_lines[i]);
            }

            for (boost::uint16_t i = 0; i < replica_results.size(); ++i) {
                dio::Results *res = replica_results.back();
                replica_results.pop_back();
                delete res;
            }

            delete[] sim_results_lines;
            delete[] simReplications;
            delete[] simInitReplications;
            delete[] simActiveReplications;

            // 5. free gsl stuff
            if (dimensions > 0) {
                gsl_vector_free(min);
                gsl_vector_free(max);
                gsl_matrix_free(sample);
            }
        }


private:

    SimulationMPI()
        {}

    SimulationMPI(const SimulationMPI&)
        {}

    ~SimulationMPI()
        {}

    void operator=(const SimulationMPI&)
        {}

    static bool areAllSignificant(tBoolSA expSignificant, boost::uint16_t size)
        {
            for (boost::uint16_t i = 0; i < size; ++i) {
                if (!expSignificant[i]) {
                    return false;
                }
            }

            return true;
        }

    static void assignParams(tDesArgsSP p_desArgs, tSimArgsMPI &desArgsMPI, gsl_matrix *sample)
        {
                // set the i-th experiment conditions
                if (LhsUtils::getNetSizeIndex(p_desArgs) >= 0) {
                    desArgsMPI.net_size = static_cast<unsigned int> (
                        round(gsl_matrix_get(
                                  sample, desArgsMPI.sim_num - 1, LhsUtils::getNetSizeIndex(p_desArgs))));
                } else {
                    desArgsMPI.net_size = p_desArgs->net_size;
                }
                if (LhsUtils::getMaxEdgesIndex(p_desArgs) >= 0) {
                    desArgsMPI.max_edges = static_cast<unsigned int> (
                        round(gsl_matrix_get(
                                  sample, desArgsMPI.sim_num - 1, LhsUtils::getMaxEdgesIndex(p_desArgs))));
                } else {
                    desArgsMPI.max_edges = p_desArgs->max_edges;
                }
                if (LhsUtils::getEdgeProbIndex(p_desArgs) >= 0) {
                    desArgsMPI.edge_prob = gsl_matrix_get(
                        sample, desArgsMPI.sim_num - 1, LhsUtils::getEdgeProbIndex(p_desArgs));
                } else {
                    desArgsMPI.edge_prob = p_desArgs->edge_prob;
                }
                if (LhsUtils::getVertexBoostIndex(p_desArgs) >= 0) {
                    desArgsMPI.boost_arrival = gsl_matrix_get(
                        sample, desArgsMPI.sim_num - 1, LhsUtils::getVertexBoostIndex(p_desArgs));
                } else {
                    desArgsMPI.boost_arrival = p_desArgs->boost_arrival;
                }
                if (LhsUtils::getEdgeBoostIndex(p_desArgs) >= 0) {
                    desArgsMPI.boost_edge = gsl_matrix_get(
                        sample, desArgsMPI.sim_num - 1, LhsUtils::getEdgeBoostIndex(p_desArgs));
                } else {
                    desArgsMPI.boost_edge = p_desArgs->boost_edge;
                }
                if (LhsUtils::getRLAlphaIndex(p_desArgs) >= 0) {
                    desArgsMPI.rl_q_alpha = gsl_matrix_get(
                        sample, desArgsMPI.sim_num - 1, LhsUtils::getRLAlphaIndex(p_desArgs));
                } else {
                    desArgsMPI.rl_q_alpha = p_desArgs->rl_q_alpha;
                }
                if (LhsUtils::getRLLambdaIndex(p_desArgs) >= 0) {
                    desArgsMPI.rl_q_lambda = gsl_matrix_get(
                        sample, desArgsMPI.sim_num - 1, LhsUtils::getRLLambdaIndex(p_desArgs));
                } else {
                    desArgsMPI.rl_q_lambda = p_desArgs->rl_q_lambda;
                }
                if (LhsUtils::getRLEpsilonIndex(p_desArgs) >= 0) {
                    desArgsMPI.rl_policy_epsilon = gsl_matrix_get(
                        sample, desArgsMPI.sim_num - 1, LhsUtils::getRLEpsilonIndex(p_desArgs));
                } else {
                    desArgsMPI.rl_policy_epsilon = p_desArgs->rl_policy_epsilon;
                }
                if (LhsUtils::getNNMomentumIndex(p_desArgs) >= 0) {
                    desArgsMPI.nn_momentum = gsl_matrix_get(
                        sample, desArgsMPI.sim_num - 1, LhsUtils::getNNMomentumIndex(p_desArgs));
                } else {
                    desArgsMPI.nn_momentum = p_desArgs->nn_momentum;
                }
                if (LhsUtils::getRlWplEtaIndex(p_desArgs) >= 0) {
                    desArgsMPI.rl_policy_wpl_eta = gsl_matrix_get(
                        sample, desArgsMPI.sim_num - 1, LhsUtils::getRlWplEtaIndex(p_desArgs));
                } else {
                    desArgsMPI.rl_policy_wpl_eta = p_desArgs->rl_policy_wpl_eta;
                }
                if (LhsUtils::getCplAPosIndex(p_desArgs) >= 0) {
                    desArgsMPI.cognitive_A_pos = gsl_matrix_get(
                        sample, desArgsMPI.sim_num - 1, LhsUtils::getCplAPosIndex(p_desArgs));
                } else {
                    desArgsMPI.cognitive_A_pos = p_desArgs->cognitive_A_pos;
                }
                if (LhsUtils::getCplANegIndex(p_desArgs) >= 0) {
                    desArgsMPI.cognitive_A_neg = gsl_matrix_get(
                        sample, desArgsMPI.sim_num - 1, LhsUtils::getCplANegIndex(p_desArgs));
                } else {
                    desArgsMPI.cognitive_A_neg = p_desArgs->cognitive_A_neg;
                }
                if (LhsUtils::getCplRPosIndex(p_desArgs) >= 0) {
                    desArgsMPI.cognitive_r_pos = gsl_matrix_get(
                        sample, desArgsMPI.sim_num - 1, LhsUtils::getCplRPosIndex(p_desArgs));
                } else {
                    desArgsMPI.cognitive_r_pos = p_desArgs->cognitive_r_pos;
                }
                if (LhsUtils::getCplRNegIndex(p_desArgs) >= 0) {
                    desArgsMPI.cognitive_r_neg = gsl_matrix_get(
                        sample, desArgsMPI.sim_num - 1, LhsUtils::getCplRNegIndex(p_desArgs));
                } else {
                    desArgsMPI.cognitive_r_neg = p_desArgs->cognitive_r_neg;
                }
        }
};


}
}



#endif /* __DES_CORE_SIMULATIONMPI_HH__ */
