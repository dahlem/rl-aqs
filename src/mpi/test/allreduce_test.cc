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

#ifdef HAVE_MPI
# include <mpi.h>
#endif /* HAVE_MPI */

#include <cstdlib>
#include <iostream>

int main(int argc, char *argv[])
{
    int *out;
    int rank, size, colour;
    MPI_Comm group_comm;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    out = new int[1];

    if (rank == 0) {
        colour = 0;
    } else {
        colour = ((rank - 1) / 3) + 1;
    }

    // split the world in 2
    MPI_Comm_split(MPI_COMM_WORLD, colour, rank, &group_comm);

    // find out the minimum rank per group
    MPI_Allreduce(&rank, out, 1, MPI_INT, MPI_MIN, group_comm);

    if (rank == *out) {
        std::cout << "Group: " << colour << ", process: " << rank << " has minimum rank " << *out << std::endl;
    } else {
        std::cout << "Group: " << colour << ", process: " << rank << " does not have minimum rank " << *out << std::endl;
    }

    delete [] out;

    MPI_Comm_free(&group_comm);
    MPI_Finalize();

    return EXIT_SUCCESS;
}
