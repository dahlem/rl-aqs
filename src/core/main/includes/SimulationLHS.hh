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

#include <sstream>
#include <string>

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

#include "common.hh"
#include "CL.hh"

#include "Results.hh"
namespace dio = des::io;

#include "CRN.hh"
#include "Seeds.hh"
#include "LHS.hh"
namespace dsample = des::sampling;



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
    SimulationLHS(DecoratedSim p_dsim)
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

            min = gsl_vector_calloc(1);
            max = gsl_vector_calloc(1);
            gsl_vector_set(min, 0, p_desArgs->min_stop_time);
            gsl_vector_set(max, 0, p_desArgs->max_stop_time);

            dsample::LHS::sample(rng.get(), min, max, p_desArgs->simulations, &sample);

            // 2. print header into log-file
            std::stringstream outDir, csv_line;
            outDir << p_desArgs->results_dir << "/";

            std::string dir = outDir.str();
            std::string file = "simulations.dat";

            dio::tResultsSP sim_output(
                new dio::Results(file, dir));

            csv_line << "sim_num," << ARGS_HEADER << ",actual_reps";
            sim_output->print(csv_line);

            // 3. run experiment
            for (boost::uint16_t i = 0; i < p_desArgs->simulations; ++i) {
                p_desArgs->sim_num = i + 1;

                // set the i-th experiment conditions
                p_desArgs->stop_time = gsl_matrix_get(sample, i, 0);

                output = m_dsim->simulate(p_desArgs);

                csv_line.str("");
                csv_line << p_desArgs->sim_num << "," << const_cast <const desArgs_t&> (*p_desArgs)
                         << "," << output.replications;
                sim_output->print(csv_line);
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


    DecoratedSim m_dsim;
};


}
}



#endif /* __DES_CORE_SIMULATIONLHS_HH__ */
