// Copyright (C) 2007-2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <cstddef>
#include <iostream>
#include <string>

#ifdef HAVE_MPI
# include <mpi.h>
# include <mpio.h>
#else
# include <boost/lexical_cast.hpp>
#endif /* HAVE_MPI */

#include "Seeds.hh"


namespace des
{
namespace sampling
{


Seeds::Seeds()
{
}

Seeds::Seeds(const Singleton<Seeds> &)
{
}

Seeds::~Seeds()
{
#ifndef HAVE_MPI
    if (is != NULL) {
        if (is.is_open()) {
            is.close();
        }
    }
#endif /* HAVE_MPI */
}

#ifdef HAVE_MPI
void Seeds::close()
{
    int rc;
    rc = MPI_File_close(&fh);
    if (rc != MPI_SUCCESS) {
        std::cerr << "Error closing seed file." << std::endl;
        std::cerr.flush();
    }
}


void Seeds::init(const char *p_file) throw (SamplingException)
{
    int rc;

    if (!isInitialised) {
        rc = MPI_File_open(
            MPI_COMM_WORLD, const_cast<char *> (p_file), MPI_MODE_RDONLY | MPI_MODE_SEQUENTIAL, MPI_INFO_NULL, &fh);
        if (rc != MPI_SUCCESS) {
            std::cerr << "Error opening seed file " << p_file << "." << std::endl;
            std::cerr.flush();
            MPI_Abort(MPI_COMM_WORLD, 99);
        }
    } else {
        throw SamplingException(SamplingException::ALREADY_INITIALISED);
    }
}

#else
void Seeds::init(const char *p_file) throw (SamplingException)
{
    if (!isInitialised) {
        if (!is.is_open()) {
            is.open(p_file);
            fromStream = true;

            if (!is.is_open()) {
                throw SamplingException(SamplingException::NO_SEEDS_FILE);
            }
        }
    } else {
        throw SamplingException(SamplingException::ALREADY_INITIALISED);
    }
}

void Seeds::init() throw (SamplingException)
{
    if (!isInitialised) {
        seed = CRN::getInstance().init(gsl_rng_default_seed);
        seeds_rng = CRN::getInstance().get(seed);
        fromStream = false;

        CRN::getInstance().log(gsl_rng_default_seed, "seeds");
    } else {
        throw SamplingException(SamplingException::ALREADY_INITIALISED);
    }
}
#endif /* HAVE_MPI */

const boost::uint32_t Seeds::getSeed() throw (SamplingException)
{
    boost::uint32_t newSeed = 0;

#ifdef HAVE_MPI
    MPI_Status status;
    int rc;
    int *buf = new int[1];

    rc = MPI_File_read_shared(fh, buf, 1, MPI_INT, &status);
    if (rc != MPI_SUCCESS) {
        std::cerr << "Error reading from seed file." << std::endl;
        std::cerr.flush();
    }

    newSeed = static_cast<boost::uint32_t>(buf[0]);
    delete[] buf;
#else
    std::string line;

    if (fromStream) {
        if (is.is_open()) {
            if (!is.eof()) {
                std::getline(is, line);
                try {
                    newSeed = boost::lexical_cast<unsigned long>(line);
                } catch (boost::bad_lexical_cast&) {
                    throw SamplingException(SamplingException::NOT_A_NUMBER);
                }
            } else {
                throw SamplingException(SamplingException::NO_SEEDS);
            }
        } else {
            throw SamplingException(SamplingException::NOT_INITIALISED);
        }
    } else {
        newSeed = gsl_rng_uniform_int(seeds_rng.get(), gsl_rng_max(seeds_rng.get()));
    }
#endif /* HAVE_MPI */

#ifndef NDEBUG
    std::cout << "Return seed: " << newSeed << std::endl;
    std::cout.flush();
#endif /* NDEBUG */

    return newSeed;
}

}
}
