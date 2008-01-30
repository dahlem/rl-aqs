// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef LHS_HH
#define LHS_HH


#include <gsl/gsl_rng.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

#include "SamplingException.hh"
using des::sampling::SamplingException;


namespace des
{
    namespace sampling
    {
        
/**
 * This class implements Latin Hypercube Sampling with re-scaling.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class LHS
{
private:
    /**
     * Hide the constructors, because we don't want an instance to be created.
     */
    LHS();
    ~LHS();

public:
    
    /**
     * Get the latin hypercube sample.
     *
     * @param const gsl_rng* the random number generator
     * @param const gsl_vector* the vector with the minimum values for each dimension
     * @param const gsl_vector* the vector with the maximum values for each dimension
     * @param int the sample size
     * @param gsl_matrix** the obtained sample
     * @throws SamplingException, if an error occurs during the sampling.
     */
    static void sample(const gsl_rng *p_rng, const gsl_vector *p_min,
                       const gsl_vector *p_max, int p_size, gsl_matrix **p_mat)
        throw (SamplingException);
    
};


    }
}

#endif
