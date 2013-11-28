// Copyright (C) 2008, 2009, 2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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

/** @file SimulationLHS.hh
 * This header declares the simulation method, so that it can be considered a
 * black-box simulation by the main method.
 *
 * @author Dominik Dahlem
 */
#ifndef __DES_CORE_SIMULATIONLHS_HH__
#define __DES_CORE_SIMULATIONLHS_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#if NDEBUG
# include <iostream>
#endif /* NDEBUG */
#include <sstream>
#include <string>

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

#include "Results.hh"
namespace dio = des::io;

#include "CRN.hh"
#include "Seeds.hh"
#include "LHS.hh"
namespace dsample = des::sampling;

#include "common.hh"
#include "CL.hh"
#include "LhsUtils.hh"



namespace des
{
namespace core
{


/** @class SimulationLHS
 * Class encapsulating the logic to start an Latin-Hypercube Design of experiments.
 */
template <class DecoratedSim>
class SimulationLHS
{
public:
    SimulationLHS(DecoratedSim &p_dsim)
        : m_dsim(p_dsim)
        {}

    ~SimulationLHS()
        {}

    /** @fn static sim_output simulate()
     * Conduct the simulation to be within a given confidence interval.
     *
     * @return the simulation output variables
     */
    sim_output simulate(tDesArgsSP p_desArgs)
        {
            sim_output output;

            // 1. perform lhs
            gsl_vector *min, *max;
            gsl_matrix *sample;

            // init the crn for the arrival events
            boost::uint32_t seed = dsample::Seeds::getInstance().getSeed();
            boost::int32_t rng_index = dsample::CRN::getInstance().init(seed);
            dsample::CRN::getInstance().log(seed, "LHS permutation");
            dsample::tGslRngSP rng
                = dsample::CRN::getInstance().get(rng_index);

            int dimensions = LhsUtils::dimensions(p_desArgs);

            // 2. print header into log-file
            std::stringstream outDir, csv_line;
            outDir << p_desArgs->results_dir << "/";

            std::string dir = outDir.str();
            std::string file = "simulations.dat";

            dio::Results sim_output(file, dir);

#ifndef NDEBUG
            std::cout << "The number of LHS dimensions is " << dimensions << std::endl;
            std::cout << "Network Size Index: " << LhsUtils::getNetSizeIndex(p_desArgs) << std::endl;
            std::cout << "Max. Edges Index: " << LhsUtils::getMaxEdgesIndex(p_desArgs) << std::endl;
            std::cout << "Edge Prob. Index: " << LhsUtils::getEdgeProbIndex(p_desArgs) << std::endl;
            std::cout << "Arrival Boost Index: " << LhsUtils::getVertexBoostIndex(p_desArgs) << std::endl;
            std::cout << "Edge Boost Index: " << LhsUtils::getEdgeBoostIndex(p_desArgs) << std::endl;
            std::cout << "RL Q alpha Index: " << LhsUtils::getRLAlphaIndex(p_desArgs) << std::endl;
            std::cout << "RL Q Lambda Index: " << LhsUtils::getRLLambdaIndex(p_desArgs) << std::endl;
            std::cout << "RL epsilon Index: " << LhsUtils::getRLEpsilonIndex(p_desArgs) << std::endl;
            std::cout << "NN momentum Index: " << LhsUtils::getNNMomentumIndex(p_desArgs) << std::endl;
            std::cout << "RL policy wpl Index: " << LhsUtils::getRlWplEtaIndex(p_desArgs) << std::endl;
            std::cout << "CPL A(+) Index: " << LhsUtils::getCplAPosIndex(p_desArgs) << std::endl;
            std::cout << "CPL A(-) Index: " << LhsUtils::getCplANegIndex(p_desArgs) << std::endl;
            std::cout << "CPL r(+) Index: " << LhsUtils::getCplRPosIndex(p_desArgs) << std::endl;
            std::cout << "CPL r(+) Index: " << LhsUtils::getCplRNegIndex(p_desArgs) << std::endl;
#endif /* NDEBUG */

            if (dimensions > 0) {
                min = gsl_vector_calloc(dimensions);
                max = gsl_vector_calloc(dimensions);

                if (LhsUtils::getNetSizeIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getNetSizeIndex(p_desArgs), p_desArgs->min_size);
                    gsl_vector_set(max, LhsUtils::getNetSizeIndex(p_desArgs), p_desArgs->max_size);
                }
                if (LhsUtils::getMaxEdgesIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getMaxEdgesIndex(p_desArgs), p_desArgs->min_max_edges);
                    gsl_vector_set(max, LhsUtils::getMaxEdgesIndex(p_desArgs), p_desArgs->max_max_edges);
                }
                if (LhsUtils::getEdgeProbIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getEdgeProbIndex(p_desArgs), p_desArgs->min_edge_prob);
                    gsl_vector_set(max, LhsUtils::getEdgeProbIndex(p_desArgs), p_desArgs->max_edge_prob);
                }
                if (LhsUtils::getVertexBoostIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getVertexBoostIndex(p_desArgs), p_desArgs->min_boost_arrival);
                    gsl_vector_set(max, LhsUtils::getVertexBoostIndex(p_desArgs), p_desArgs->max_boost_arrival);
                }
                if (LhsUtils::getEdgeBoostIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getEdgeBoostIndex(p_desArgs), p_desArgs->min_boost_edge);
                    gsl_vector_set(max, LhsUtils::getEdgeBoostIndex(p_desArgs), p_desArgs->max_boost_edge);
                }
                if (LhsUtils::getRLAlphaIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getRLAlphaIndex(p_desArgs), p_desArgs->min_rl_q_alpha);
                    gsl_vector_set(max, LhsUtils::getRLAlphaIndex(p_desArgs), p_desArgs->max_rl_q_alpha);
                }
                if (LhsUtils::getRLLambdaIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getRLLambdaIndex(p_desArgs), p_desArgs->min_rl_q_lambda);
                    gsl_vector_set(max, LhsUtils::getRLLambdaIndex(p_desArgs), p_desArgs->max_rl_q_lambda);
                }
                if (LhsUtils::getRLEpsilonIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getRLEpsilonIndex(p_desArgs), p_desArgs->min_rl_policy_epsilon);
                    gsl_vector_set(max, LhsUtils::getRLEpsilonIndex(p_desArgs), p_desArgs->max_rl_policy_epsilon);
                }
                if (LhsUtils::getNNMomentumIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getNNMomentumIndex(p_desArgs), p_desArgs->min_nn_momentum);
                    gsl_vector_set(max, LhsUtils::getNNMomentumIndex(p_desArgs), p_desArgs->max_nn_momentum);
                }
                if (LhsUtils::getRlWplEtaIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getRlWplEtaIndex(p_desArgs), p_desArgs->min_rl_policy_wpl_eta);
                    gsl_vector_set(max, LhsUtils::getRlWplEtaIndex(p_desArgs), p_desArgs->max_rl_policy_wpl_eta);
                }
                if (LhsUtils::getCplAPosIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getCplAPosIndex(p_desArgs), p_desArgs->min_cognitive_A_pos);
                    gsl_vector_set(max, LhsUtils::getCplAPosIndex(p_desArgs), p_desArgs->max_cognitive_A_pos);
                }
                if (LhsUtils::getCplANegIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getCplANegIndex(p_desArgs), p_desArgs->min_cognitive_A_neg);
                    gsl_vector_set(max, LhsUtils::getCplANegIndex(p_desArgs), p_desArgs->max_cognitive_A_neg);
                }
                if (LhsUtils::getCplRPosIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getCplRPosIndex(p_desArgs), p_desArgs->min_cognitive_r_pos);
                    gsl_vector_set(max, LhsUtils::getCplRPosIndex(p_desArgs), p_desArgs->max_cognitive_r_pos);
                }
                if (LhsUtils::getCplRNegIndex(p_desArgs) >= 0) {
                    gsl_vector_set(min, LhsUtils::getCplRNegIndex(p_desArgs), p_desArgs->min_cognitive_r_neg);
                    gsl_vector_set(max, LhsUtils::getCplRNegIndex(p_desArgs), p_desArgs->max_cognitive_r_neg);
                }

                if (p_desArgs->lhs_optimal) {
#ifndef NDEBUG
                    std::cout << "Perform optimal LHS sampling..." << std::endl;
#endif /* NDEBUG */
                    dsample::LHS::sample(rng.get(), min, max, p_desArgs->simulations, &sample, p_desArgs->lhs_r);
                } else {
#ifndef NDEBUG
                    std::cout << "Perform LHS sampling..." << std::endl;
#endif /* NDEBUG */
                    dsample::LHS::sample(rng.get(), min, max, p_desArgs->simulations, &sample);
                }

                // serialise design matrix
                std::string designFile = "design.dat";
                LhsUtils::serialiseDesign(p_desArgs, sample, designFile, dir);
            }

            if (p_desArgs->add_sim.empty()) {
                csv_line << "sim_num," << ARGS_HEADER << ",actual_reps,meanDelay,varDelay,meanAvgNumEvents,varAvgNumEvents,meanAvgEventInSystem,varAvgEventInSystem";
                sim_output.print(csv_line);
            }

