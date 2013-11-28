// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "StatsException.hh"
using des::statistics::StatsException;


/**
 * Specification of the valid error strings.
 */
const char* StatsException::m_errorStrings[NUM_ERRORS] =
{
    "Dimensions of the given vectors must match."
    "The vector elements are not strictly positive."
    "The dimension of the vector has to match the column dimension of the matrix."
    "The row dimension of the matrix is too small."
    "The matrix is not square."
    "The matrix dimensions don't match."
};

StatsException::StatsException(int p_code)
    : exception(), m_code(p_code)
{}

const char* StatsException::what() const throw()
{
    return m_errorStrings[m_code];
}
