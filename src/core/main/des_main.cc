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
#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <fstream>
#include <iostream>
#include <string>

#include <boost/cstdint.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/graph/graphml.hpp>
#include <boost/graph/graphviz.hpp>

#include <gsl/gsl_randist.h>

#include "CL.hh"
namespace dcore = des::core;

#include "CRN.hh"
#include "Seeds.hh"
namespace dsample = des::sampling;

#include "WEvonet.hh"
namespace dnet = des::network;



int main(int argc, char *argv[])
{
    dcore::tDesArgsSP desArgs(new dcore::desArgs_t);
    dcore::CL cl;
    boost::uint32_t cur_arrival;


    if (cl.parse(argc, argv, desArgs)) {
        return EXIT_SUCCESS;
    }

    dnet::Graph graph;

    if (desArgs->graph_filename != "") {
        // read the graph
        std::ifstream in((desArgs->graph_filename).c_str(), std::ifstream::in);

        if (in.is_open()) {
            boost::dynamic_properties dp;
            dp.property("id", get(boost::vertex_index, graph));
            dp.property("weight", get(boost::edge_weight, graph));
            dp.property("service_rate", get(vertex_service_rate, graph));
            dp.property("arrival_rate", get(vertex_arrival_rate, graph));

            boost::read_graphml(in, graph, dp);

            in.close();
        } else {
            std::cerr << "Error: Cannot open graph file " << desArgs->graph_filename
                      << "!" << std::endl;
        }
    } else {
        // generate the graph
        std::cout << "Generate the graph " << std::endl;
    }

    // initialise the random number seeds
    dsample::Seeds seeds = dsample::SeedsSingleton::getInstance();

    if (desArgs->seeds_filename != "") {
        // read the seeds
        seeds.init(desArgs->seeds_filename.c_str());
    } else {
        // generate the graph
        std::cout << "Use random number to generate seeds." << std::endl;
    }

    // initialise the common random number container
    dsample::CRN crn = dsample::CRNSingleton::getInstance();
    boost::uint32_t seed = seeds.getSeed();
    int index = crn.init(seed);
    dsample::tGslRngSP rng = crn.get(index - 1);
    crn.log(seed, "arrival events");

    cur_arrival = 0;

    // for as long as there is no stopping event
    while (cur_arrival < desArgs->stop_time) {
        // generate arrival events
        cur_arrival += gsl_ran_poisson(rng.get(), 4);

        std::cout << cur_arrival << std::endl;
    }

    // generate events over this graph
    // process events

    return EXIT_SUCCESS;
}
