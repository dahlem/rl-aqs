#ifndef SAMPLINGEXCEPTION_HH
#define SAMPLINGEXCEPTION_HH


#include <exception>

using std::exception;



class SamplingException: public exception
{
public:
    SamplingException(int p_code);

    const char* what() const throw();
    
    enum Errors {
        UNKNOWN_INDEX,
        NUM_ERRORS
    };

private:
    static const char* m_errorStrings[NUM_ERRORS];
    int m_code;
};

#endif
