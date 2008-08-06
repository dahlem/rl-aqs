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
#include <fstream>
#include <iostream>
#include <string>

#include <boost/graph/adjacency_list.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>
namespace po = boost::program_options;

#include <boost/graph/graphml.hpp>
#include <boost/graph/graphviz.hpp>

#include "WEvonet.hh"
using des::network::EdgeWeightMap;
using des::network::Graph;
using des::network::Vertex;
using des::network::VertexArrivalRateMap;
using des::network::VertexIndexMap;
using des::network::VertexServiceRateMap;



const std::string STOPTIME = "stop_time";
const std::string GRAPH = "graph";
const std::string HELP = "help";



int main(int argc, char *argv[])
{
    long stop_time;
    std::string filename;

    // Declare the supported options.
    // NOTE: [ddahlem] use a separte cc file for the command-line parsing logic.
    po::options_description desc("General Configuration");
    desc.add_options()
        (HELP.c_str(), "produce help message")
        (STOPTIME.c_str(), po::value<long>(), "set the stop time of the event simulator.")
        (GRAPH.c_str(), po::value<std::string>(), "set the graph for the event simulator.")
        ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count(HELP)) {
        std::cout << desc << std::endl;
        return EXIT_SUCCESS;
    }

    if (vm.count(STOPTIME.c_str())) {
        stop_time = vm[STOPTIME.c_str()].as<long>();
        std::cout << "Stopping time set to " << stop_time << "." << std::endl;
    } else {
        stop_time = 100;
        std::cout << "Default stopping time is " << stop_time << "." << std::endl;
    }

    if (vm.count(GRAPH.c_str())) {
        filename = vm[GRAPH.c_str()].as<std::string>();
        std::cout << "Read graph from filename " << filename << "." << std::endl;
    }

    // generate/read graph
    Graph graph;

    std::ifstream in(filename.c_str(), std::ifstream::in);

    if (in.is_open()) {
        boost::dynamic_properties dp;
        dp.property("id", get(boost::vertex_index, graph));
        dp.property("weight", get(boost::edge_weight, graph));
        dp.property("service_rate", get(vertex_service_rate, graph));
        dp.property("arrival_rate", get(vertex_arrival_rate, graph));

        boost::read_graphml(in, graph, dp);

        in.close();
    }

    Vertex z = vertex(0, graph);
    VertexArrivalRateMap vertex_arrival_props_map;
    VertexIndexMap vertex_index_props_map;
    VertexServiceRateMap vertex_service_props_map;

    std::cout << boost::num_vertices(graph) << std::endl;
    std::cout << vertex_service_props_map[z] << std::endl;
    std::cout << vertex_arrival_props_map[z] << std::endl;
    std::cout << vertex_index_props_map[z] << std::endl;

    // for as long as there is no stopping event
    // generate events over this graph
    // process events

    return EXIT_SUCCESS;
}
