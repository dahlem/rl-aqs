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
#include <vector>

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
const std::string LOG_GRAPHS = "log_graphs";
const std::string LOG_GRAPH_RATE = "graph_generation";
const std::string LOG_EVENTS = "log_events";
const std::string ADD_SIM = "add_sim";
const std::string SIMULATION_DIR = "sim_dir";

const std::string WITH_CI = "confidence";
const std::string WITH_LHS = "lhs";
const std::string LHS_OPTIMAL = "lhs_optimal";
const std::string REPLICATIONS = "replications";
const std::string INIT_REPLICATIONS = "init_replications";
const std::string SIMULATIONS = "simulations";
const std::string ALPHA = "alpha";
const std::string ERROR = "error";
const std::string LHS_R = "lhs_r";

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
const std::string MIN_RL_Q_ALPHA = "min_rl_q_alpha";
const std::string MAX_RL_Q_ALPHA = "max_rl_q_alpha";
const std::string RL_Q_LAMBDA = "rl_q_lambda";
const std::string MIN_RL_Q_LAMBDA = "min_rl_q_lambda";
const std::string MAX_RL_Q_LAMBDA = "max_rl_q_lambda";
const std::string RL_RESPONSE_ALPHA = "rl_response_alphas";
const std::string RL_RESPONSE_REWARD = "rl_response_rewards";
const std::string RL_POLICY = "rl_policy";
const std::string RL_POLICY_EPSILON = "rl_policy_epsilon";
const std::string MIN_RL_POLICY_EPSILON = "min_rl_policy_epsilon";
const std::string MAX_RL_POLICY_EPSILON = "max_rl_policy_epsilon";
const std::string RL_POLICY_BOLTZMANN_T = "rl_policy_boltzmann_t";
const std::string RL_POLICY_WPL_ETA = "rl_policy_wpl_eta";

const std::string RL_HYBRID = "rl_hybrid";
const std::string RL_HYBRID_WARMUP = "rl_hybrid_warmup";

const std::string CL_EXPERT_NORMAL = "expert_normal";
const std::string CL_EXPERT_ABSOLUTE = "expert_absolute";
const std::string CL_EXPERT_POSITIVE = "expert_positive";
const std::string CL_EXPERT_NEGATIVE = "expert_negative";

const std::string CL_NN_HIDDENLAYER_NEURONS = "nn_hidden_neurons";
const std::string CL_NN_LOSS_POLICY = "nn_loss_policy";
const std::string CL_NN_WINDOW = "nn_window";
const std::string CL_NN_BRENT_ITER = "nn_brent_iter";
const std::string CL_NN_MOMENTUM = "nn_momentum";
const std::string CL_NN_CG = "nn_cg";
const std::string CL_NN_OUTSOURCE = "nn_outsource";
const std::string CL_RL_STATE_IDS = "rl_state_representation";
const std::string MIN_NN_MOMENTUM = "min_nn_momentum";
const std::string MAX_NN_MOMENTUM = "max_nn_momentum";


/** @typedef tOptDescSP
 * Specifies shared pointer to the boost options description
 */
typedef boost::shared_ptr <po::options_description> tOptDescSP;

