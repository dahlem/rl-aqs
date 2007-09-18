#include "SamplingException.hh"


const char* SamplingException::m_errorStrings[NUM_ERRORS] =
{
    "The random number generator index does not exist."
};

SamplingException::SamplingException(int p_code)
    : exception(), m_code(p_code)
{}

const char* SamplingException::what() const throw()
{
    return m_errorStrings[m_code];
}
