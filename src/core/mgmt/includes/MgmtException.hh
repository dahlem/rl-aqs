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

/** @file MgmtException.hh
 * Declaration of the core management exceptions.
 */

#ifndef __DES_CORE_MGMTEXCEPTION_HH__
#define __DES_CORE_MGMTEXCEPTION_HH__


#include <exception>



namespace des
{
namespace core
{

/**
 * This exception specifies errors that may occur within the management component.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class MgmtException: public std::exception
{
public:
    MgmtException(int p_code);

    const char* what() const throw();
    int errorCode() const;

    enum Errors {
        CHANNEL_NOT_EXIST = 0,
        NODE_NOT_EXIST,
        TIME_NOT_EXIST,
        NUM_ERRORS
    };

private:
    static const char* m_errorStrings[NUM_ERRORS];
    int m_code;
};


    }
}

#endif
