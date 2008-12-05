// Copyright (C) 2007-2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include <cstddef>
#include <iostream>
#include <string>

#include <boost/lexical_cast.hpp>

#include "Seeds.hh"


namespace des
{
namespace sampling
{


Seeds::Seeds()
{}

Seeds::Seeds(const Singleton<Seeds> &)
{}

Seeds::~Seeds()
{
    if (is != NULL) {
        if (is.is_open()) {
            is.close();
        }
    }
}

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
        seeds_rng = CRN::getInstance().get(seed - 1);
        fromStream = false;

        CRN::getInstance().log(gsl_rng_default_seed, "seeds");
    } else {
        throw SamplingException(SamplingException::ALREADY_INITIALISED);
    }
}

const boost::uint32_t Seeds::getSeed() throw (SamplingException)
{
    std::string line;
    boost::uint32_t newSeed = 0;

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

    return newSeed;
}

}
}
