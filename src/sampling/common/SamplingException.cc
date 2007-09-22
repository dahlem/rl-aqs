// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "SamplingException.hh"
using des::sampling::SamplingException;



const char* SamplingException::m_errorStrings[NUM_ERRORS] =
{
    "The random number generator index does not exist."
};

SamplingException::SamplingException(int p_code)
    : exception(), m_code(p_code)
{}

const char* SamplingException::what() const throw()
{
    return m_errorStrings[m_code];
}
