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

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <iostream>
#include <cstring>

#ifdef HAVE_MPI
# include <mpi.h>
#endif /* HAVE_MPI */

#include <boost/cstdint.hpp>

typedef struct {
    unsigned int replications;   /* initial replications required */
    unsigned int simulations;    /* initial number of simulations */
    unsigned int sim_num;        /* simulation number */
    unsigned int rep_num;        /* replication number */

    double stop_time;               /* stopping time of the DES */
    double min_stop_time;           /* min. stopping time of the DES */
    double max_stop_time;           /* max. stopping time of the DES */
    double alpha;                   /* 100(1 - alpha) confidence interval for the experiments */
    double error;                   /* error threshold for the ci calculations */

    int vertex;          /* the vertex to trace */
    int graph_rate;      /* the rate to generate graphs at */
    int generations;     /* number of generations for the event simulation */

    unsigned short trace_event;               /* trace an event */
    unsigned short log_events;                /* log the events */
    unsigned short confidence;                /* run experiments within a confidence band */
    unsigned short lhs;                       /* run experiments with lhs sampling */

    char graph_filename[256];     /* filename for the graph */
    char seeds_filename[256];     /* filename for the seeds */
    char results_dir[256];        /* directory name for the results */
    char events_unprocessed[256]; /* filename for the unprocessed events */
    char events_processed[256];   /* filename for the processed events */
} s;

int main(int argc, char *argv[])
{
    s value;
    int          rank;
    MPI_Datatype mystruct;
    int          blocklens[5];
    MPI_Aint     indices[5];
    MPI_Datatype old_types[5];

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* One value of each type */
    blocklens[0] = 4;
    blocklens[1] = 5;
    blocklens[2] = 3;
    blocklens[3] = 4;
    blocklens[4] = 1280;

    /* The base types */
    old_types[0] = MPI_UNSIGNED;
    old_types[1] = MPI_DOUBLE;
    old_types[2] = MPI_INT;
    old_types[3] = MPI_SHORT;
    old_types[4] = MPI_CHAR;

    /* The locations of each element */
    MPI_Address( &value.replications, &indices[0] );
    MPI_Address( &value.stop_time, &indices[1] );
    MPI_Address( &value.vertex, &indices[2] );
    MPI_Address( &value.trace_event, &indices[3] );
    MPI_Address( &value.graph_filename, &indices[4] );

    /* Make relative */
    indices[4] = indices[4] - indices[0];
    indices[3] = indices[3] - indices[0];
    indices[2] = indices[2] - indices[0];
    indices[1] = indices[1] - indices[0];
    indices[0] = 0;

    MPI_Type_struct( 5, blocklens, indices, old_types, &mystruct );
    MPI_Type_commit( &mystruct );

    if (rank == 0) {
        value.replications = 1;
        value.simulations = 2;
        value.sim_num = 3;
        value.rep_num = 4;

        value.stop_time = 5.0;
        value.min_stop_time = 0;
        value.max_stop_time = 6.0;
        value.alpha = 0.05;
        value.error = 0.1;

        value.vertex = 7;
        value.graph_rate = 8;
        value.generations = 9;

        value.trace_event = 1;
        value.log_events = 0;
        value.confidence = 1;
        value.lhs = 0;

        strncpy(value.graph_filename, "grpah", 256);
        strncpy(value.seeds_filename, "seeds", 256);
        strncpy(value.results_dir, "results", 256);
        strncpy(value.events_unprocessed, "unprocessed", 256);
        strncpy(value.events_processed, "processed", 256);
    }

    MPI_Bcast( &value, 1, mystruct, 0, MPI_COMM_WORLD );
    std::cout << "Process " << rank << " got "
              << value.replications << ", " << value.simulations
              << ", " << value.sim_num << ", " << value.rep_num
              << ", " << value.stop_time << ", " << value.min_stop_time
              << ", " << value.max_stop_time << ", " << value.alpha
              << ", " << value.error << ", " << value.vertex
              << ", " << value.graph_rate << ", " << value.generations
              << ", " << value.trace_event << ", " << value.log_events
              << ", " << value.confidence << ", " << value.lhs
              << ", " << value.graph_filename << ", " << value.seeds_filename
              << ", " << value.results_dir << ", " << value.events_unprocessed
              << ", " << value.events_processed << std::endl;

    /* Clean up the type */
    MPI_Type_free(&mystruct);
    MPI_Finalize();

    return EXIT_SUCCESS;
}
