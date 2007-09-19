#include <cstddef>

#include "QRNSequence.hh"



QRNSequence::QRNSequence(
    const gsl_qrng_type *p_type,
    unsigned int p_dim) 
{
    m_dim = p_dim;

    if (p_type == NULL) {
        p_type = gsl_qrng_sobol;
    }
    
    m_qrng = gsl_qrng_alloc(p_type, m_dim);
}

QRNSequence::~QRNSequence() 
{
    gsl_qrng_free(m_qrng);
}

void QRNSequence::get(double p_sample[], double p_scaling[])
{
    gsl_qrng_get(m_qrng, p_sample);
}
