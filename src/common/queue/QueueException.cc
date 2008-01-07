// Copyright (C) 2007-2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "QueueException.hh"
using des::common::QueueException;


/**
 * Specification of the valid error strings.
 */
const char* QueueException::m_errorStrings[NUM_ERRORS] =
{
    "Reset Not Allowed, because the fifo data structure is not empty."
    "Could not find the rung to complete the enqueue operation."
    "Requested bucket does not exist."
    "Requested rung does not exist."
    "An epoch has not been started with the Ladder structure."
    "An epoch cannot be created, if another one is not finished yet."
    "Cannot enqueue past event."
};

QueueException::QueueException(int p_code)
    : exception(), m_code(p_code)
{}

const char* QueueException::what() const throw()
{
    return m_errorStrings[m_code];
}
