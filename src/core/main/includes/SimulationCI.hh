// Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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

#ifdef HAVE_OPENMP
# include <omp.h>
#endif /* HAVE_OPENMP */

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
#include "FsUtils.hh"
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
    SimulationCI(double p_alpha, double p_error, boost::uint16_t p_initialExp)
        : m_alpha(p_alpha), m_error(p_error), m_initialExp(p_initialExp)
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
            outDir << p_desArgs->results_dir << "/";

            if (!p_desArgs->add_sim.empty()) {
                std::string parentDir = outDir.str();
                p_desArgs->sim_num = dio::FsUtils::directories(parentDir) + 1;
            }

            outDir << p_desArgs->sim_num;

            std::string dir = outDir.str();
            std::string file = "replica_results.dat";

            dio::Results replica_output(file, dir);

            sim_output output, result;
            dstats::OnlineStats avgDelay;
            dstats::OnlineStats avgNumEvents;
            dstats::OnlineStats totalQ;

            if (p_desArgs->add_sim.empty()) {
                csv_line << "sim_num,rep_num,systemDelay,systemAvgNumEvents,systemTotalQ";
                replica_output.print(csv_line);
            }

            // start 2 experiments
            for (p_desArgs->rep_num = 1; p_desArgs->rep_num <= m_initialExp; p_desArgs->rep_num++) {
                csv_line.str("");
                Simulation sim;
                output = sim(p_desArgs);

                // update the online statistics
                avgDelay.push(output.system_average_delay);
                avgNumEvents.push(output.system_expected_average_num_in_queue);
                totalQ.push(output.system_total_q);

                // output the new statistics
                csv_line << p_desArgs->sim_num << ","
                         << p_desArgs->rep_num << ","
                         << output.system_average_delay << ","
                         << output.system_expected_average_num_in_queue << ","
                         << output.system_total_q;

                replica_output.print(csv_line);
            }

            bool isConfident =
                dstats::CI::isConfidentWithPrecision(
                    avgDelay.mean(),
                    avgDelay.variance(),
                    avgDelay.getNumValues(), m_alpha, m_error)
                && dstats::CI::isConfidentWithPrecision(
                    avgNumEvents.mean(),
                    avgNumEvents.variance(),
                    avgNumEvents.getNumValues(), m_alpha, m_error)
                && dstats::CI::isConfidentWithPrecision(
                    totalQ.mean(),
                    totalQ.variance(),
                    totalQ.getNumValues(), m_alpha, m_error);

            while (!isConfident) {
#ifndef NDEBUG
                std::cout << "Simulation " << output.simulation_id << ", replications: "
                          << output.replications << " is confident: " << isConfident << std::endl;
                std::cout.flush();
#endif /* NDEBUG */

                csv_line.str("");
                Simulation sim;
                output = sim(p_desArgs);

                avgDelay.push(output.system_average_delay);
                avgNumEvents.push(output.system_expected_average_num_in_queue);
                totalQ.push(output.system_total_q);

                csv_line << p_desArgs->sim_num << ","
                         << p_desArgs->rep_num << ","
                         << output.system_average_delay << ","
                         << output.system_expected_average_num_in_queue << ","
                         << output.system_total_q;

#ifndef NDEBUG
                std::cout << csv_line.str() << std::endl << std::flush;
#endif /* NDEBUG */

                replica_output.print(csv_line);
                (p_desArgs->rep_num)++;

                isConfident =
                    dstats::CI::isConfidentWithPrecision(
                        avgDelay.mean(),
                        avgDelay.variance(),
                        avgDelay.getNumValues(), m_alpha, m_error)
                    && dstats::CI::isConfidentWithPrecision(
                        avgNumEvents.mean(),
                        avgNumEvents.variance(),
                        avgNumEvents.getNumValues(), m_alpha, m_error)
                    && dstats::CI::isConfidentWithPrecision(
                        totalQ.mean(),
                        totalQ.variance(),
                        totalQ.getNumValues(), m_alpha, m_error);
            }

            result.mean_system_average_delay = avgDelay.mean();
            result.sd_system_average_delay = avgDelay.variance();
            result.mean_system_expected_average_num_in_queue = avgNumEvents.mean();
            result.sd_system_expected_average_num_in_queue = avgNumEvents.variance();
            result.mean_system_total_q = totalQ.mean();
            result.sd_system_total_q = totalQ.variance();

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


    double m_alpha;
    double m_error;
    boost::uint16_t m_initialExp;
};


}
}



#endif /* __DES_CORE_SIMULATIONCI_HH__ */
