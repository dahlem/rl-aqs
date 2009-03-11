// Copyright (C) 2007-2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

/** @file QueueException.cc
 * Implementation of the Queue Exceptions @ref{QueueException.hh}
 */

#include "QueueException.hh"
using des::common::QueueException;


/**
 * Specification of the valid error strings.
 */
const char* QueueException::m_errorStrings[QueueException::NUM_ERRORS] =
{
    "Reset Not Allowed, because the fifo data structure is not empty.",
    "Could not find the rung to complete the enqueue operation.",
    "Requested bucket does not exist.",
    "Requested rung does not exist.",
    "An epoch has not been started with the Ladder structure.",
    "An epoch cannot be created, if another one is not finished yet.",
    "Cannot enqueue past event.",
    "The queue is empty.",
    "Advanced the dequeue bucket ignoring existing events.",
    "Current enqueue Bucket equals the current dequeue bucket."
};

QueueException::QueueException(int p_code)
    : exception(), m_code(p_code)
{}

const char* QueueException::what() const throw()
{
    return m_errorStrings[m_code];
}

int QueueException::errorCode() const
{
    return m_code;
}
