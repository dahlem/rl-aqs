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
#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <iostream>

#ifdef HAVE_MPI
# include <mpi.h>
#endif /* HAVE_MPI */


#include "Seeds.hh"
namespace dsample = des::sampling;


int main(int argc, char *argv[])
{
    int rank, num_tasks;

#ifdef HAVE_MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);
#endif /* HAVE_MPI */


    for (boost::uint16_t i = 0; i < 100; ++i) {
        std::cout << dsample::Seeds::getInstance().getSeed() << std::cout;
        std::cout.flush();
    }

#ifdef HAVE_MPI
    MPI_Finalize();
#endif /* HAVE_MPI */

    return EXIT_SUCCESS;
}
