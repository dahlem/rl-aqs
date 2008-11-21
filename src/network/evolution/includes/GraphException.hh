// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file GraphException.hh
 * Declaration of the Graph Exceptions.
 */

#ifndef __GRAPHEXCEPTION_HH__
#define __GRAPHEXCEPTION_HH__


#include <exception>



namespace des
{
    namespace network
    {

/**
 * This exception specifies errors that may occur within graphs.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class GraphException: public std::exception
{
public:
    GraphException(int p_code);

    const char* what() const throw();
    int errorCode() const;

    enum {
        GRAPH_INPUT_ERROR = 0,
        GRAPH_READ_ERROR,
        NUM_ERRORS
    };

private:
    static const char* m_errorStrings[NUM_ERRORS];
    int m_code;
};


    }
}

#endif /* __GRAPHEXCEPTION_HH__ */
