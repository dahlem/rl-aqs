// Copyright (C) 2007, 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef CRN_HH
#define CRN_HH


#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <gsl/gsl_rng.h>

#include <vector>
using std::vector;

#include "SamplingException.hh"

#include "Singleton.hh"
using des::design::Singleton;



namespace des
{
    namespace sampling
    {

typedef shared_ptr<gsl_rng> tGslRngSP;

/**
 * This class implements a layer to provide common random number generators
 * in order to reduce the variance of computer simulations. See the book
 * <ul>
 *   <li>Simulation, Modeling and analysis by Averill M. Law and W.
 *       David Kelton.</li>
 * </ul>
 *
 * A client class using random numbers is adviced to initialise the
 * <code>CRN</code> class with a new seed in order to retrieve an index to a
 * newly allocated GSL random number generator. This generator can then be obtained
 * using this index and therefore avoiding an overlap usage of random number
 * generators.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class CRN
{
public:
    CRN();
    ~CRN();

    /**
     * Initialise a new GSL random number generator.
     *
     * @param usigned long int the random number seed
     * @return the index to the newly allocated GSL random number generator
     */
    const signed int init(unsigned long int p_seed);

    /**
     * Retrieve the random number generator associated with a given index.
     *
     * @param unsigned int the index of the random number generator
     * @return the GSL random number generator
     * @throws SamplingException thrown, if the random number generator cannot
     *                           be returned
     */
    tGslRngSP get(const unsigned int p_rng) throw (SamplingException);


private:

    /**
     * type definition of a vector of random number generators.
     */
    typedef vector<tGslRngSP>  ptr_rng;
    ptr_rng m_gslRngs;

};


/**
 * Define a singleton type of the <code>CRN</code> class.
 */
typedef Singleton<CRN> CRNSingleton;

    }
}

#endif
