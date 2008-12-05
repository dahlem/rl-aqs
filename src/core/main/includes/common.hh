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

namespace des
{
namespace core
{

/** @struct sim_output
 * Specifies the output variables of the simulation
 */
struct sim_output
{
    // single simulation output
    double system_average_delay;
    double system_expected_average_num_in_queue;

    // average simulation output
    double mean_system_average_delay;
    double mean_system_expected_average_num_in_queue;
    double sd_system_average_delay;
    double sd_system_expected_average_num_in_queue;

    sim_output()
        : system_average_delay(0.0),
          system_expected_average_num_in_queue(0.0),
          mean_system_average_delay(0.0),
          mean_system_expected_average_num_in_queue(0.0),
          sd_system_average_delay(0.0),
          sd_system_expected_average_num_in_queue(0.0)
        {}

};


}
}



#endif /* __DES_CORE_COMMON_HH__ */
