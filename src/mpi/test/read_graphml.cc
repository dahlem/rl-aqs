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
#include <fstream>
#include <string>

#ifdef HAVE_MPI
# include <mpi.h>
#endif /* HAVE_MPI */

#include <boost/graph/adjacency_list.hpp>

#include "DirectedGraph.hh"
#include "GraphUtil.hh"
namespace dnet = des::network;


int main(int argc, char *argv[])
{
#ifdef HAVE_MPI
    int rank, num_tasks;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);

    std::string filename = "/home/tecsc/ddahlem/des-1.8/graphs/test/10_3.gml";
    std::ifstream in(filename.c_str(), std::ifstream::in);

    dnet::tGraphSP graph(new dnet::Graph);
    boost::dynamic_properties dp;

    dp.property(dnet::EDGE_WEIGHT, get(boost::edge_weight, *graph));
    dp.property(dnet::VERTEX_ID, get(boost::vertex_index, *graph));
    dp.property(dnet::SERVICE_RATE, get(vertex_service_rate, *graph));
    dp.property(dnet::ARRIVAL_RATE, get(vertex_arrival_rate, *graph));
    dp.property(dnet::BUSY, get(vertex_busy, *graph));
    dp.property(dnet::TIME_SERVICE_ENDS, get(vertex_time_service_ends, *graph));
    dp.property(dnet::NUMBER_IN_QUEUE, get(vertex_number_in_queue, *graph));
    dp.property(dnet::AVERAGE_DELAY_IN_QUEUE, get(vertex_average_delay_in_queue, *graph));
    dp.property(dnet::NUM_EVENTS, get(vertex_num_events, *graph));
    dp.property(dnet::UTILISATION, get(vertex_utilisation, *graph));
    dp.property(dnet::BDT, get(vertex_Bdt, *graph));
    dp.property(dnet::QDT, get(vertex_Qdt, *graph));
    dp.property(dnet::LAST_EVENT_TIME, get(vertex_last_event_time, *graph));
    dp.property(dnet::EXPECTED_AVERAGE_NUMBER_EVENT, get(vertex_expected_average_number_event, *graph));

    std::cout << "Rank: " << rank << ", read graphml file." << std::endl;
    std::cout.flush();
    boost::read_graphml(in, *graph, dp);
    std::cout << "Rank: " << rank << ", vertices: " << boost::num_vertices(*graph) << std::endl;
    std::cout.flush();

    MPI_Finalize();
#endif /* HAVE_MPI */

    return EXIT_SUCCESS;
}
