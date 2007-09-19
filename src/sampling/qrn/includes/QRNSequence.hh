// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

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
