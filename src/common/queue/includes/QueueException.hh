#ifndef QUEUEEXCEPTION_HH
#define QUEUEEXCEPTION_HH


#include <exception>

using std::exception;



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
        NUM_ERRORS
    };

private:
    static const char* m_errorStrings[NUM_ERRORS];
    int m_code;
};

#endif
