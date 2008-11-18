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

/** @file CL.cc
 * Implementation of the command-line parsing of the main routine for DES.
 *
 * @author Dominik Dahlem
 */
#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <iostream>
#include <string>

#include <boost/cstdint.hpp>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>
namespace po = boost::program_options;

#include "CL.hh"
using des::core::CL;
using des::core::tDesArgsSP;


CL::CL()
{
    // Declare the supported options.
    opt_desc = tOptDescSP(new po::options_description("General Configuration"));
    opt_desc->add_options()
        (HELP.c_str(), "produce help message")
        (STOPTIME.c_str(), po::value <boost::uint32_t>(), "set the stop time of the event simulator.")
        (GENERATIONS.c_str(), po::value <boost::int32_t>(), "set the number of generations for the event simulator (default -1).")
        (GRAPH.c_str(), po::value <std::string>(), "set the graph for the event simulator.")
        (SEEDS.c_str(), po::value <std::string>(), "set the seeds for the event simulator.")
        (RESULTS.c_str(), po::value <std::string>(), "set directory for the results of the event simulator.")
        ;
}


int CL::parse(int argc, char *argv[], tDesArgsSP desArgs)
{
    po::variables_map vm;

    std::cout << std::endl << "Parsing the command-line..." << std::endl;

    po::store(po::parse_command_line(argc, argv, (*opt_desc.get())), vm);
    po::notify(vm);

    if (vm.count(HELP)) {
        std::cout << (*opt_desc.get()) << std::endl;
        return EXIT_FAILURE;
    }

    if (vm.count(STOPTIME.c_str())) {
        desArgs->stop_time = vm[STOPTIME.c_str()].as <boost::uint32_t>();
        std::cout << "Stopping time set to " << desArgs->stop_time << "." << std::endl;
    } else {
        desArgs->stop_time = 100;
        std::cout << "Default stopping time is " << desArgs->stop_time << "." << std::endl;
    }

    if (vm.count(GENERATIONS.c_str())) {
        desArgs->generations = vm[GENERATIONS.c_str()].as <boost::int32_t>();
        std::cout << "Number of generations set to " << desArgs->generations << "." << std::endl;
    } else {
        desArgs->generations = -1;
        std::cout << "No generations." << std::endl;
    }

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

    if (vm.count(RESULTS.c_str())) {
        desArgs->results_dir = vm[RESULTS.c_str()].as <std::string>();
        std::cout << "Set the results directory "
                  << desArgs->results_dir << "." << std::endl;
    } else {
        desArgs->results_dir = "./results";
        std::cout << "Set the results directory "
                  << desArgs->results_dir << "." << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Output Files:" << std::endl;

    desArgs->events_unprocessed = "events_unprocessed.dat";
    desArgs->events_processed = "events_processed.dat";

    std::cout << "Unprocessed events\t" << desArgs->events_unprocessed << std::endl;
    std::cout << "Processed events\t" << desArgs->events_processed << std::endl;

    std::cout << "******************************" << std::endl << std::endl;

    return EXIT_SUCCESS;
}
