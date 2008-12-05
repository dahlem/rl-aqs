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

#include "common.hh"
#include "CL.hh"
#include "Simulation.hh"
#include "SimulationCI.hh"
#include "SimulationLHS.hh"
namespace dcore = des::core;

#include "Seeds.hh"
namespace dsample = des::sampling;


typedef dcore::SimulationCI <dcore::SimSP> SimCI;
typedef boost::shared_ptr <SimCI> SimCISP;


int main(int argc, char *argv[])
{
    dcore::tDesArgsSP desArgs(new dcore::desArgs_t);
    dcore::CL cl;


    if (cl.parse(argc, argv, desArgs)) {
        return EXIT_SUCCESS;
    }

    if (desArgs->seeds_filename != "") {
        // read the seeds
        dsample::Seeds::getInstance().init(desArgs->seeds_filename.c_str());
    } else {
        // generate the seeds
        std::cout << "Use random number to generate seeds." << std::endl;
        dsample::Seeds::getInstance().init();
    }

    if (desArgs->confidence) {
        dcore::SimSP sim(new dcore::Simulation());
        SimCISP sim_ci(new SimCI(sim, 0.95, 0.1));
        sim_ci->simulate(desArgs);
    } else {
        dcore::SimSP sim(new dcore::Simulation());
        sim->simulate(desArgs);
    }

    return EXIT_SUCCESS;
}
