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
#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <iostream>
#include <string>

#include <boost/shared_ptr.hpp>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>
namespace po = boost::program_options;

#include "CRN.hh"
#include "Seeds.hh"
namespace dsample = des::sampling;

#include "GraphUtil.hh"
#include "DirectedGraph.hh"
#include "WEvonet.hh"
namespace dnet =  des::network;


const std::string HELP = "help";
const std::string SEEDS = "seeds";
const std::string SIZE = "size";
const std::string MAX_EDGES = "max_edges";
const std::string FILENAME = "filename";
const std::string FORMAT = "format";
const std::string WEIGHT_FIXED = "weight";
const std::string EDGE_PROB = "edge_prob";
const std::string GENERATOR = "generator";
const std::string MAX_ARRIVAL = "max_arrival";



int main(int argc, char *argv[])
{
    int net_size;
    int max_edges;
    int format, format_temp;
    int net_gen = 1;
    double edge_fixed = -1.0;
    double edge_prob = 0.05;
    double max_arrival;

    dsample::tGslRngSP r1, r2, r3;
    std::string filename;
    std::string seeds_file;

    // Declare the supported options.
    po::options_description opt_desc = po::options_description();

    po::options_description desc("General Configuration");
    desc.add_options()
        (HELP.c_str(), "produce help message")
        (SIZE.c_str(), po::value<int>(), "set the size of the network")
        (FILENAME.c_str(), po::value<std::string>(), "set the filename for the graph output")
        (SEEDS.c_str(), po::value <std::string>(), "set the seeds for the event simulator.")
        (FORMAT.c_str(), po::value <int>(), "set the output format (1=dot, 2=graphML).")
        (GENERATOR.c_str(), po::value <int>()->default_value(1), "Network generator (1=BBV, 2=Erdoes-Renyi).")
        (MAX_ARRIVAL.c_str(), po::value <double>()->default_value(1), "Maximum arrival rate.")
        ;

    po::options_description desc_soc("Social Network Configuration");
    desc_soc.add_options()
        (MAX_EDGES.c_str(), po::value<int>(), "set the maximum number of edges to connect a new vertex")
        (WEIGHT_FIXED.c_str(), po::value <double>()->default_value(-1.0), "fix the edge weights (-1=dont't fix).")
        ;

    po::options_description desc_rand("Erdoes-Renyi Network Configuration");
    desc_rand.add_options()
        (EDGE_PROB.c_str(), po::value <double>()->default_value(0.05), "probability of having an edge (u,v).")
        ;

    opt_desc.add(desc);
    opt_desc.add(desc_soc);
    opt_desc.add(desc_rand);

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, opt_desc), vm);
    po::notify(vm);

    if (vm.count(HELP.c_str())) {
        std::cout << opt_desc << std::endl;
        return EXIT_SUCCESS;
    }

    if (vm.count(SIZE.c_str())) {
        std::cout << "Size of network set to "
                  << vm[SIZE.c_str()].as<int>() << "." << std::endl;
        net_size = vm[SIZE.c_str()].as<int>();
    } else {
        std::cout << "Default network size is 10." << std::endl;
        net_size = 10;
    }

    if (vm.count(MAX_EDGES.c_str())) {
        std::cout << "Maximum number of edges is set to "
                  << vm[MAX_EDGES.c_str()].as<int>() << "." << std::endl;
        max_edges = vm[MAX_EDGES.c_str()].as<int>();
    } else {
        std::cout << "Default maximum number of edges is to MAX_edges." << std::endl;
        max_edges = dnet::WEvonet::MAX_EDGES;
    }

    if (vm.count(FORMAT.c_str())) {
        format_temp = vm[FORMAT.c_str()].as<int>();

        if (format_temp == 1) {
            std::cout << "Use the dot output format." << std::endl;
            format = dnet::GraphUtil::GRAPHVIZ;
        } else {
            std::cout << "Use the GraphML output format." << std::endl;
            format = dnet::GraphUtil::GRAPHML;
        }
    } else {
        std::cout << "Use the GraphML output format." << std::endl;
        format = dnet::GraphUtil::GRAPHML;
    }

    if (vm.count(FILENAME.c_str())) {
        std::cout << "Set the filename to "
                  << vm[FILENAME.c_str()].as<std::string>() << "." << std::endl;
        filename = vm[FILENAME.c_str()].as<std::string>();
    } else {
        std::cout << "Default filename is test.gml." << std::endl;
        filename = "test.gml";
    }

    if (vm.count(SEEDS.c_str())) {
        seeds_file = vm[SEEDS.c_str()].as <std::string>();
        if (fs::exists(seeds_file)) {
            std::cout << "Read seeds from filename "
                      << seeds_file << "." << std::endl;
        } else {
            std::cerr << "Error: Seeds file " << seeds_file
                      << " does not exist!" << std::endl;

            return EXIT_FAILURE;
        }
    }
    if (vm.count(MAX_ARRIVAL.c_str())) {
        max_arrival = vm[MAX_ARRIVAL.c_str()].as <double>();
    }
    std::cout << "The max. arrival rate is set to "
              << max_arrival << "." << std::endl;

    if (vm.count(WEIGHT_FIXED.c_str())) {
        edge_fixed = vm[WEIGHT_FIXED.c_str()].as <double>();
    }
    std::cout << "Fix the edge weight at "
              << edge_fixed << "." << std::endl;

    if (vm.count(EDGE_PROB.c_str())) {
        edge_prob = vm[EDGE_PROB.c_str()].as <double>();
    }
    std::cout << "The probability of having in edge (u,v) is "
              << edge_prob << " (only for ER graphs)." << std::endl;

    if (vm.count(GENERATOR.c_str())) {
        net_gen = vm[GENERATOR.c_str()].as <int>();
    }
    std::cout << "Generate network type "
              << net_gen << "." << std::endl;

    boost::int32_t arrival_rng_index;
    boost::int32_t seeds_rng_index;
    boost::int32_t uniform_rng_index;
    boost::int32_t num_edges_rng_index;
    boost::uint32_t seed = 0;

    if (seeds_file != "") {
        // read the seeds
        dsample::Seeds::getInstance().init(seeds_file.c_str());

        // init the crn for the arrival events
        seed = dsample::Seeds::getInstance().getSeed();
        arrival_rng_index = dsample::CRN::getInstance().init(seed);
        dsample::CRN::getInstance().log(seed, "vertex arrival rate");
        seed = dsample::Seeds::getInstance().getSeed();
        uniform_rng_index = dsample::CRN::getInstance().init(seed);
        dsample::CRN::getInstance().log(seed, "uniform");
        seed = dsample::Seeds::getInstance().getSeed();
        num_edges_rng_index = dsample::CRN::getInstance().init(seed);
        dsample::CRN::getInstance().log(seed, "number of edges");
    } else {
        // generate the seeds
        std::cout << "Use random number to generate seeds." << std::endl;

        // 1. init the random number generator for the seeds
        seeds_rng_index = dsample::CRN::getInstance().init(gsl_rng_default_seed);
        dsample::CRN::getInstance().log(gsl_rng_default_seed, "seeds");

        dsample::tGslRngSP seeds_rng = dsample::CRN::getInstance().get(seeds_rng_index);

        // 2. init the crn for the arrival events
        seed = gsl_rng_uniform_int(seeds_rng.get(), gsl_rng_max(seeds_rng.get()));
        arrival_rng_index = dsample::CRN::getInstance().init(seed);
        dsample::CRN::getInstance().log(seed, "arrival events");
        seed = gsl_rng_uniform_int(seeds_rng.get(), gsl_rng_max(seeds_rng.get()));
        uniform_rng_index = dsample::CRN::getInstance().init(seed);
        dsample::CRN::getInstance().log(seed, "uniform");
        seed = gsl_rng_uniform_int(seeds_rng.get(), gsl_rng_max(seeds_rng.get()));
        num_edges_rng_index = dsample::CRN::getInstance().init(seed);
        dsample::CRN::getInstance().log(seed, "number of edges");
    }

    r1 = dsample::CRN::getInstance().get(num_edges_rng_index);
    r2 = dsample::CRN::getInstance().get(uniform_rng_index);
    r3 = dsample::CRN::getInstance().get(arrival_rng_index);

    // use the WEvonet class
    std::cout << "Generating Graph..." << std::endl;

    dnet::tGraphSP g;

    if (net_gen == 1) {
        g = dnet::WEvonet::createBBVGraph(net_size, max_edges, edge_fixed, max_arrival, r1, r2, r3);
    } else if (net_gen == 2) {
        g = dnet::WEvonet::createERGraph(net_size, edge_fixed, max_arrival, r1, edge_prob);
    }

    if (format == dnet::GraphUtil::GRAPHVIZ) {
        dnet::GraphUtil::print(g, filename, dnet::GraphUtil::GRAPHVIZ);
    } else {
        dnet::GraphUtil::print(g, filename, dnet::GraphUtil::GRAPHML);
    }

    return EXIT_SUCCESS;
}
