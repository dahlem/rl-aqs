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
#include "SimulationCI.hh"
#include "SimulationLHS.hh"
namespace dcore = des::core;

#include "CurDate.hh"
namespace ddate = des::date;

#include "Seeds.hh"
namespace dsample = des::sampling;

#include "Results.hh"
namespace dio = des::io;


typedef dcore::SimulationCI <dcore::SimSP> SimCI;
typedef boost::shared_ptr <SimCI> SimCISP;

typedef dcore::SimulationLHS <SimCISP> SimLHS;
typedef boost::shared_ptr <SimLHS> SimLHSSP;


int main(int argc, char *argv[])
{
#ifdef HAVE_MPI
    int rank, num_tasks;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);
#endif /* HAVE_MPI */

    dcore::tDesArgsSP desArgs(new dcore::desArgs_t);
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
    if (desArgs->seeds_filename != "") {
        // read the seeds
        dsample::Seeds::getInstance().init(desArgs->seeds_filename.c_str());
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

#ifdef HAVE_MPI
    MPI_Barrier(MPI_COMM_WORLD);
    dsample::Seeds::getInstance().close();
    MPI_Finalize();
#endif /* HAVE_MPI */

    return EXIT_SUCCESS;
}
