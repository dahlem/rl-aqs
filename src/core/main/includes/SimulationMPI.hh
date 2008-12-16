// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

#include "common.hh"
#include "CL.hh"

#include "Results.hh"
namespace dio = des::io;

#include "CRN.hh"
#include "Seeds.hh"
#include "LHS.hh"
namespace dsample = des::sampling;

#include "CI.hh"
#include "OnlineStats.hh"
namespace dstats = des::statistics;



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
    static void simulate(tDesArgsSP p_desArgs, MPI_Datatype mpi_desargs, MPI_Datatype mpi_desout)
        {
            MPI_Status status;
            sim_output output;
            int jobs = 0;
            
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

            min = gsl_vector_calloc(1);
            max = gsl_vector_calloc(1);
            gsl_vector_set(min, 0, p_desArgs->min_stop_time);
            gsl_vector_set(max, 0, p_desArgs->max_stop_time);

            dsample::LHS::sample(rng.get(), min, max, p_desArgs->simulations, &sample);

            // 3. run experiment
            for (boost::uint16_t i = 0; i < p_desArgs->simulations; ++i) {
                // start initial number of experiments
                // copy the desArgs into the mpi desargs
                tDesArgsMPI desArgsMPI;
                copy(p_desArgs, desArgsMPI);

                // set the i-th experiment conditions
                desArgsMPI.stop_time = gsl_matrix_get(sample, i, 0);
                desArgsMPI.sim_num = i + 1;
                
                for (desArgsMPI.rep_num = 1; desArgsMPI.rep_num < p_desArgs->replications; ++desArgsMPI.rep_num) {
                    // send the desargs to the slave nodes
                    MPI_Send(&desArgsMPI, mpi_desargs, 1, desArgsMPI.sim_num, jobs, MPI_COMM_WORLD);
                    jobs++;
                }
            }

            // 4. continue with as many experiments as needed
            while (!areAllSignificant(areExpsSignificant)) {
                // receive results
                MPI_Recv(&output, mpi_desout, 1, MPI_ANY_SOURCE, MPI_ANY_TAG,
                         MPI_COMM_WORLD, &status);

                // update fields
                avgDelays[status.MPI_SOURCE - 1].push(output.system_average_delay);
                avgNumEvents[status.MPI_SOURCE - 1].push(output.system_expected_average_num_in_queue);
                
                // test whether this result is significant
                if (avgDelays[status.MPI_SOURCE - 1].getNumValues() > 2) {
                    bool isConfident =
                        dstats::CI::isConfidentWithPrecision(
                            avgDelays[status.MPI_SOURCE - 1].mean(),
                            avgDelays[status.MPI_SOURCE - 1].variance(),
                            avgDelays[status.MPI_SOURCE - 1].getNumValues(),
                            p_desArgs->alpha, p_desArgs->error)
                        &&
                        dstats::CI::isConfidentWithPrecision(
                            avgNumEvents[status.MPI_SOURCE - 1].mean(),
                            avgNumEvents[status.MPI_SOURCE - 1].variance(),
                            avgNumEvents[status.MPI_SOURCE - 1].getNumValues(),
                            p_desArgs->alpha, p_desArgs->error);
                    
                    // if not send another replica
                    if (!isConfident) {
                        // update the experiment arguments
                        tDesArgsMPI desArgsMPI;
                        copy(p_desArgs, desArgsMPI);
                        desArgsMPI.rep_num = avgDelays[status.MPI_SOURCE - 1].getNumValues() + 1;
                        desArgsMPI.sim_num = status.MPI_SOURCE;
                        desArgsMPI.stop_time = gsl_matrix_get(
                            sample, status.MPI_SOURCE - 1, 0);

                        MPI_Send(&desArgsMPI, mpi_desargs, 1, desArgsMPI.sim_num, jobs, MPI_COMM_WORLD);
                        jobs++;
                    } else {
                        areExpsSignificant[status.MPI_SOURCE - 1] = true;
                    }
                }
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

    static void copy(tDesArgsSP desArgs, tDesArgsMPI &desArgsMPI) 
        {
            // copy the cl args over to the mpi args
            desArgsMPI.replications = desArgs->replications;
            desArgsMPI.simulations = desArgs->simulations;
            desArgsMPI.sim_num = desArgs->sim_num;
            desArgsMPI.rep_num = desArgs->rep_num;

            desArgsMPI.stop_time = desArgs->stop_time;
            desArgsMPI.min_stop_time = desArgs->min_stop_time;
            desArgsMPI.max_stop_time = desArgs->max_stop_time;
            desArgsMPI.alpha = desArgs->alpha;
            desArgsMPI.error = desArgs->error;

            desArgsMPI.vertex = desArgs->vertex;
            desArgsMPI.graph_rate = desArgs->graph_rate;
            desArgsMPI.generations = desArgs->generations;

            desArgsMPI.trace_event = desArgs->trace_event;
            desArgsMPI.log_events = desArgs->log_events;
            desArgsMPI.confidence = desArgs->confidence;
            desArgsMPI.lhs = desArgs->lhs;

            strncpy(desArgsMPI.graph_filename, desArgs->graph_filename.c_str(), 256);
            strncpy(desArgsMPI.seeds_filename, desArgs->seeds_filename.c_str(), 256);
            strncpy(desArgsMPI.results_dir, desArgs->results_dir.c_str(), 256);
            strncpy(desArgsMPI.events_unprocessed, desArgs->events_unprocessed.c_str(), 256);
            strncpy(desArgsMPI.events_unprocessed, desArgs->events_unprocessed.c_str(), 256);
        }
    
};


}
}



#endif /* __DES_CORE_SIMULATIONMPI_HH__ */
