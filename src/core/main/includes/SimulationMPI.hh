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
            int rc;

            // 1. perform lhs
            gsl_vector *min, *max;
            gsl_matrix *sample;
            std::vector<bool> areExpsSignificant(p_desArgs->simulations, false);
            std::vector<dstats::OnlineStats> avgDelays(p_desArgs->simulations);
            std::vector<dstats::OnlineStats> avgNumEvents(p_desArgs->simulations);

            // 2. init the crn for the lhs permutation
            boost::uint32_t seed = dsample::Seeds::getInstance().getSeed();
            boost::int32_t rng_index = dsample::CRN::getInstance().init(seed);
            dsample::CRN::getInstance().log(seed, "LHS permutation");
            dsample::tGslRngSP rng = dsample::CRN::getInstance().get(rng_index);
            int dimensions = LhsUtils::dimensions(p_desArgs);

#ifndef NDEBUG
            std::cout << "The number of LHS dimensions is " << dimensions << std::endl;
            std::cout << "Network Size Index: " << LhsUtils::getNetSizeIndex(p_desArgs) << std::endl;
            std::cout << "Max. Edges Index: " << LhsUtils::getMaxEdgesIndex(p_desArgs) << std::endl;
            std::cout << "Edge Prob. Index: " << LhsUtils::getEdgeProbIndex(p_desArgs) << std::endl;
            std::cout << "Arrival Boost Index: " << LhsUtils::getVertexBoostIndex(p_desArgs) << std::endl;
            std::cout << "Edge Boost Index: " << LhsUtils::getEdgeBoostIndex(p_desArgs) << std::endl;
