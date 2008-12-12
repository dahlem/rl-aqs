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

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <iostream>
#include <string>
#include <streambuf>

#ifdef HAVE_MPI
# include <mpi.h>
# include <mpio.h>
#endif /* HAVE_MPI */

#include <boost/cstdint.hpp>
#include <boost/lexical_cast.hpp>


#define FILE_BUFSIZE    4096
#define INT_BUFSIZE     32


int main(int argc, char *argv[])
{
#ifdef HAVE_MPI
    int rank, num_tasks, rc, counter, characters_read;
    MPI_File fh, fh_write;
    MPI_Status status;
    char *file_buffer;
    char *int_buffer;
    char temp;
    std::char_traits<char>::int_type asInt;
    std::string file = "/home/tecsc/ddahlem/des-1.8/seeds.dat";
    std::string out_file = "/home/tecsc/ddahlem/des-1.8/seeds-mpi.dat";

    file_buffer = new char[FILE_BUFSIZE];
    int_buffer = new char[INT_BUFSIZE];
    memset(file_buffer, '\0', FILE_BUFSIZE * sizeof(char));
    memset(int_buffer, '\0', INT_BUFSIZE * sizeof(char));
    characters_read = counter = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);

    if (rank == 0) {
        rc = MPI_File_open(
            MPI_COMM_SELF, const_cast<char *> (file.c_str()), MPI_MODE_RDONLY, MPI_INFO_NULL, &fh );
        if (rc != MPI_SUCCESS) {
            std::cerr << "Error opening seed file " << file << "." << std::endl;
            std::cerr.flush();
            MPI_Abort(MPI_COMM_WORLD, 99);
        }
        rc = MPI_File_open(
            MPI_COMM_SELF, const_cast<char *> (out_file.c_str()), MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fh_write );
        if (rc != MPI_SUCCESS) {
            std::cerr << "Error opening seed file /home/tecsc/ddahlem/des-1.8/seeds-mpi.dat" << "." << std::endl;
            std::cerr.flush();
            MPI_Abort(MPI_COMM_WORLD, 99);
        }
        rc = MPI_File_read(fh, file_buffer, FILE_BUFSIZE, MPI_CHAR, &status);
        if (rc != MPI_SUCCESS) {
            std::cerr << "Error reading from seed file." << std::endl;
            std::cerr.flush();
        }

        while (true) {
            if (characters_read == (FILE_BUFSIZE - 1)) {
                // read new FILE_BUFSIZE
                memset(file_buffer, '\0', FILE_BUFSIZE * sizeof(char));

                rc = MPI_File_read(fh, file_buffer, FILE_BUFSIZE, MPI_CHAR, &status);
                if (rc != MPI_SUCCESS) {
                    std::cerr << "Error reading from seed file." << std::endl;
                    std::cerr.flush();
                }

                characters_read = 0;
            }

            temp = file_buffer[characters_read];
            asInt = std::char_traits<char>::to_int_type(temp);

            // have we reached the end of the file
            if (!(std::char_traits<char>::not_eof(asInt)) || (temp == '\0')) {
                break;
            }

            // if counter == 0, blank int_buffer
            if (counter == 0) {
                memset(int_buffer, '\0', INT_BUFSIZE * sizeof(char));
            }

            // fill int_buffer until eol
            if (temp != '\n') {
                int_buffer[counter] = temp;
                counter++;
            } else {
                // convert to int and cout
                try {
                    int newSeed;
                    std::string line(int_buffer);

                    newSeed = boost::lexical_cast<int>(line);
                    std::cout << newSeed << std::endl;
                    std::cout.flush();
                    MPI_File_write(fh_write, &newSeed, 1, MPI_INT, &status);
                } catch (boost::bad_lexical_cast&) {
                    std::cout << "number format exception" << std::endl;
                }
                // reset the counter
                counter = 0;
            }

            characters_read++;
        }

        rc = MPI_File_close(&fh);
        if (rc != MPI_SUCCESS) {
            std::cerr << "Error closing seed file." << std::endl;
            std::cerr.flush();
        }

        rc = MPI_File_close(&fh_write);
        if (rc != MPI_SUCCESS) {
            std::cerr << "Error closing seed-mpi file." << std::endl;
            std::cerr.flush();
        }
    }

    delete[] file_buffer;
    delete[] int_buffer;

    MPI_Finalize();
#endif /* HAVE_MPI */

    return EXIT_SUCCESS;
}
