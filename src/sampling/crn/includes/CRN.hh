#ifndef CRN_HH
#define CRN_HH


#include <gsl/gsl_rng.h>
#include <vector>
using std::vector;

#include "SamplingException.hh"
#include "Singleton.hh"


typedef vector<gsl_rng*>  ptr_rng;



class CRN
{
public:
    CRN();
    ~CRN();

    const signed int init(unsigned long int p_seed);
    gsl_rng* get(const unsigned int p_rng) throw (SamplingException);


private:
    ptr_rng m_gslRngs;

};

typedef Singleton<CRN> CRNSingleton;


#endif
