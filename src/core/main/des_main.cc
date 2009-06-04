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

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <iostream>
#include <sstream>

#ifndef NDEBUG
# include <cassert>
# include <boost/assert.hpp>
#endif /* NDEBUG */

#ifdef HAVE_MPI
# include <mpi.h>
#endif /* HAVE_MPI */

#ifdef HAVE_OPENMP
# include <omp.h>
#endif /* HAVE_OPENMP */

#ifdef HAVE_LIBGSL
# include <gsl/gsl_math.h>
# include <gsl/gsl_ieee_utils.h>
#endif /* HAVE_LIBGSL */


#include "common.hh"
#include "CL.hh"
#include "Simulation.hh"

#ifdef HAVE_MPI
# include "SimulationMPI.hh"
#else
# include "SimulationCI.hh"
# include "SimulationLHS.hh"
#endif /* HAVE_MPI */
namespace dcore = des::core;

#include "CurDate.hh"
namespace ddate = des::date;

#include "Seeds.hh"
namespace dsample = des::sampling;

#include "Results.hh"
namespace dio = des::io;


#ifndef HAVE_MPI
typedef dcore::SimulationCI <dcore::Simulation> SimCI;
typedef dcore::SimulationLHS <SimCI> SimLHS;
#else
void register_simargs(MPI_Datatype &MPI_Simargs, dcore::tSimArgsMPI &value)
{
    int          blocklens[2];
    MPI_Aint     indices[2];
    MPI_Datatype old_types[2];

    /* One value of each type */
    blocklens[0] = 4;
    blocklens[1] = 7;

    /* The base types */
    old_types[0] = MPI_UNSIGNED;
    old_types[1] = MPI_DOUBLE;

    /* The locations of each element */
    MPI_Address(&value.sim_num, &indices[0]);
    MPI_Address(&value.edge_prob, &indices[1]);

    /* Make relative */
    indices[1] = indices[1] - indices[0];
    indices[0] = 0;

    MPI_Type_struct(2, blocklens, indices, old_types, &MPI_Simargs);
}


void register_simout(MPI_Datatype &MPI_Desout, dcore::sim_output &value)
{
    int          blocklens[2];
    MPI_Aint     indices[2];
    MPI_Datatype old_types[2];

    /* One value of each type */
    blocklens[0] = 9;
    blocklens[1] = 2;

    /* The base types */
    old_types[0] = MPI_DOUBLE;
    old_types[1] = MPI_UNSIGNED;

    /* The locations of each element */
    MPI_Address(&value.system_average_delay, &indices[0]);
    MPI_Address(&value.replications, &indices[1]);

    /* Make relative */
    indices[1] = indices[1] - indices[0];
    indices[0] = 0;

    MPI_Type_struct(2, blocklens, indices, old_types, &MPI_Desout);
}
#endif /* HAVE_MPI */




int main(int argc, char *argv[])
{
#ifdef HAVE_LIBGSL
    /* read GSL_IEEE_MODE */
    gsl_ieee_env_setup();
#endif /* HAVE_LIBGSL */

    dcore::tDesArgsSP desArgs(new dcore::desArgs_t);
    char dateCStr[128];

#ifdef HAVE_MPI
    int rank, num_tasks, rc;
    MPI_Datatype MPI_Simargs, MPI_Desout;
    dcore::sim_output outVal;
    dcore::tSimArgsMPI desArgsMPI;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);

    // register the structs
# ifndef NDEBUG
    std::cout << "Register the structs with MPI." << std::endl;
# endif /* NDEBUG */

    register_simargs(MPI_Simargs, desArgsMPI);
    register_simout(MPI_Desout, outVal);
    MPI_Type_commit(&MPI_Simargs);
    MPI_Type_commit(&MPI_Desout);
#endif /* HAVE_MPI */

    dcore::CL cl;

    if (cl.parse(argc, argv, desArgs)) {
        return EXIT_SUCCESS;
    }

    // set the basedir for the results
    std::stringstream baseDir;

    // if we start a new experiment we'll create a new date string
    std::string dateStr;

    // otherwise use the one supplied on the command-line
    if (desArgs->add_sim.empty()) {
        dateStr = ddate::CurDateSingleton::getInstance().get();
    } else {
        dateStr = desArgs->add_sim;
    }

    strncpy(dateCStr, dateStr.c_str(), 128);

#ifdef HAVE_MPI
    rc = MPI_Bcast(dateCStr, 128, MPI_CHAR, 0, MPI_COMM_WORLD);
    if (rc != MPI_SUCCESS) {
        std::cerr << "Error broadcasting the date string." << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 915);
    }
