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

/** @file common.hh
 * This header declares common structures for the discrete event simulator.
 *
 * @author Dominik Dahlem
 */
#ifndef __DES_CORE_COMMON_HH__
#define __DES_CORE_COMMON_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_MPI
# include <climits>
# include <mpi.h>
# define KILL_PILL USHRT_MAX
#endif /* HAVE_MPI */


namespace des
{
namespace core
{

/** @struct sim_output
 * Specifies the output variables of the simulation
 */
typedef struct
{
    // single simulation output
    double system_average_delay;
    double system_expected_average_num_in_queue;

    // average simulation output
    double mean_system_average_delay;
    double mean_system_expected_average_num_in_queue;
    double sd_system_average_delay;
    double sd_system_expected_average_num_in_queue;

    // number of replications
    unsigned int replications;
} sim_output;



#ifdef HAVE_MPI

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
} tDesArgsMPI;

#endif /* HAVE_MPI */

}
}



#endif /* __DES_CORE_COMMON_HH__ */
