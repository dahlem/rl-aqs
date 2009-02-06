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

/** @file CL.hh
 * Declaration of the methods for the command-line parsing of the main
 * routine for DES.
 *
 * @author Dominik Dahlem
 */
#ifndef __DES_CORE_CL_HH__
#define __DES_CORE_CL_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <string>

#include <boost/cstdint.hpp>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/program_options/options_description.hpp>
namespace po = boost::program_options;


namespace des { namespace core {


/**
 * const variables specifying the allowed options.
 */
const std::string STOPTIME = "stop_time";
const std::string GENERATIONS = "generations";
const std::string MAX_ARRIVAL = "max_arrival_rate";

const std::string GRAPH = "graph";
const std::string SEEDS = "seeds";
const std::string HELP = "help";
const std::string RESULTS = "results";
const std::string TRACE = "trace";
const std::string VERTEX = "vertex";
const std::string VERS = "version";
const std::string LOG_GRAPH_RATE = "graph_generation";
const std::string LOG_EVENTS = "log_events";
const std::string ADD_SIM = "add_sim";

const std::string WITH_CI = "confidence";
const std::string WITH_LHS = "lhs";
const std::string REPLICATIONS = "replications";
const std::string SIMULATIONS = "simulations";
const std::string ALPHA = "alpha";
const std::string ERROR = "error";

const std::string SIZE = "size";
const std::string MINSIZE = "min_size";
const std::string MAXSIZE = "max_size";
const std::string MAX_EDGES = "max_edges";
const std::string MIN_MAX_EDGES = "min_max_edges";
const std::string MAX_MAX_EDGES = "max_max_edges";
const std::string WEIGHT_FIXED = "weight";
const std::string EDGE_PROB = "edge_prob";
const std::string MIN_EDGE_PROB = "min_edge_prob";
const std::string MAX_EDGE_PROB = "max_edge_prob";
const std::string BOOST_ARRIVAL = "boost_arrival";
const std::string BOOST_EDGE = "boost_edge";
const std::string MIN_BOOST_ARRIVAL = "min_boost_arrival";
const std::string MAX_BOOST_ARRIVAL = "max_boost_arrival";
const std::string MIN_BOOST_EDGE = "min_boost_edge";
const std::string MAX_BOOST_EDGE = "max_boost_edge";
const std::string GENERATOR = "graph_generator";

const std::string RL = "rl";
const std::string RL_Q_ALPHA = "rl_q_alpha";
const std::string RL_Q_LAMBDA = "rl_q_lambda";
const std::string RL_RESPONSE_ALPHA = "rl_response_alphas";
const std::string RL_RESPONSE_REWARD = "rl_response_rewards";
const std::string RL_POLICY = "rl_policy";
const std::string RL_POLICY_EPSILON = "rl_policy_epsilon";


/** @typedef tOptDescSP
 * Specifies shared pointer to the boost options description
 */
typedef boost::shared_ptr <po::options_description> tOptDescSP;

static const std::string ARGS_HEADER = "stop_time,generations,graphs,max_arrival,boost_arrival,boost_edge,confidence,alpha,error,initial_reps,network_type,network_size,max_edges,edgeProb,edgeDiffusion";

/** @struct
 * structure specifying the command line variables.
 */
struct desArgs_t {
    std::string graph_filename;     /* filename for the graph */
    std::string seeds_filename;     /* filename for the seeds */
    std::string results_dir;        /* directory name for the results */
    std::string events_unprocessed; /* filename for the unprocessed events */
    std::string events_processed;   /* filename for the processed events */
    std::string add_sim;            /* add to existing experiments */

    bool trace_event;               /* trace an event */
    bool log_events;                /* log the events */
    boost::int32_t vertex;          /* the vertex to trace */
    boost::int32_t graph_rate;      /* the rate to generate graphs at */
    double max_arrival;             /* the max. arrival rate */

