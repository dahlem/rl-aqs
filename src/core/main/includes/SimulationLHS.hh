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

#include <boost/shared_ptr.hpp>

#include "common.hh"
#include "CL.hh"



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
            return m_dsim->simulate(p_desArgs);
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
