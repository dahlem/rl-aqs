// Copyright (C) 2009 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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

/** @file Stack.hh
 * Specification of a simple stack
 */
#ifndef __DESCOMMONSTACKEXCEPTION_HH__
#define __DESCOMMONSTACKEXCEPTION_HH__

#include <exception>



namespace des
{
namespace common
{

class StackException: public std::exception
{
public:
    StackException(int p_code) : exception(), m_code(p_code)
        {}

    const char* what() const throw()
        { return m_errorStrings[m_code]; }

    int errorCode() const
        {
            return m_code;
        }

    enum Errors {
        ERROR_EMPTY = 0,
        NUM_ERRORS
    };

private:
    static const char* m_errorStrings[NUM_ERRORS];
    int m_code;
};


}
}



#endif
