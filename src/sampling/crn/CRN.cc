// Copyright (C) 2007-2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file CRN.cc
 * Implementation of the common random number interface @ref{CRN.hh}
 */
#include <iostream>

#include <gsl/gsl_rng.h>

#include "CRN.hh"
namespace dsample = des::sampling;


dsample::CRN::CRN()
{
    gsl_rng_env_setup();
}


dsample::CRN::~CRN()
{
}


const boost::int32_t dsample::CRN::init(const boost::intmax_t p_seed)
{
    const gsl_rng_type *rng_type = gsl_rng_default;
    dsample::tGslRngSP rng = tGslRngSP(gsl_rng_alloc(rng_type), gsl_rng_free);

    gsl_rng_set(rng.get(), p_seed);
    m_gslRngs.push_back(rng);

    return (m_gslRngs.size() - 1);
}


dsample::tGslRngSP dsample::CRN::get(const boost::uint32_t p_rng) throw (SamplingException)
{
    if (p_rng < m_gslRngs.size()) {
        return m_gslRngs[p_rng];
    } else {
        throw SamplingException(SamplingException::UNKNOWN_INDEX);
    }
}


void dsample::CRN::log(const boost::intmax_t p_seed, std::string eventType)
{
    std::cout << "Use seed " << p_seed << " for the " << eventType << "." << std::endl;
}


void dsample::CRN::log(const boost::intmax_t p_min, const boost::intmax_t p_max,
                       std::string eventType)
{
    std::cout << "Use seed indeces [" << p_min << ", " << p_max << "] for the " << eventType << "." << std::endl;
}
