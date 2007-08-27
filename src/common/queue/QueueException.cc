#include "QueueException.hh"


const char* QueueException::m_errorStrings[NUM_ERRORS] =
{
    "Reset Not Allowed, because the fifo data structure is not empty."
    "Could not find the rung to complete the enqueue operation."
};

QueueException::QueueException(int p_code)
    : exception(), m_code(p_code)
{}

const char* QueueException::what() const throw()
{
    return m_errorStrings[m_code];
}
