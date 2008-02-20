// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef STATSEXCEPTION_HH
#define STATSEXCEPTION_HH


#include <exception>
using std::exception;



namespace des
{
    namespace statistics
    {

/**
 * This exception specifies errors that may occur within the statistics module.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class StatsException: public exception
{
public:
    StatsException(int p_code);

    const char* what() const throw();

    enum Errors {
        DIM_MATCH_ERROR,
        VEC_NOT_POSITIVE,
        VEC_MAT_COL_DIM_MATCH_ERROR,
        MAT_ROW_TOO_SMALL,
        MAT_NOT_SQUARE,
        MAT_DIM_MATCH_ERROR,
        NUM_ERRORS
    };

private:
    static const char* m_errorStrings[NUM_ERRORS];
    int m_code;
};


    }
}

#endif
