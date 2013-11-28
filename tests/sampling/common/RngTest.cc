// Copyright (C) 2007-2008 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_rng.h>

#include "Rng.hh"
using des::sampling::Rng;

#include "RngTest.hh"


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(RngTest);


void RngTest::testUniform()
{
    const gsl_rng_type *rng_type = gsl_rng_mt19937;
    gsl_rng *rng;
    gsl_matrix *sample;

    rng = gsl_rng_alloc(rng_type);
    sample = gsl_matrix_calloc(5, 2);

    Rng::uniform(rng, sample);

    for (int i = 0; i < sample->size1; ++i) {
        for (int j = 0; j < sample->size2; ++j) {
            CPPUNIT_ASSERT(gsl_matrix_get(sample, i, j) > 0);
            CPPUNIT_ASSERT(gsl_matrix_get(sample, i, j) < 1);
        }
    }
    
    gsl_rng_free(rng);
    gsl_matrix_free(sample);
}
