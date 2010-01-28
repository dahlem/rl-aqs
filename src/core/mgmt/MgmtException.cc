// Copyright (C) 2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file MgmtException.cc
 * Implementation of the MgmtExceptions @ref{MgmtException.hh}
 */

#include "MgmtException.hh"


namespace des
{
namespace core
{


/**
 * Specification of the valid error strings.
 */
const char* MgmtException::m_errorStrings[MgmtException::NUM_ERRORS] =
{
    "The Channel does not exist."
};

MgmtException::MgmtException(int p_code)
    : exception(), m_code(p_code)
{}

const char* MgmtException::what() const throw()
{
    return m_errorStrings[m_code];
}

int MgmtException::errorCode() const
{
    return m_code;
}


}
}
