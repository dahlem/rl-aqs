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
#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <iostream>
#include <cstring>
#include <cstdlib>

#ifdef HAVE_MPI
# include <mpi.h>
#endif /* HAVE_MPI */

enum {
    MASTER_LHS_TAG = 2323,
    LHS_REP_TAG
};


int main(int argc, char *argv[])
{
    MPI_Comm   myComm;       /* intra-communicator of local sub-group */
    MPI_Comm   COMM_TO_LHS;  /* inter-communicator */
    MPI_Comm   COMM_TO_REP; /* second inter-communicator (group 1 only) */
    MPI_Comm   COMM_TO_MASTER; /* second inter-communicator (group 1 only) */
    int membershipKey;
    int rank;
    int numLHS = 2;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* User code must generate membershipKey in the range [0, 1, 2] */
    if (rank == 0) {
        membershipKey = 0;
    } else {
        membershipKey = (rank - 1) / numLHS + 1;
    }

    /* Build intra-communicator for local sub-group */
    MPI_Comm_split(MPI_COMM_WORLD, membershipKey, rank, &myComm);

    /* Build inter-communicators. Tags are hard-coded. */
    if (membershipKey == 0) {
        /* Group 0 communicates with group 1. */
        MPI_Intercomm_create(myComm, 0, MPI_COMM_WORLD, 1, MASTER_LHS_TAG, &COMM_TO_LHS);
    } else if (membershipKey == 1) {
        /* Group 1 communicates with groups 0 and 2. */
        MPI_Intercomm_create(myComm, 1, MPI_COMM_WORLD, 0, MASTER_LHS_TAG, &COMM_TO_MASTER);
        MPI_Intercomm_create(myComm, 1, MPI_COMM_WORLD, numLHS + 1, LHS_REP_TAG, &COMM_TO_REP);
    } else if (membershipKey == 2) {
        /* Group 2 communicates with groups 1. */
        MPI_Intercomm_create(myComm, numLHS + 1, MPI_COMM_WORLD, 1, LHS_REP_TAG, &COMM_TO_LHS);
    }


    /* Do work ... */
    if (membershipKey == 0) {
        // send work items
        // wait for finished work
        // send kill pill
    } else if (membershipKey == 1) {
        // wait for work items
        // send work replications
        // wait for results and return to master
    } else if (membershipKey == 2) {
        // wait for work items until kill pill is sent
    }

    /* free communicators appropriately */
    switch(membershipKey) {
      case 0:
          MPI_Comm_free(&COMM_TO_LHS);
      case 1:
          MPI_Comm_free(&COMM_TO_MASTER);
          MPI_Comm_free(&COMM_TO_REP);
          break;
      case 2:
          MPI_Comm_free(&COMM_TO_LHS);
          break;
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}
