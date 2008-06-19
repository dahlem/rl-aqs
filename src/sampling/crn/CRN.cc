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

#include "CRN.hh"
using des::sampling::CRN;
using des::sampling::tGslRngSP;


CRN::CRN()
{
    gsl_rng_env_setup();
}

CRN::~CRN()
{
}

const signed int CRN::init(unsigned long int p_seed)
{
    const gsl_rng_type *rng_type = gsl_rng_default;

    m_gslRngs.push_back(tGslRngSP(gsl_rng_alloc(rng_type), gsl_rng_free));

    return m_gslRngs.size();
}

tGslRngSP CRN::get(const unsigned int p_rng) throw (SamplingException)
{
    if ((p_rng >= 0) && (p_rng < m_gslRngs.size())) {
        return m_gslRngs[p_rng];
    } else {
        throw SamplingException(SamplingException::UNKNOWN_INDEX);
    }
}
