// Copyright (C) 2007-2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#include <gsl/gsl_rng.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

#include "LHS.hh"
using des::sampling::LHS;

#include "SamplingException.hh"
using des::sampling::SamplingException;

#include "LHSTest.hh"



// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(LHSTest);



void LHSTest::testDimDontMatch()
{
    const gsl_rng_type *rng_type = gsl_rng_mt19937;
    gsl_rng *rng;
    gsl_vector *min, *max;
    gsl_matrix *sample;

    rng = gsl_rng_alloc(rng_type);
    min = gsl_vector_calloc(3);
    max = gsl_vector_calloc(4);
    
    CPPUNIT_ASSERT_THROW(LHS::sample(rng, min, max, 4, &sample), SamplingException);
    
    gsl_rng_free(rng);
    gsl_vector_free(min);
    gsl_vector_free(max);
}

void LHSTest::testSizeTooSmall()
{
    const gsl_rng_type *rng_type = gsl_rng_mt19937;
    gsl_rng *rng;
    gsl_vector *min, *max;
    gsl_matrix *sample;

    rng = gsl_rng_alloc(rng_type);
    min = gsl_vector_calloc(3);
    max = gsl_vector_calloc(3);
    
    CPPUNIT_ASSERT_THROW(LHS::sample(rng, min, max, 1, &sample), SamplingException);
    
    gsl_rng_free(rng);
    gsl_vector_free(min);
    gsl_vector_free(max);
}

void LHSTest::testSample()
{
    const gsl_rng_type *rng_type = gsl_rng_mt19937;
    gsl_rng *rng;
    gsl_vector *min, *max;
    gsl_matrix *sample;

    double segments[] = {0.4, 0.2
    };    

    rng = gsl_rng_alloc(rng_type);
    min = gsl_vector_calloc(2);
    max = gsl_vector_calloc(2);

    gsl_vector_set(min, 0, -2);
    gsl_vector_set(min, 1, -1);
    gsl_vector_set(max, 0, 2);
    gsl_vector_set(max, 1, 1);

    LHS::sample(rng, min, max, 10, &sample);

    // test whether in range
    for (int i = 0; i < sample->size1; ++i) {
        CPPUNIT_ASSERT(gsl_matrix_get(sample, i, 0) < 2);
        CPPUNIT_ASSERT(gsl_matrix_get(sample, i, 1) < 1);
        CPPUNIT_ASSERT(gsl_matrix_get(sample, i, 0) > -2);
        CPPUNIT_ASSERT(gsl_matrix_get(sample, i, 1) > -1);
    }

    // test whether all segments are covered.
    for (int i = 1; i <= 10; ++i) {
        double tmax1 = -2 + i * segments[0];
        double tmax2 = -1 + i * segments[1];
        double tmin1 = -2 + (i - 1) * segments[0];
        double tmin2 = -1 + (i - 1) * segments[1];
        bool isIn1 = false;
        bool isIn2 = false;
        
        for (int k = 0; k < 10; ++k) {
            if ((gsl_matrix_get(sample, k, 0) < tmax1)
                && (gsl_matrix_get(sample, k, 0) > tmin1)) {
                if (isIn1) {
                    CPPUNIT_FAIL("No segment can be occupied twice");
                }
                isIn1 = true;
            }
            if ((gsl_matrix_get(sample, k, 1) < tmax2)
                && (gsl_matrix_get(sample, k, 1) > tmin2)) {
                if (isIn2) {
                    CPPUNIT_FAIL("No segment can be occupied twice");
                }
                isIn2 = true;
            }
        }

        CPPUNIT_ASSERT(isIn1);
        CPPUNIT_ASSERT(isIn2);
    }

    gsl_rng_free(rng);
    gsl_vector_free(min);
    gsl_vector_free(max);
    gsl_matrix_free(sample);
}
