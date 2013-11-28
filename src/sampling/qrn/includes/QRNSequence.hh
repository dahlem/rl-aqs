// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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



namespace des
{
    namespace sampling
    {
        
/**
 * This class provides quasi-random number sequences for space-filling computer
 * simulation designs using the quasi-random number generators provided
 * by GSL.
 *
 * @author <a href="mailto:Dominik.Dahlem@gmail.com">Dominik Dahlem</a>
 */
class QRNSequence
{
public:
    /**
     * Construct a <code>QRNSequence</code> object with a specified QRN type
     * and the dimension of the hypercube to be sampled.
     *
     * @param gsl_qrng_type* the quasi-random number generator
     * @param unsigned int the dimension of the hypercube
     */
    QRNSequence(
        const gsl_qrng_type *p_type,
        unsigned int p_dim);
    ~QRNSequence();

    /**
     * Retrieve the next sample vector of the sequence.
     *
     * @param double[] the vector to be filled with the next sequence
     * @param double[] the scaling factors for each element of the vector
     */
    void get(double p_sample[], double p_scaling[]);
    
private:
    gsl_qrng *m_qrng;
    int m_dim;
};


    }
}

#endif