#ifndef NDEBUG
            std::cout << "Perform " << sample->size1 << " simulations..." << std::endl;
#endif /* NDEBUG */

            // 3. run experiment
            for (boost::uint16_t i = 0; i < sample->size1; ++i) {
                p_desArgs->sim_num = i + 1;

                // set the i-th experiment conditions
                if (LhsUtils::getNetSizeIndex(p_desArgs) >= 0) {
                    p_desArgs->net_size = static_cast<boost::uint16_t> (
                        round(gsl_matrix_get(
                                  sample, i, LhsUtils::getNetSizeIndex(p_desArgs))));
                }
                if (LhsUtils::getMaxEdgesIndex(p_desArgs) >= 0) {
                    p_desArgs->max_edges = static_cast<boost::uint16_t> (
                        round(gsl_matrix_get(
                                  sample, i, LhsUtils::getMaxEdgesIndex(p_desArgs))));
                }
                if (LhsUtils::getEdgeProbIndex(p_desArgs) >= 0) {
                    p_desArgs->edge_prob = gsl_matrix_get(
                        sample, i, LhsUtils::getEdgeProbIndex(p_desArgs));
                }
                if (LhsUtils::getVertexBoostIndex(p_desArgs) >= 0) {
                    p_desArgs->boost_arrival = gsl_matrix_get(
                        sample, i, LhsUtils::getVertexBoostIndex(p_desArgs));
                }
                if (LhsUtils::getEdgeBoostIndex(p_desArgs) >= 0) {
                    p_desArgs->boost_edge = gsl_matrix_get(
                        sample, i, LhsUtils::getEdgeBoostIndex(p_desArgs));
                }
                if (LhsUtils::getRLAlphaIndex(p_desArgs) >= 0) {
                    p_desArgs->rl_q_alpha = gsl_matrix_get(
                        sample, i, LhsUtils::getRLAlphaIndex(p_desArgs));
                }
                if (LhsUtils::getRLLambdaIndex(p_desArgs) >= 0) {
                    p_desArgs->rl_q_lambda = gsl_matrix_get(
                        sample, i, LhsUtils::getRLLambdaIndex(p_desArgs));
                }
                if (LhsUtils::getRLEpsilonIndex(p_desArgs) >= 0) {
                    p_desArgs->rl_policy_epsilon = gsl_matrix_get(
                        sample, i, LhsUtils::getRLEpsilonIndex(p_desArgs));
                }
                if (LhsUtils::getNNMomentumIndex(p_desArgs) >= 0) {
                    p_desArgs->nn_momentum = gsl_matrix_get(
                        sample, i, LhsUtils::getNNMomentumIndex(p_desArgs));
                }
                if (LhsUtils::getRlWplEtaIndex(p_desArgs) >= 0) {
                    p_desArgs->rl_policy_wpl_eta = gsl_matrix_get(
                        sample, i, LhsUtils::getRlWplEtaIndex(p_desArgs));
                }
                if (LhsUtils::getCplAPosIndex(p_desArgs) >= 0) {
                    p_desArgs->cognitive_A_pos = gsl_matrix_get(
                        sample, i, LhsUtils::getCplAPosIndex(p_desArgs));
                }
                if (LhsUtils::getCplANegIndex(p_desArgs) >= 0) {
                    p_desArgs->cognitive_A_neg = gsl_matrix_get(
                        sample, i, LhsUtils::getCplANegIndex(p_desArgs));
                }
                if (LhsUtils::getCplRPosIndex(p_desArgs) >= 0) {
                    p_desArgs->cognitive_r_pos = gsl_matrix_get(
                        sample, i, LhsUtils::getCplRPosIndex(p_desArgs));
                }
                if (LhsUtils::getCplRNegIndex(p_desArgs) >= 0) {
                    p_desArgs->cognitive_r_neg = gsl_matrix_get(
                        sample, i, LhsUtils::getCplRNegIndex(p_desArgs));
                }

                output = m_dsim.simulate(p_desArgs);

                csv_line.str("");
                csv_line << p_desArgs->sim_num << ","
                         << const_cast <const desArgs_t&> (*p_desArgs) << ","
                         << output.replications << ","
                         << output.mean_system_average_delay << ","
                         << output.sd_system_average_delay << ","
                         << output.mean_system_expected_average_num_in_queue << ","
                         << output.sd_system_expected_average_num_in_queue << ","
                         << output.mean_system_total_q << ","
                         << output.sd_system_total_q;

                sim_output.print(csv_line);
            }

            // 4. free gsl stuff
            gsl_vector_free(min);
            gsl_vector_free(max);
            gsl_matrix_free(sample);

            return output;
        }


private:

    SimulationLHS(const SimulationLHS&)
        {}

    void operator=(const SimulationLHS&)
        {}

    DecoratedSim &m_dsim;
};


}
}



#endif /* __DES_CORE_SIMULATIONLHS_HH__ */
