// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef SAMPLINGEXCEPTION_HH
#define SAMPLINGEXCEPTION_HH


#include <exception>
using std::exception;


namespace des
{
    namespace sampling
    {
        

/**
 * This exception specifies errors that may occur during sampling operations.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class SamplingException: public exception
{
public:
    SamplingException(int p_code);

    const char* what() const throw();
    
    enum Errors {
        UNKNOWN_INDEX,
        NO_SEEDS,
        NO_SEEDS_FILE,
        NOT_INITIALISED,
        NOT_A_NUMBER,
        NUM_ERRORS
    };

private:
    static const char* m_errorStrings[NUM_ERRORS];
    int m_code;
};

    }
}

#endif
