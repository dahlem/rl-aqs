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

/** @file Simulation.hh
 * This header declares the simulation method, so that it can be considered a
 * black-box simulation by the main method.
 *
 * @author Dominik Dahlem
 */
#ifndef __DES_CORE_SIMULATION_HH__
#define __DES_CORE_SIMULATION_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include "CL.hh"



namespace des
{
namespace core
{

/** @struct sim_output
 * Specifies the output variables of the simulation
 */
struct sim_output
{
    sim_output()
        {}

};


/** @class Simulation
 * Class encapsulating the logic to start the discrete event simulator.
 */
class Simulation
{
public:

    /** @fn static sim_output simulate()
     * start the simulation
     *
     * @return the simulation output variables
     */
    static sim_output simulate(tDesArgsSP);

private:
    Simulation()
        {}

    Simulation(const Simulation&)
        {}

    ~Simulation()
        {}

    void operator=(const Simulation&)
        {}

};


}
}



#endif /* __DES_CORE_SIMULATION_HH__ */
