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
#include <ostream>
#include <sstream>
#include <string>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file.hpp>
namespace bio = boost::iostreams;

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include <gsl/gsl_permutation.h>
#include <gsl/gsl_randist.h>

#ifdef HAVE_MPI
# include <mpi.h>
# ifndef HAVE_OPENMPI
#  include <mpio.h>
# endif
#endif /* HAVE_MPI */


#define SIZE 100000000


int main(int argc, char *argv[])
{
#ifdef HAVE_MPI
    int rank, rc;
    MPI_File fh_write;
    MPI_Status status;
    std::string seeds_file = "/home/tecsc/dahlemd/seeds.dat";
    std::string seeds_file_mpi = "/home/tecsc/dahlemd/seeds-mpi.dat";
    gsl_permutation *p;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    if (rank == 0) {
        gsl_rng_env_setup();

        const gsl_rng_type *rng_type = gsl_rng_default;
        gsl_rng *rng = gsl_rng_alloc(rng_type);

        rc = MPI_File_open(
            MPI_COMM_SELF,
            const_cast<char *> (seeds_file_mpi.c_str()),
            MPI_MODE_CREATE | MPI_MODE_RDWR,
            MPI_INFO_NULL, &fh_write);
        if (rc != MPI_SUCCESS) {
            std::cerr << "Error opening seed file /home/tecsc/ddahlem/seeds-mpi.dat" << "." << std::endl;
            std::cerr.flush();
            MPI_Abort(MPI_COMM_WORLD, 99);
        }

        bio::stream_buffer<bio::file_sink> buf(seeds_file);
        std::ostream os(&buf);

        // convert to int and cout
        p = gsl_permutation_calloc(SIZE);
        gsl_ran_shuffle(rng, p->data, SIZE, sizeof(size_t));

        for (int i = 0; i < SIZE; i++) {
            int newSeed = p->data[i];

            MPI_File_write(fh_write, &newSeed, 1, MPI_INT, &status);
            os << newSeed << std::endl;
        }

        rc = MPI_File_close(&fh_write);
        if (rc != MPI_SUCCESS) {
            std::cerr << "Error closing seed-mpi file." << std::endl;
            std::cerr.flush();
        }

        gsl_permutation_free(p);
        gsl_rng_free(rng);
    }


    MPI_Finalize();
#endif /* HAVE_MPI */

    return EXIT_SUCCESS;
}
