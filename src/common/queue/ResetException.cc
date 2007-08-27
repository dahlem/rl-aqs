#include "ResetException.hh"


const char* ResetException::what() const throw()
{
    return "Reset is only allowed on an empty fifo data structure!";
}
