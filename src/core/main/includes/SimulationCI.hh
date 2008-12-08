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

#include <iostream>
#include <sstream>
#include <string>

#include <boost/shared_ptr.hpp>

#include "common.hh"
#include "CL.hh"

#include "CI.hh"
#include "OnlineStats.hh"
namespace dstats = des::statistics;

#include "Results.hh"
namespace dio = des::io;


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
            std::stringstream outDir, csv_line;
            outDir << p_desArgs->results_dir << "/" << p_desArgs->sim_num;

            std::string dir = outDir.str();
            std::string file = "replica_results.dat";

            dio::tResultsSP replica_output(
                new dio::Results(file, dir));

            sim_output output, result;
            dstats::OnlineStats avgDelay;
            dstats::OnlineStats avgNumEvents;

            csv_line << "sim_num,rep_num,systemDelay,systemAvgNumEvents,meanDelay,varDelay,meanAvgNumEvents,varAvgNumEvents";
            replica_output->print(csv_line);

            // start 2 experiments
            for (p_desArgs->rep_num = 1; p_desArgs->rep_num < m_initialExp; ++p_desArgs->rep_num) {
                csv_line.str("");
                output = m_dsim->simulate(p_desArgs);
                avgDelay.push(output.system_average_delay);
                avgNumEvents.push(output.system_expected_average_num_in_queue);
                csv_line << p_desArgs->sim_num << "," << p_desArgs->rep_num << ","
                         << output.system_average_delay << "," << output.system_expected_average_num_in_queue
                         << "," << avgDelay.mean() << "," << avgDelay.variance() << ","
                         << avgNumEvents.mean() << "," << avgNumEvents.variance();
                replica_output->print(csv_line);
            }

            while (!dstats::CI::isConfidentWithPrecision(
                         avgDelay.mean(),
                         avgDelay.variance(),
                         avgDelay.getNumValues(), m_alpha, m_error)
                     && !dstats::CI::isConfidentWithPrecision(
                         avgNumEvents.mean(),
                         avgNumEvents.variance(),
                         avgNumEvents.getNumValues(), m_alpha, m_error))
            {
                csv_line.str("");
                output = m_dsim->simulate(p_desArgs);
                avgDelay.push(output.system_average_delay);
                avgNumEvents.push(output.system_expected_average_num_in_queue);
                csv_line << p_desArgs->sim_num << "," << p_desArgs->rep_num << ","
                         << output.system_average_delay << "," << output.system_expected_average_num_in_queue
                         << "," << avgDelay.mean() << "," << avgDelay.variance() << ","
                         << avgNumEvents.mean() << "," << avgNumEvents.variance();
                replica_output->print(csv_line);
                (p_desArgs->rep_num)++;
            }

            result.mean_system_average_delay = avgDelay.mean();
            result.sd_system_average_delay = avgDelay.standardDeviation();
            result.mean_system_expected_average_num_in_queue = avgNumEvents.mean();
            result.sd_system_expected_average_num_in_queue = avgNumEvents.standardDeviation();

            if (p_desArgs->rep_num > m_initialExp) {
                result.replications = p_desArgs->rep_num - 1;
            } else {
                result.replications = p_desArgs->rep_num;
            }

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