#endif /* HAVE_MPI */

    baseDir << desArgs->results_dir << "/" << dateCStr;
    desArgs->results_dir = baseDir.str();

# ifndef NDEBUG
    std::cout << "Base directory set to: " << desArgs->results_dir << std::endl;
# endif /* NDEBUG */

    desArgs->sim_num = 1;
    desArgs->rep_num = 1;

#ifdef HAVE_MPI
    if (rank == 0) {

        // check whether we have enough nodes
        if ((desArgs->simulations * desArgs->replications + 1) > num_tasks) {
            std::cerr << "Error: Not enough nodes available!" << std::endl;
            MPI_Abort(MPI_COMM_WORLD, 911);
        }
    }

    if (desArgs->seeds_filename != "") {
# ifndef NDEBUG
        std::cout << "Init the seeds" << std::endl;
# endif /* NDEBUG */

        // read the seeds
        dsample::Seeds::getInstance().init(desArgs->seeds_filename.c_str());
    } else {
        std::cerr << "Error: Seed file needs to be present!" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 912);
    }
#else
    if (desArgs->seeds_filename != "") {
        // read the seeds
        dsample::Seeds::getInstance().init(desArgs->seeds_filename.c_str());
    } else {
        // generate the seeds
        std::cout << "Use random number to generate seeds." << std::endl;
        dsample::Seeds::getInstance().init();
    }
#endif /* HAVE_MPI */

#ifdef HAVE_MPI
    if (rank == 0) {
        if (desArgs->lhs && desArgs->confidence) {
# ifndef NDEBUG
            std::cout << "Start master." << std::endl;
# endif /* NDEBUG */
            // master node
            dcore::SimulationMPI::simulate(desArgs, MPI_Simargs, MPI_Desout);

            // send kill pill
            for(int i = 1; i < num_tasks; ++i) {
# ifndef NDEBUG
                std::cout << "Send kill pill to " << i << std::endl;
# endif /* NDEBUG */
                /* Send no more jobs */
                MPI_Send(0, 0, MPI_UNSIGNED_SHORT, i, KILL_PILL, MPI_COMM_WORLD);
            }
        }
    } else {
# ifndef NDEBUG
        std::cout << "Start slave: " << rank << std::endl;
# endif /* NDEBUG */
        // slave node
        dcore::Simulation sim;
        sim.simulate(MPI_Simargs, MPI_Desout, desArgs);
    }
#else
    dcore::sim_output output;
    if (desArgs->lhs && desArgs->confidence) {
        SimCI simCI(desArgs->alpha, desArgs->error, desArgs->replications);
        SimLHS simLHS(simCI);
        output = simLHS.simulate(desArgs);
    } else if (desArgs->confidence) {
        SimCI simCI(desArgs->alpha, desArgs->error, desArgs->replications);
        output = simCI.simulate(desArgs);
    } else {
        dcore::Simulation sim;
        output = sim(desArgs);
    }

    if (!desArgs->lhs) {
        std::stringstream outDir, csv_line;
        outDir << desArgs->results_dir << "/";

        std::string dir = outDir.str();
        std::string file = "simulations.dat";

        dio::Results sim_results(file, dir);

        if (desArgs->add_sim.empty()) {
            csv_line << "sim_num," << dcore::ARGS_HEADER << ",actual_reps,meanDelay,varDelay,meanAvgNumEvents,varAvgNumEvents,meanAvgEventInSystem,varAvgEventInSystem";
            sim_results.print(csv_line);
        }

        csv_line.str("");
        csv_line << desArgs->sim_num << ","
                 << const_cast <const dcore::desArgs_t&> (*desArgs)
                 << "," << output.replications
                 << "," << output.mean_system_average_delay
                 << "," << output.sd_system_average_delay
                 << "," << output.mean_system_expected_average_num_in_queue
                 << "," << output.sd_system_expected_average_num_in_queue
                 << "," << output.mean_system_total_q
                 << "," << output.sd_system_total_q;
        sim_results.print(csv_line);
    }
#endif /* HAVE_MPI */

#ifdef HAVE_MPI
# ifndef NDEBUG
    std::cout << "Closing shared seed file." << std::endl;
# endif /* NDEBUG */
    dsample::Seeds::getInstance().close();

# ifndef NDEBUG
    std::cout << "Free up MPI resources." << std::endl;
# endif /* NDEBUG */
    MPI_Type_free(&MPI_Simargs);
    MPI_Type_free(&MPI_Desout);
    MPI_Finalize();
#endif /* HAVE_MPI */

    return EXIT_SUCCESS;
}
