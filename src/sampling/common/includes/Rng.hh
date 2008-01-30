// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef RNG_HH
#define RNG_HH

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_rng.h>


namespace des
{
    namespace sampling
    {


/**
 * This class provides random number generator rng read from a specified file.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class Rng
{
private:
    Rng();
    ~Rng();

public:
    
    /**
     * sample from the uniform distribution and store the results into
     * a vector with a specified size
     *
     * @param const gsl_rng* the random number generator
     * @param gsl_matrix* the matrix to store the result of the sampling
     */
    static void uniform(const gsl_rng *p_rng, gsl_matrix *p_mat);
};
        

        
    }
}

#endif
