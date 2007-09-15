#include "QueueException.hh"


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
