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
#if HAVE_CONFIG_H
# include <config.h>
#endif

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
    opt_desc = tOptDescSP(new po::options_description());

    // Declare the supported options.
    po::options_description opt_general("General Configuration");
    opt_general.add_options()
        (HELP.c_str(), "produce help message")
        (VERS.c_str(), "show the version")
        ;

    po::options_description opt_app("Application Configuration");
    opt_app.add_options()
        (STOPTIME.c_str(), po::value <boost::uint32_t>()->default_value(100), "set the stop time of the event simulator.")
        (GENERATIONS.c_str(), po::value <boost::int32_t>()->default_value(-1), "set the number of generations for the event simulator.")
        (GRAPH.c_str(), po::value <std::string>(), "set the graph for the event simulator.")
        (SEEDS.c_str(), po::value <std::string>(), "set the seeds for the event simulator.")
        (RESULTS.c_str(), po::value <std::string>()->default_value("./results"), "set directory for the results of the event simulator.")
        (LOG_GRAPH_RATE.c_str(), po::value <boost::int32_t>()->default_value(0), "set the graph generation rate.")
        (LOG_EVENTS.c_str(), po::value <bool>()->default_value(true), "log the events.")
        ;

    po::options_description opt_debug("Debug Configuration");
    opt_debug.add_options()
        (TRACE.c_str(), po::value <bool>()->default_value(false), "Set debugging.")
        (VERTEX.c_str(), po::value <boost::int32_t>(), "The source vertex to trace the event for.")
        ;

    opt_desc->add(opt_general);
    opt_desc->add(opt_app);
    opt_desc->add(opt_debug);
}


int CL::parse(int argc, char *argv[], tDesArgsSP desArgs)
{
    po::variables_map vm;

    po::store(po::parse_command_line(argc, argv, (*opt_desc.get())), vm);
    po::notify(vm);

    if (vm.count(HELP)) {
        std::cout << (*opt_desc.get()) << std::endl;
        return EXIT_FAILURE;
    }

    if (vm.count(VERS)) {
        std::cout << argv[0] << " " << PACKAGE_VERSION << std::endl;
        std::cout << PACKAGE_NAME << std::endl;
        return EXIT_FAILURE;
    }

    if (vm.count(STOPTIME.c_str())) {
        desArgs->stop_time = vm[STOPTIME.c_str()].as <boost::uint32_t>();
        std::cout << "Stopping time set to " << desArgs->stop_time << "." << std::endl;
    } else {
        std::cout << "Default stopping time is " << desArgs->stop_time << "." << std::endl;
    }

    if (vm.count(GENERATIONS.c_str())) {
        desArgs->generations = vm[GENERATIONS.c_str()].as <boost::int32_t>();
        std::cout << "Number of generations set to " << desArgs->generations << "." << std::endl;
    } else {
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
    }
    std::cout << "Set the results directory "
              << desArgs->results_dir << "." << std::endl;

    if (vm.count(LOG_GRAPH_RATE.c_str())) {
        desArgs->graph_rate = vm[LOG_GRAPH_RATE.c_str()].as <boost::int32_t>();
    }
    std::cout << "Set the graph generation rate to "
              << desArgs->graph_rate << "." << std::endl;

    if (vm.count(LOG_EVENTS.c_str())) {
        desArgs->log_events = vm[LOG_EVENTS.c_str()].as <bool>();
    }
    std::cout << "Log the events "
              << desArgs->log_events << "." << std::endl;

    if (vm.count(TRACE.c_str())) {
        desArgs->trace_event = vm[TRACE.c_str()].as <bool>();
    }

    if (desArgs->trace_event) {
        if (vm.count(VERTEX.c_str())) {
            desArgs->vertex = vm[VERTEX.c_str()].as <boost::int32_t>();
            std::cout << std::endl << "Trace vertex " << desArgs->vertex << "." << std::endl;
        } else {
            std::cout << "A vertex needs to be specified to trace the event." << std::endl;
            return EXIT_FAILURE;
        }
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