    double stop_time;               /* stopping time of the DES */
    boost::int32_t generations;     /* number of generations for the event simulation */
    bool confidence;                /* run experiments within a confidence band */
    bool lhs;                       /* run experiments with lhs sampling */
    double alpha;                   /* 100(1 - alpha) confidence interval for the experiments */
    double error;                   /* error threshold for the ci calculations */
    boost::uint16_t replications;   /* initial replications required */
    boost::uint16_t simulations;    /* initial number of simulations */

    boost::uint16_t sim_num;        /* simulation number */
    boost::uint16_t rep_num;        /* replication number */

    boost::uint16_t net_size;       /* network size */
    boost::uint16_t min_size;       /* min. network size */
    boost::uint16_t max_size;       /* max. network size */
    boost::uint32_t max_edges;      /* max number of edges */
    boost::uint32_t min_max_edges;  /* min. maximum number of edges */
    boost::uint32_t max_max_edges;  /* max. maximum number of edges */
    double boost_arrival;           /* boost arrival rate */
    double min_boost_arrival;       /* min boost arrival rate */
    double max_boost_arrival;       /* max boost arrival rate */
    double boost_edge;              /* boost edge weight */
    double min_boost_edge;          /* min boost edge weight */
    double max_boost_edge;          /* max boost edge weight */

    boost::uint16_t net_gen;        /* network generator */
    double edge_fixed;              /* information diffusion coefficient */
    double edge_prob;               /* probability of vertices connecting */
    double min_edge_prob;           /* min. probability of vertices connecting */
    double max_edge_prob;           /* max. probability of vertices connecting */

    bool rl;                        /* switch to enable reinforcement learning */
    boost::shared_array<double> response_alpha;     /* the response levels for the reward model */
    boost::shared_array<double> response_reward;    /* the response rewards for the reward model */
    boost::uint16_t response_levels;                /* number of response levels */
    double rl_q_alpha;                              /* q-learning rate */
    double rl_q_lambda;                             /* action value discount rate */
    boost::uint16_t rl_policy;                      /* policy */
    double rl_policy_epsilon;                       /* epsilon-greedy */


    friend std::ostream& operator <<(std::ostream &p_os, const desArgs_t &desArgs)
        {
            p_os << desArgs.stop_time << ","
                 << desArgs.generations << ","
                 << (desArgs.graph_rate + 1) << ","
                 << desArgs.max_arrival << ","
                 << desArgs.boost_arrival << ","
                 << desArgs.boost_edge << ","
                 << desArgs.confidence << ","
                 << desArgs.alpha << ","
                 << desArgs.error << ","
                 << desArgs.replications << ","
                 << desArgs.net_gen << ","
                 << desArgs.net_size << ","
                 << desArgs.max_edges << ","
                 << desArgs.edge_prob << ","
                 << desArgs.edge_fixed << ","
                 << desArgs.rl_q_alpha << ","
                 << desArgs.rl_q_lambda << ","
                 << desArgs.rl_policy << ","
                 << desArgs.rl_policy_epsilon;

            return p_os;
        }
};

/** @typedef tDesArgsSP
 * Specifies shared pointer to the command line options
 */
typedef boost::shared_ptr <desArgs_t> tDesArgsSP;



/** @class CL
 * This class uses the boost program-options library to parse the command-line
 * parameters for the main routine of the discrete event simulator.
 */
class CL
{
public:
    CL();

    /** @fn parse(int argc, char *argv[], tDesArgsSP desArgs);
     * Parse the command-line parameters and store the relevant information
     * in a shared pointer of a structure.
     *
     * @param int number of command-line arguments
     * @param char** the command-line arguments
     * @param tDesArgsSP a shared pointer to the structure of the command-line
     *        arguments
     * @return either success or failure. In case of a failure then the help
     *        message was requested.
     */
    int parse(int argc, char *argv[], tDesArgsSP desArgs);

    /** @fn verify(tDesArgsSP)
     * Verify the command-line parameters
     *
     * @param tDesArgsSP a shared pointer to the structure of the command-line
     *        arguments.
     */
    void verify(tDesArgsSP desArgs);

private:

    /**
     * A shared pointer to the description of the command-line arguments.
     */
    tOptDescSP opt_desc;
};


    }
}


#endif
