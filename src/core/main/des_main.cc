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

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <iostream>
#include <sstream>

#ifdef HAVE_MPI
# include <mpi.h>
#endif /* HAVE_MPI */

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
typedef dcore::SimulationCI <dcore::SimSP> SimCI;
typedef boost::shared_ptr <SimCI> SimCISP;

typedef dcore::SimulationLHS <SimCISP> SimLHS;
typedef boost::shared_ptr <SimLHS> SimLHSSP;
#else
void register_desargs(MPI_Datatype MPI_Desargs, dcore::tDesArgsMPI value)
{
    int          blocklens[5];
    MPI_Aint     indices[5];
    MPI_Datatype old_types[5];

    /* One value of each type */
    blocklens[0] = 4;
    blocklens[1] = 5;
    blocklens[2] = 3;
    blocklens[3] = 4;
    blocklens[4] = 1280;

    /* The base types */
    old_types[0] = MPI_UNSIGNED;
    old_types[1] = MPI_DOUBLE;
    old_types[2] = MPI_INT;
    old_types[3] = MPI_SHORT;
    old_types[4] = MPI_CHAR;

    /* The locations of each element */
    MPI_Address(&value.replications, &indices[0]);
    MPI_Address(&value.stop_time, &indices[1]);
    MPI_Address(&value.vertex, &indices[2]);
    MPI_Address(&value.trace_event, &indices[3]);
    MPI_Address(&value.graph_filename, &indices[4]);

    /* Make relative */
    indices[4] = indices[4] - indices[0];
    indices[3] = indices[3] - indices[0];
    indices[2] = indices[2] - indices[0];
    indices[1] = indices[1] - indices[0];
    indices[0] = 0;

    MPI_Type_struct(5, blocklens, indices, old_types, &MPI_Desargs);
}


void register_desout(MPI_Datatype MPI_Desout, dcore::sim_output value)
{
    int          blocklens[2];
    MPI_Aint     indices[2];
    MPI_Datatype old_types[2];

    /* One value of each type */
    blocklens[0] = 6;
    blocklens[1] = 1;

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
    dcore::tDesArgsSP desArgs(new dcore::desArgs_t);

#ifdef HAVE_MPI
    int rank, num_tasks;
    MPI_Datatype MPI_Desargs, MPI_Desout;
    dcore::sim_output outVal;
    dcore::tDesArgsMPI desArgsMPI;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);

    // register the structs
    register_desargs(MPI_Desargs, desArgsMPI);
    register_desout(MPI_Desout, outVal);
    MPI_Type_commit(&MPI_Desargs);
    MPI_Type_commit(&MPI_Desout);

    if (rank == 0) {

#endif /* HAVE_MPI */

        dcore::CL cl;

        if (cl.parse(argc, argv, desArgs)) {
            return EXIT_SUCCESS;
        }

        // set the basedir for the results
        std::stringstream baseDir;
        baseDir << desArgs->results_dir << "/";
        baseDir << ddate::CurDateSingleton::getInstance().get();
        desArgs->results_dir = baseDir.str();

        desArgs->sim_num = 1;
        desArgs->rep_num = 1;

#ifdef HAVE_MPI

        // check whether we have enough nodes
        if ((desArgs->simulations * desArgs->replications + 1) > num_tasks) {
            MPI_Abort(MPI_COMM_WORLD, 911);
        }

        if (desArgs->seeds_filename != "") {
            // read the seeds
            dsample::Seeds::getInstance().init(desArgs->seeds_filename.c_str());
        }
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
            // master node
            dcore::SimulationMPI::simulate(desArgs, MPI_Desargs, MPI_Desout);

            // send kill pill
            for(int i = 1; i < num_tasks; ++i) {
                /* Send no more jobs */
                MPI_Send(0, 0, MPI_UNSIGNED_SHORT, i, KILL_PILL, MPI_COMM_WORLD);
            }
        }
    } else {
        // slave node
        dcore::SimSP sim(new dcore::Simulation());
        sim->simulate(MPI_Desargs, MPI_Desout);
    }
#else
    dcore::sim_output output;
    if (desArgs->lhs && desArgs->confidence) {
        dcore::SimSP sim(new dcore::Simulation());
        SimCISP sim_ci(
            new SimCI(sim, desArgs->alpha, desArgs->error, desArgs->replications));
        SimLHSSP sim_lhs(new SimLHS(sim_ci));
        output = sim_lhs->simulate(desArgs);
    } else if (desArgs->confidence) {
        dcore::SimSP sim(new dcore::Simulation());
        SimCISP sim_ci(
            new SimCI(sim, desArgs->alpha, desArgs->error, desArgs->replications));
        output = sim_ci->simulate(desArgs);
    } else {
        dcore::SimSP sim(new dcore::Simulation());
        output = sim->simulate(desArgs);
    }

    if (!desArgs->lhs) {
        std::stringstream outDir, csv_line;
        outDir << desArgs->results_dir << "/";

        std::string dir = outDir.str();
        std::string file = "simulations.dat";

        dio::tResultsSP sim_results(
            new dio::Results(file, dir));

        csv_line << "sim_num," << dcore::ARGS_HEADER << ",actual_reps";
        sim_results->print(csv_line);

        csv_line.str("");
        csv_line << desArgs->sim_num << "," << const_cast <const dcore::desArgs_t&> (*desArgs)
                 << "," << output.replications;
        sim_results->print(csv_line);
    }
#endif /* HAVE_MPI */

#ifdef HAVE_MPI
    MPI_Barrier(MPI_COMM_WORLD);

    dsample::Seeds::getInstance().close();

    MPI_Type_free(&MPI_Desargs);
    MPI_Type_free(&MPI_Desout);
    MPI_Finalize();
#endif /* HAVE_MPI */

    return EXIT_SUCCESS;
}
