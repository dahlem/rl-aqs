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

#ifdef HAVE_MPI
# include <mpi.h>
#endif /* HAVE_MPI */

#include <boost/shared_ptr.hpp>

#include "common.hh"
#include "CL.hh"

#include "DirectedGraph.hh"
namespace dnet = des::network;



namespace des
{
namespace core
{



/** @class Simulation
 * Class encapsulating the logic to start the discrete event simulator.
 */
class Simulation
{
public:
    Simulation()
        {}

    ~Simulation()
        {}

    /** @fn sim_output simulate()
     * start the simulation
     *
     * @return the simulation output variables
     */
#ifdef HAVE_MPI
    void simulate(MPI_Datatype&, MPI_Datatype&, MPI_Comm&, tDesArgsSP);
#else
    sim_output operator()(tDesArgsSP);
#endif /* HAVE_MPI */

private:

    Simulation(const Simulation&)
        {}

    void operator=(const Simulation&)
        {}

    static dnet::tGraphSP createGraph(tDesArgsSP, boost::uint16_t, boost::uint16_t,
                                      double, double, double);


};

typedef boost::shared_ptr <Simulation> SimSP;

}
}



#endif /* __DES_CORE_SIMULATION_HH__ */
