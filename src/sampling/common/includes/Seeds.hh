// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef SEEDS_HH
#define SEEDS_HH


#include <fstream>
using std::ifstream;

#include "SamplingException.hh"

#include "Singleton.hh"
using des::design::Singleton;


namespace des
{
    namespace sampling
    {


/**
 * This class provides random number generator seeds read from a specified file.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class Seeds
{
public:
    Seeds();
    Seeds(const Seeds&);
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

    /**
     * Retrieve the next seed from the seeds file.
     *
     * @return a long integer
     * @throws SamplingException thrown, if no seeds are available any more or
     *                           the file has not been opened.
     */
    const unsigned long getSeed() throw (SamplingException);

private:
    ifstream is;
    
};
        

/**
 * Define a singleton type of the <code>Seeds</code> class.
 */
typedef Singleton<Seeds> SeedsSingleton;
        
    }
}

#endif