#endif /* NDEBUG */

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

            dsample::LHS::sample(rng.get(), min, max, p_desArgs->simulations, &sample);

            std::stringstream outDir, simDir, csv_line;
            std::vector <dio::tResultsSP> replica_results;

            simDir << p_desArgs->results_dir << "/";

            std::stringstream *sim_results_lines = new std::stringstream[p_desArgs->simulations];

            std::string dir = simDir.str();
            std::string file = "simulations.dat";

            dio::tResultsSP sim_results(
                new dio::Results(file, dir));

            if (p_desArgs->add_sim.empty()) {
                csv_line << "sim_num," << ARGS_HEADER << ",actual_reps";
                sim_results->print(csv_line);
            }
            csv_line.str("");

            // 3. run experiment
            for (boost::uint16_t i = 0; i < p_desArgs->simulations; ++i) {
                // start initial number of experiments
                // copy the desArgs into the mpi desargs
                tSimArgsMPI desArgsMPI;

                // set the i-th experiment conditions
                if (LhsUtils::getNetSizeIndex(p_desArgs) >= 0) {
                    desArgsMPI.net_size = static_cast<unsigned int> (
                        round(gsl_matrix_get(
                                  sample, i, LhsUtils::getNetSizeIndex(p_desArgs))));
                } else {
                    desArgsMPI.net_size = p_desArgs->net_size;
                }
                if (LhsUtils::getMaxEdgesIndex(p_desArgs) >= 0) {
                    desArgsMPI.max_edges = static_cast<unsigned int> (
                        round(gsl_matrix_get(
                                  sample, i, LhsUtils::getMaxEdgesIndex(p_desArgs))));
                } else {
                    desArgsMPI.max_edges = p_desArgs->max_edges;
                }
                if (LhsUtils::getEdgeProbIndex(p_desArgs) >= 0) {
                    desArgsMPI.edge_prob = gsl_matrix_get(
                        sample, i, LhsUtils::getEdgeProbIndex(p_desArgs));
                } else {
                    desArgsMPI.edge_prob = p_desArgs->edge_prob;
                }
                if (LhsUtils::getVertexBoostIndex(p_desArgs) >= 0) {
                    desArgsMPI.boost_arrival = gsl_matrix_get(
                        sample, i, LhsUtils::getVertexBoostIndex(p_desArgs));
                } else {
                    desArgsMPI.boost_arrival = p_desArgs->boost_arrival;
                }
                if (LhsUtils::getEdgeBoostIndex(p_desArgs) >= 0) {
                    desArgsMPI.boost_edge = gsl_matrix_get(
                        sample, i, LhsUtils::getEdgeBoostIndex(p_desArgs));
                } else {
                    desArgsMPI.boost_edge = p_desArgs->boost_edge;
                }
                desArgsMPI.sim_num = i + 1;

                for (desArgsMPI.rep_num = 1; desArgsMPI.rep_num <= p_desArgs->replications;
                     ++desArgsMPI.rep_num) {
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

                dio::tResultsSP replica_output(
                    new dio::Results(file, dir));

                csv_line << "sim_num,rep_num,systemDelay,systemAvgNumEvents,meanDelay,varDelay,meanAvgNumEvents,varAvgNumEvents";
                replica_output->print(csv_line);
                csv_line.str("");

                replica_results.push_back(replica_output);

                // prepare the overall results
                sim_results_lines[i] << desArgsMPI.sim_num << "," << p_desArgs->stop_time << ","
                                     << p_desArgs->generations << "," << p_desArgs->confidence << ","
                                     << p_desArgs->alpha << "," << p_desArgs->error << ","
                                     << p_desArgs->replications << "," << desArgsMPI.net_size
                                     << "," << p_desArgs->max_edges << "," << p_desArgs->edge_prob
                                     << "," << p_desArgs->edge_fixed;
            }

            // 4. continue with as many experiments as needed
            while (!areAllSignificant(areExpsSignificant)) {
                sim_output *output = new sim_output[1];
#ifndef NDEBUG
                std::cout << "Receive results." << std::endl;
                std::cout.flush();
#endif /* NDEBUG */
                // receive results
                rc = MPI_Recv(output, 1, mpi_desout, MPI_ANY_SOURCE, MPI_ANY_TAG,
                              MPI_COMM_WORLD, &status);
                if (rc != MPI_SUCCESS) {
                    std::cerr << "Error receiving results from slave." << std::endl;
                    MPI_Abort(MPI_COMM_WORLD, 916);
                }

#ifndef NDEBUG
                std::cout << "Received simulation: " << output->simulation_id << ", replications: "
                          << output->replications << std::endl;
                std::cout.flush();
#endif /* NDEBUG */
                // update fields
                avgDelays[output->simulation_id - 1].push(output->system_average_delay);
                avgNumEvents[output->simulation_id - 1].push(output->system_expected_average_num_in_queue);

                // test whether this result is significant
                if (avgDelays[output->simulation_id - 1].getNumValues()
                    > (p_desArgs->simulations - 1)) {
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
                            p_desArgs->alpha, p_desArgs->error);

                    // if not send another replica
                    if (!isConfident) {
#ifndef NDEBUG
                        std::cout << "Simulation " << output->simulation_id << " is not confident." << std::endl;
                        std::cout.flush();
#endif /* NDEBUG */
                        // update the experiment arguments
                        tSimArgsMPI desArgsMPI;

                        desArgsMPI.rep_num = avgDelays[output->simulation_id - 1].getNumValues() + 1;
                        desArgsMPI.sim_num = output->simulation_id;

                        if (LhsUtils::getNetSizeIndex(p_desArgs) >= 0) {
                            desArgsMPI.net_size = static_cast<unsigned int> (
                                round(gsl_matrix_get(
                                          sample, output->simulation_id - 1, LhsUtils::getNetSizeIndex(p_desArgs))));
                        } else {
                            desArgsMPI.net_size = p_desArgs->net_size;
                        }
                        if (LhsUtils::getMaxEdgesIndex(p_desArgs) >= 0) {
                            desArgsMPI.max_edges = static_cast<unsigned int> (
                                round(gsl_matrix_get(
                                          sample, output->simulation_id - 1, LhsUtils::getMaxEdgesIndex(p_desArgs))));
                        } else {
                            desArgsMPI.max_edges = p_desArgs->max_edges;
                        }
                        if (LhsUtils::getEdgeProbIndex(p_desArgs) >= 0) {
                            desArgsMPI.edge_prob = gsl_matrix_get(
                                sample, output->simulation_id - 1, LhsUtils::getEdgeProbIndex(p_desArgs));
                        } else {
                            desArgsMPI.edge_prob = p_desArgs->edge_prob;
                        }
                        if (LhsUtils::getVertexBoostIndex(p_desArgs) >= 0) {
                            desArgsMPI.boost_arrival = gsl_matrix_get(
                                sample, output->simulation_id - 1, LhsUtils::getVertexBoostIndex(p_desArgs));
                        } else {
                            desArgsMPI.boost_arrival = p_desArgs->boost_arrival;
                        }
                        if (LhsUtils::getEdgeBoostIndex(p_desArgs) >= 0) {
                            desArgsMPI.boost_edge = gsl_matrix_get(
                                sample, output->simulation_id - 1, LhsUtils::getEdgeBoostIndex(p_desArgs));
                        } else {
                            desArgsMPI.boost_edge = p_desArgs->boost_edge;
                        }

                        rc = MPI_Send(&desArgsMPI, 1, mpi_desargs, status.MPI_SOURCE, jobs, MPI_COMM_WORLD);
                        if (rc != MPI_SUCCESS) {
                            std::cerr << "Error sending task to slave." << std::endl;
                            MPI_Abort(MPI_COMM_WORLD, 916);
                        }
                        jobs++;
                    } else {
#ifndef NDEBUG
                        std::cout << "Simulation " << output->simulation_id << " is confident." << std::endl;
                        std::cout.flush();
#endif /* NDEBUG */
                        areExpsSignificant[output->simulation_id - 1] = true;

                        // write the overall results
                        sim_results_lines[output->simulation_id - 1] << output->replications;
                    }
                }

                // write replica results
                csv_line.str("");
                csv_line << output->simulation_id << "," << output->replications << ","
                         << output->system_average_delay << ","
                         << output->system_expected_average_num_in_queue
                         << "," << avgDelays[output->simulation_id - 1].mean() << ","
                         << avgDelays[output->simulation_id - 1].variance() << ","
                         << avgNumEvents[output->simulation_id - 1].mean() << ","
                         << avgNumEvents[output->simulation_id - 1].variance();
                replica_results[output->simulation_id - 1]->print(csv_line);

                delete[] output;
            }

            for (boost::uint16_t i = 0; i < p_desArgs->simulations; ++i) {
                sim_results->print(sim_results_lines[i]);
            }

            // 5. free gsl stuff
            gsl_vector_free(min);
            gsl_vector_free(max);
            gsl_matrix_free(sample);
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

    static bool areAllSignificant(std::vector<bool> expSignificant)
        {
            std::vector<bool>::iterator
                it(expSignificant.begin()),
                it_end(expSignificant.end());

            for (; it != it_end; ++it) {
                if (!(*it)) {
                    return false;
                }
            }

            return true;
        }
};


}
}



#endif /* __DES_CORE_SIMULATIONMPI_HH__ */
