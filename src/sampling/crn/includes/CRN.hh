// Copyright (C) 2007, 2008, 2009 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file CRN.hh
 * Declaration of the common random number interface.
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef CRN_HH
#define CRN_HH

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <string>
#include <vector>

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>

#include <gsl/gsl_rng.h>

#include "SamplingException.hh"

#include "Singleton.hh"
using des::design::Singleton;



namespace des
{
    namespace sampling
    {

/** @typedef tGslRngSP
 * Typedef of the shared pointer to a GSL random number generator.
 */
typedef boost::shared_ptr<gsl_rng> tGslRngSP;


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
 * @author <a href="mailto:Dominik.Dahlem@gmail.com">Dominik Dahlem</a>
 */
class CRN : public Singleton <CRN>
{
public:

    /** @fn init(unsigned long int p_seed)
     * Initialise a new GSL random number generator.
     *
     * @param boost::intmax_t the random number seed
     * @return the index to the newly allocated GSL random number generator
     */
    const boost::int32_t init(const boost::intmax_t p_seed);

    /** @fn get(const unsigned int p_rng) throw (SamplingException)
     * Retrieve the random number generator associated with a given index.
     *
     * @param const boost::uint32_t the index of the random number generator
     * @return the GSL random number generator
     * @throws SamplingException thrown, if the random number generator cannot
     *                           be returned
     */
    tGslRngSP get(const boost::uint32_t p_rng) throw (SamplingException);

    /** @fn log(std::string eventType)
     * Log the seed and its purpose of use.
     *
     * @param const boost::intmax_t the seed of the random number generator
     * @param std::string the event type
     */
    void log(const boost::intmax_t p_seed, std::string eventType);

    /** @fn log(std::string eventType)
     * Log the seed and its purpose of use.
     *
     * @param const boost::intmax_t the min seed of the random number generator
     * @param const boost::intmax_t the max seed of the random number generator
     * @param std::string the event type
     */
    void log(const boost::intmax_t p_min, const boost::intmax_t p_max,
             std::string eventType);


    CRN();
    ~CRN();

private:

    /** @typedef ptr_rng
     * type definition of a vector of random number generators.
     */
    typedef std::vector<tGslRngSP>  ptr_rng;
    ptr_rng m_gslRngs;

};


    }
}

#endif
