// Copyright (C) 2008, 2009, 2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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
typedef struct SimOutput
{
    // single simulation output
    double system_average_delay;
    double system_expected_average_num_in_queue;
    double system_total_q;

    // average simulation output
    double mean_system_average_delay;
    double mean_system_expected_average_num_in_queue;
    double mean_system_total_q;
    double sd_system_average_delay;
    double sd_system_expected_average_num_in_queue;
    double sd_system_total_q;

    // number of replications
    unsigned int replications;
    unsigned int simulation_id;

    SimOutput()
        : system_average_delay(0.0), system_expected_average_num_in_queue(0.0),
          system_total_q(0.0), mean_system_average_delay(0.0),
          mean_system_expected_average_num_in_queue(0.0), mean_system_total_q(0.0),
          sd_system_average_delay(0.0), sd_system_expected_average_num_in_queue(0.0),
          sd_system_total_q(0.0), replications(0), simulation_id(0)
        {}

} sim_output;



#ifdef HAVE_MPI

typedef struct SimArgsMPI {
    unsigned int sim_num;       /* simulation number */
    unsigned int rep_num;       /* replication number */
    unsigned int net_size;      /* network size */
    unsigned int max_edges;     /* max edges */
    double edge_prob;           /* probability of edge (u,v) */
    double boost_arrival;       /* boost arrival rate */
    double boost_edge;          /* boost edge weight */
    double rl_q_alpha;          /* q-learning rate */
    double rl_q_lambda;         /* action value discount rate */
    double rl_policy_epsilon;   /* rl policy epsilon-greedy value */
    double rl_policy_boltzmann_t;   /* rl policy boltzmann temperature */
    double nn_momentum;             /* NN Backpropagation momentum */
    double rl_policy_wpl_eta;             /* NN Backpropagation momentum */
    double cognitive_A_pos;             /* cognitive A(+) */
    double cognitive_A_neg;             /* cognitive A(-) */
    double cognitive_r_pos;             /* cognitive r(+) */
    double cognitive_r_neg;             /* cognitive r(-) */

    SimArgsMPI()
        : sim_num(0), rep_num(0), net_size(0), max_edges(0), edge_prob(0.0),
          boost_arrival(0.0), boost_edge(0.0), rl_q_alpha(0.0), rl_q_lambda(0.0),
          rl_policy_epsilon(0.0), rl_policy_boltzmann_t(0.0), nn_momentum(0.0),
          rl_policy_wpl_eta(0.0), cognitive_A_pos(0.0), cognitive_A_neg(0.0),
          cognitive_r_pos(0.0), cognitive_r_neg(0.0)
        {}

} tSimArgsMPI;


#endif /* HAVE_MPI */

}
}



#endif /* __DES_CORE_COMMON_HH__ */
