#ifndef QRNSEQUENCE_HH
#define QRNSEQUENCE_HH


#include <gsl/gsl_qrng.h>



class QRNSequence
{
public:
    QRNSequence(
        const gsl_qrng_type *p_type,
        unsigned int p_dim);
    ~QRNSequence();

    void get(double p_sample[], double p_scaling[]);
    
private:
    gsl_qrng *m_qrng;
    int m_dim;
};



#endif
