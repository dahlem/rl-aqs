// Copyright (C) 2007, 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __DES_SAMPLING_SEEDS_HH__
#define __DES_SAMPLING_SEEDS_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */


#include <fstream>

#include <boost/cstdint.hpp>

#include <gsl/gsl_randist.h>

#include "Singleton.hh"
namespace ddesign = des::design;

#include "CRN.hh"
#include "SamplingException.hh"


namespace des
{
    namespace sampling
    {


/**
 * This class provides random number generator seeds read from a specified file.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class Seeds : public ddesign::Singleton <Seeds>
{
public:
    Seeds();
    ~Seeds();

    /**
     * initialise the <code>Seeds</code> class with a file to read the
     * random seeds from.
     *
     * @param string the file to read the seeds from
     * @throws SamplingException thrown, if the file does not exist or
     *                           cannot be opened.
     */
    void init(const char *p_file) throw (SamplingException);
    void init() throw (SamplingException);

    /**
     * Retrieve the next seed from the seeds file.
     *
     * @return a long integer
     * @throws SamplingException thrown, if no seeds are available any more or
     *                           the file has not been opened.
     */
    const boost::uint32_t getSeed() throw (SamplingException);

private:
    Seeds(const Singleton<Seeds> &);

    std::ifstream is;
    tGslRngSP seeds_rng;

    int seed;
    bool fromStream;
    bool isInitialised;

};


    }
}

#endif
