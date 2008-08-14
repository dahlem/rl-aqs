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
#include <utility>

#include <boost/cstdint.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/graph/graphml.hpp>
#include <boost/graph/graphviz.hpp>

#include <gsl/gsl_randist.h>

#include "CL.hh"
#include "events.hh"
#include "EventGenerator.hh"
namespace dcore = des::core;

#include "CRN.hh"
#include "Seeds.hh"
namespace dsample = des::sampling;

#include "Entry.hh"
#include "LadderQueue.hh"
namespace dcommon = des::common;

#include "WEvonet.hh"
namespace dnet = des::network;




int main(int argc, char *argv[])
{
    dcore::tDesArgsSP desArgs(new dcore::desArgs_t);
    dcore::CL cl;
    boost::shared_ptr <dcommon::LadderQueue> queue;


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

    // initialise the random number seeds and the common random number container
    dsample::Seeds seeds = dsample::SeedsSingleton::getInstance();
    dsample::CRN crn = dsample::CRNSingleton::getInstance();
    boost::int32_t arrival_rng_index;
    boost::int32_t seeds_rng_index;
    boost::uint32_t seed = 0;

    if (desArgs->seeds_filename != "") {
        // read the seeds
        seeds.init(desArgs->seeds_filename.c_str());

        // init the crn for the arrival events
        seed = seeds.getSeed();
        arrival_rng_index = crn.init(seed);
        crn.log(seed, "arrival events");
    } else {
        // generate the seeds
        std::cout << "Use random number to generate seeds." << std::endl;

        // 1. init the random number generator for the seeds
        seeds_rng_index = crn.init(gsl_rng_default_seed);
        crn.log(gsl_rng_default_seed, "seeds");

        // 2. init the crn for the arrival events
        dsample::tGslRngSP seeds_rng = crn.get(seeds_rng_index - 1);
        seed = gsl_rng_uniform_int(seeds_rng.get(), gsl_rng_max(seeds_rng.get()));
        arrival_rng_index = crn.init(seed);
        crn.log(seed, "arrival events");
    }

    dsample::tGslRngSP arrival_rng = crn.get(arrival_rng_index - 1);

    dnet::VertexIndexMap vertex_index_props_map =
        get(boost::vertex_index, graph);
    dnet::VertexArrivalRateMap vertex_arrival_props_map =
        get(vertex_arrival_rate, graph);

    // generate events for each vertex in the graph
    typedef boost::graph_traits <dnet::Graph>::vertex_iterator vertex_iter_t;
    std::pair <vertex_iter_t, vertex_iter_t> p;

    queue = boost::shared_ptr<dcommon::LadderQueue>(new dcommon::LadderQueue);

    boost::int32_t destination;
    double arrival_rate;

    // generate events over this graph
    for (p = boost::vertices(graph); p.first != p.second; ++p.first) {
        destination = vertex_index_props_map[*p.first];
        arrival_rate = vertex_arrival_props_map[*p.first];

        dcore::EventGenerator::generate(
            queue, arrival_rng, destination, arrival_rate, desArgs->stop_time);
    }

    // process events
    dcommon::entry_t *entry;
    while ((entry = queue->dequeue()) != NULL) {
        std::cout << entry->arrival << " : " << entry->destination << " : " << entry->type << std::endl;
        delete entry;
    }

    return EXIT_SUCCESS;
}
