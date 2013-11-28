// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <iostream>

#ifdef HAVE_MPI
# include <mpi.h>
#endif /* HAVE_MPI */

#include <boost/cstdint.hpp>

#include "Seeds.hh"
namespace dsample = des::sampling;


int main(int argc, char *argv[])
{
#ifdef HAVE_MPI
    int rank, num_tasks;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);
#endif /* HAVE_MPI */


    dsample::Seeds::getInstance().init("/home/tecsc/ddahlem/des-1.8/seeds-mpi.dat");
    for (boost::uint16_t i = 0; i < 1000; ++i) {
        std::cout << rank << " : " << dsample::Seeds::getInstance().getSeed() << std::endl;
        std::cout.flush();
    }

#ifdef HAVE_MPI
    MPI_Barrier(MPI_COMM_WORLD);
    dsample::Seeds::getInstance().close();

    MPI_Finalize();
#endif /* HAVE_MPI */

    return EXIT_SUCCESS;
}
