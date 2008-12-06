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

/** @file SimulationCI.hh
 * This header declares the simulation method, so that it can be considered a
 * black-box simulation by the main method.
 *
 * @author Dominik Dahlem
 */
#ifndef __DES_CORE_SIMULATIONCI_HH__
#define __DES_CORE_SIMULATIONCI_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <boost/shared_ptr.hpp>

#include "common.hh"
#include "CL.hh"

#include "CI.hh"
#include "OnlineStats.hh"
namespace dstats = des::statistics;



namespace des
{
namespace core
{


/** @class SimulationCI
 * Class encapsulating the logic to start the discrete event simulator.
 */
template <class DecoratedSim>
class SimulationCI
{
public:
    SimulationCI(DecoratedSim p_dsim, double p_alpha, double p_error,
                 boost::uint16_t p_initialExp)
        : m_dsim(p_dsim), m_alpha(p_alpha), m_error(p_error), m_initialExp(p_initialExp)
        {}

    ~SimulationCI()
        {}

    /** @fn static sim_output simulate()
     * Conduct the simulation to be within a given confidence interval.
     *
     * @return the simulation output variables
     */
    sim_output simulate(tDesArgsSP p_desArgs)
        {
            sim_output output, result;
            dstats::OnlineStats avgDelay;
            dstats::OnlineStats avgNumEvents;

            // start 2 experiments
            for (boost::uint16_t i = 0; i < m_initialExp; ++i) {
                output = m_dsim->simulate(p_desArgs);
                avgDelay.push(output.system_average_delay);
                avgNumEvents.push(output.system_expected_average_num_in_queue);
            }

            while (dstats::CI::isConfidentWithPrecision(
                         avgDelay.mean(),
                         avgDelay.variance(),
                         avgDelay.getNumValues(), m_alpha, m_error)
                     && dstats::CI::isConfidentWithPrecision(
                         avgNumEvents.mean(),
                         avgNumEvents.variance(),
                         avgNumEvents.getNumValues(), m_alpha, m_error))
            {
                output = m_dsim->simulate(p_desArgs);
                avgDelay.push(output.system_average_delay);
                avgNumEvents.push(output.system_expected_average_num_in_queue);
            }

            result.mean_system_average_delay = avgDelay.mean();
            result.sd_system_average_delay = avgDelay.standardDeviation();
            result.mean_system_expected_average_num_in_queue = avgNumEvents.mean();
            result.sd_system_expected_average_num_in_queue = avgNumEvents.standardDeviation();

            return result;
        }


private:

    SimulationCI(const SimulationCI&)
        {}

    void operator=(const SimulationCI&)
        {}


    DecoratedSim m_dsim;
    double m_alpha;
    double m_error;
    boost::uint16_t m_initialExp;
};


}
}



#endif /* __DES_CORE_SIMULATIONCI_HH__ */
