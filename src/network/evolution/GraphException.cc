// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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

/** @file GraphException.cc
 * Implementation of the Graph Exceptions @ref{GraphException.hh}
 */

#include "GraphException.hh"
using des::network::GraphException;


/**
 * Specification of the valid error strings.
 */
const char* GraphException::m_errorStrings[GraphException::NUM_ERRORS] =
{
    "Could not read graph from file.",
    "Reading a graph from a file yielded an error."
};

GraphException::GraphException(int p_code)
    : exception(), m_code(p_code)
{}

const char* GraphException::what() const throw()
{
    return m_errorStrings[m_code];
}

int GraphException::errorCode() const
{
    return m_code;
}