static const std::string ARGS_HEADER = "stop_time,generations,graphs,max_arrival,boost_arrival,boost_edge,confidence,alpha,error,initial_reps,network_type,network_size,max_edges,edgeProb,edgeDiffusion,rl,rlq_alpha,rlq_lambda,rl_policy,rl_policy_epsilon,rl_policy_boltzmann_t,rl_hybrid,rl_hybrid_warmup,nn_momentum,rl_policy_wpl_eta";

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
    std::string sim_dir;            /* simulation results directory */

    bool trace_event;               /* trace an event */
    bool log_events;                /* log the events */
    bool log_graphs;                /* log the graphs */
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
    boost::uint16_t init_replications;   /* initial replications on cluster */
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
    bool rl_hybrid;                 /* switch to enable hybrid reinforcement learning */
    boost::uint16_t rl_hybrid_warmup;        /* rl hybrid warmup phase */
    double rl_q_alpha;                              /* q-learning rate */
    double min_rl_q_alpha;                          /* min q-learning rate */
    double max_rl_q_alpha;                          /* max q-learning rate */
    double rl_q_lambda;                             /* action value discount rate */
    double min_rl_q_lambda;                         /* min action value discount rate */
    double max_rl_q_lambda;                         /* max action value discount rate */
    boost::uint16_t rl_policy;                      /* policy */
    double rl_policy_epsilon;                       /* epsilon-greedy */
    double min_rl_policy_epsilon;                   /* min epsilon-greedy */
    double max_rl_policy_epsilon;                   /* max epsilon-greedy */
    double rl_policy_boltzmann_t;                   /* the temperature value for boltzmann */
    bool lhs_optimal;               /* lhs sampling with optimal design */
    boost::uint16_t lhs_r;          /* optimal design iterations */

    bool expert_normal;             /* expert metrics from "Expertness Based Cooperative Q-Learning" */
    bool expert_absolute;
    bool expert_positive;
    bool expert_negative;

    std::vector<int> rl_state_representation;
    boost::uint16_t nn_hidden_neurons;
    boost::uint16_t nn_loss_policy;
    boost::uint16_t nn_window;
    boost::uint16_t nn_brent_iter;
    double nn_momentum;
    bool nn_cg;
    bool nn_outsource;
    double min_nn_momentum;          /* min momentum */
    double max_nn_momentum;          /* max momentum */
    double rl_policy_wpl_eta;          /* wpl learning rate */


    desArgs_t(desArgs_t const &args)
        : graph_filename(args.graph_filename), seeds_filename(args.seeds_filename), results_dir(args.results_dir),
          events_unprocessed(args.events_unprocessed), events_processed(args.events_processed), add_sim(args.add_sim),
          sim_dir(args.sim_dir), trace_event(args.trace_event), log_events(args.log_events), log_graphs(args.log_graphs),
          vertex(args.vertex), graph_rate(args.graph_rate), max_arrival(args.max_arrival),
          stop_time(args.stop_time), generations(args.generations), confidence(args.confidence),
          lhs(args.lhs), alpha(args.alpha), error(args.error),
          replications(args.replications), init_replications(args.init_replications), simulations(args.simulations), sim_num(args.sim_num),
          rep_num(args.rep_num), net_size(args.net_size), max_size(args.max_size),
          max_edges(args.max_edges), min_max_edges(args.min_max_edges), max_max_edges(args.max_max_edges),
          boost_arrival(args.boost_arrival), min_boost_arrival(args.min_boost_arrival), max_boost_arrival(args.max_boost_arrival),
          boost_edge(args.boost_edge), min_boost_edge(args.min_boost_edge), max_boost_edge(args.max_boost_edge),
          net_gen(args.net_gen), edge_fixed(args.edge_fixed), edge_prob(args.edge_prob),
          min_edge_prob(args.min_edge_prob), max_edge_prob(args.max_edge_prob), rl(args.rl), rl_hybrid(args.rl_hybrid), rl_hybrid_warmup(args.rl_hybrid_warmup),
          rl_q_alpha(args.rl_q_alpha), min_rl_q_alpha(args.min_rl_q_alpha), max_rl_q_alpha(args.max_rl_q_alpha),
          rl_q_lambda(args.rl_q_lambda), min_rl_q_lambda(args.min_rl_q_lambda), max_rl_q_lambda(args.max_rl_q_lambda),
          rl_policy(args.rl_policy), rl_policy_epsilon(args.rl_policy_epsilon), min_rl_policy_epsilon(args.min_rl_policy_epsilon),
          max_rl_policy_epsilon(args.max_rl_policy_epsilon), rl_policy_boltzmann_t(args.rl_policy_boltzmann_t),
          lhs_optimal(args.lhs_optimal), lhs_r(args.lhs_r), expert_normal(args.expert_normal), expert_absolute(args.expert_absolute),
          expert_positive(args.expert_positive), expert_negative(args.expert_negative), rl_state_representation(args.rl_state_representation),
          nn_hidden_neurons(args.nn_hidden_neurons), nn_loss_policy(args.nn_loss_policy),
          nn_window(args.nn_window), nn_brent_iter(args.nn_brent_iter), nn_momentum(args.nn_momentum),
          nn_cg(args.nn_cg), nn_outsource(args.nn_outsource), min_nn_momentum(args.min_nn_momentum), max_nn_momentum(args.max_nn_momentum),
          rl_policy_wpl_eta(args.rl_policy_wpl_eta)
        {}

    desArgs_t()
        : graph_filename(""), seeds_filename(""), results_dir(""),
          events_unprocessed(""), events_processed(""), add_sim(""), sim_dir(""),
          trace_event(0), log_events(0), log_graphs(0),
          vertex(0), graph_rate(0), max_arrival(0.0),
          stop_time(0.0), generations(0), confidence(0),
          lhs(0), alpha(0.0), error(0.0),
          replications(0), init_replications(0), simulations(0), sim_num(0),
          rep_num(0), net_size(0), max_size(0),
          max_edges(0), min_max_edges(0), max_max_edges(0),
          boost_arrival(0.0), min_boost_arrival(0.0), max_boost_arrival(0.0),
          boost_edge(0.0), min_boost_edge(0.0), max_boost_edge(0.0),
          net_gen(0), edge_fixed(0.0), edge_prob(0.0),
          min_edge_prob(0.0), max_edge_prob(0.0), rl(0), rl_hybrid(false), rl_hybrid_warmup(1000),
          rl_q_alpha(0.0), min_rl_q_alpha(0.0), max_rl_q_alpha(0.0),
          rl_q_lambda(0.0), min_rl_q_lambda(0.0), max_rl_q_lambda(0.0),
          rl_policy(0), rl_policy_epsilon(0.0), min_rl_policy_epsilon(0.0),
          max_rl_policy_epsilon(0.0), rl_policy_boltzmann_t(0.0),
          lhs_optimal(false), lhs_r(10), expert_normal(false), expert_absolute(false),
          expert_positive(false), expert_negative(false), rl_state_representation(),
          nn_hidden_neurons(5), nn_loss_policy(1),
          nn_window(100), nn_brent_iter(500), nn_momentum(1.0),
          nn_cg(true), nn_outsource(false), min_nn_momentum(0.0), max_nn_momentum(0.0), rl_policy_wpl_eta(0.0)
        {}

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
                 << desArgs.rl << ","
                 << desArgs.rl_q_alpha << ","
                 << desArgs.rl_q_lambda << ","
                 << desArgs.rl_policy << ","
                 << desArgs.rl_policy_epsilon << ","
                 << desArgs.rl_policy_boltzmann_t << ","
                 << desArgs.rl_hybrid << ","
                 << desArgs.rl_hybrid_warmup << ","
                 << desArgs.nn_momentum << ","
                 << desArgs.rl_policy_wpl_eta;

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
