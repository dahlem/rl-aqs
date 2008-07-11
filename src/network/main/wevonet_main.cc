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
#include <iostream>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>
namespace po = boost::program_options;

#include "WEvonet.hh"
using des::network::WEvonet;



int main(int argc, char *argv[])
{
    int net_size;
    int max_edges;
    const gsl_rng_type * T;
    shared_ptr <gsl_rng> r1, r2, r3;

    // Declare the supported options.
    po::options_description desc("Configuration");
    desc.add_options()
        ("help", "produce help message")
        ("size", po::value<int>(), "set the size of the network")
        ("max_edges", po::value<int>(), "set the maximum number of edges to connect a new vertex")
        ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return EXIT_SUCCESS;
    }

    if (vm.count("size")) {
        std::cout << "Size of network set to "
                  << vm["size"].as<int>() << "." << std::endl;
        net_size = vm["size"].as<int>();
    } else {
        std::cout << "Default network size is 10." << std::endl;
        net_size = 10;
    }

    if (vm.count("max_edges")) {
        std::cout << "Maximum number of edges is set to "
                  << vm["max_edges"].as<int>() << "." << std::endl;
        max_edges = vm["max_edges"].as<int>();
    } else {
        std::cout << "Default maximum number of edges is 3." << std::endl;
        max_edges = 3;
    }

    gsl_rng_env_setup();

    T = gsl_rng_default;
    r1 = shared_ptr <gsl_rng>(gsl_rng_alloc(T), gsl_rng_free);
    r2 = shared_ptr <gsl_rng>(gsl_rng_alloc(T), gsl_rng_free);
    r3 = shared_ptr <gsl_rng>(gsl_rng_alloc(T), gsl_rng_free);

    // use the WEvonet class
    std::cout << "Generating Graph..." << std::endl;

    WEvonet net(net_size, max_edges, r1, r2, r3);
    net.print("test.dot");

    return EXIT_SUCCESS;
}
