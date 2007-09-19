// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

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
