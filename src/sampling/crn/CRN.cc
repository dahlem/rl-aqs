#include <cstddef>

#include "CRN.hh"



CRN::CRN()
{
    gsl_rng_env_setup();
}

CRN::~CRN()
{
    for (unsigned int i = 0; i < m_gslRngs.size(); ++i) {
        gsl_rng_free(m_gslRngs[i]);
    }
}

const signed int CRN::init(unsigned long int p_seed)
{
    const gsl_rng_type *rng_type = gsl_rng_default;
    gsl_rng *rng = gsl_rng_alloc(rng_type);

    m_gslRngs.push_back(rng);

    return m_gslRngs.size();
}

gsl_rng* CRN::get(const unsigned int p_rng) throw (SamplingException)
{
    if ((p_rng >= 0) && (p_rng < m_gslRngs.size())) {
        return m_gslRngs[p_rng];
    } else {
        throw SamplingException(SamplingException::UNKNOWN_INDEX);
    }
}
