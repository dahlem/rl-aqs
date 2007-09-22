// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef QUEUEEXCEPTION_HH
#define QUEUEEXCEPTION_HH


#include <exception>
using std::exception;



namespace des
{
    namespace common
    {

/**
 * This exception specifies errors that may occur within queues.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class QueueException: public exception
{
public:
    QueueException(int p_code);

    const char* what() const throw();

    enum Errors {
        BAD_RESET,
        RUNG_NOT_FOUND,
        RUNG_OUT_OF_BOUNDS,
        BUCKET_OUT_OF_BOUNDS,
        NO_EPOCH_INIT,
        EPOCH_EXISTS,
        PAST_EVENT_NOT_ALLOWED,
        NUM_ERRORS
    };

private:
    static const char* m_errorStrings[NUM_ERRORS];
    int m_code;
};


    }
}

#endif
