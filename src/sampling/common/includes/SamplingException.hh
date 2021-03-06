// Copyright (C) 2007, 2008 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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
 * @author <a href="mailto:Dominik.Dahlem@gmail.com">Dominik Dahlem</a>
 */
class SamplingException: public exception
{
public:
    SamplingException(int p_code);

    const char* what() const throw();
    int errorCode() const;

    enum Errors {
        UNKNOWN_INDEX = 0,
        NO_SEEDS,
        NO_SEEDS_FILE,
        NOT_INITIALISED,
        ALREADY_INITIALISED,
        NOT_A_NUMBER,
        DIM_DONT_MATCH,
        SAMPLE_SIZE_TOO_SMALL,
        GSL_RNG_NULL,
        NUM_ERRORS
    };

private:
    static const char* m_errorStrings[NUM_ERRORS];
    int m_code;
};

    }
}

#endif
