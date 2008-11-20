// Copyright (C) 2007,2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file QueueException.hh
 * Declaration of the Queue Exceptions.
 */

#ifndef QUEUEEXCEPTION_HH
#define QUEUEEXCEPTION_HH


#include <exception>



namespace des
{
    namespace common
    {

/**
 * This exception specifies errors that may occur within queues.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class QueueException: public std::exception
{
public:
    QueueException(int p_code);

    const char* what() const throw();
    int errorCode() const;

    enum {
        BAD_RESET = 0,
        RUNG_NOT_FOUND = 1,
        RUNG_OUT_OF_BOUNDS = 2,
        BUCKET_OUT_OF_BOUNDS = 3,
        NO_EPOCH_INIT = 4,
        EPOCH_EXISTS = 5,
        PAST_EVENT_NOT_ALLOWED = 6,
        QUEUE_EMPTY = 7,
        ADVANCE_IGNORED_EVENTS = 8,
        NUM_ERRORS = 9
    };

private:
    static const char* m_errorStrings[NUM_ERRORS];
    int m_code;
};


    }
}

#endif
