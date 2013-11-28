// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#include <cmath>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

#include "SCF.hh"
using des::statistics::SCF;

#include "StatsException.hh"
using des::statistics::StatsException;

#include "ScfTest.hh"



// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(ScfTest);



void ScfTest::testSCFDim()
{
    gsl_vector *a, *b, *theta;
    
    a = gsl_vector_calloc(3);
    b = gsl_vector_calloc(4);
    theta = gsl_vector_calloc(2);
    
    CPPUNIT_ASSERT_THROW(SCF::scf_func(a, b, theta, 1.0, 0.0), StatsException);

    gsl_vector_free(a);
    gsl_vector_free(b);
    gsl_vector_free(theta);
}

void ScfTest::testSCFThetaPos()
{
    gsl_vector *a, *b, *theta;
    
    a = gsl_vector_calloc(3);
    b = gsl_vector_calloc(3);
    theta = gsl_vector_calloc(3);
    
    CPPUNIT_ASSERT_THROW(SCF::scf_func(a, b, theta, 1.0, 0.0), StatsException);

    gsl_vector_free(a);
    gsl_vector_free(b);
    gsl_vector_free(theta);
}

void ScfTest::testSCF1()
{
    gsl_vector *a, *b, *theta;
    
    a = gsl_vector_calloc(1);
    b = gsl_vector_calloc(1);
    theta = gsl_vector_calloc(1);

    gsl_vector_set(a, 0, 1);
    gsl_vector_set(b, 0, 1);
    gsl_vector_set(theta, 0, 1);
    
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, SCF::scf_func(a, b, theta, 2, 0.0), 0.001);

    gsl_vector_free(a);
    gsl_vector_free(b);
    gsl_vector_free(theta);
}

void ScfTest::testSCF2()
{
    gsl_vector *a, *b, *theta;
    
    a = gsl_vector_calloc(1);
    b = gsl_vector_calloc(1);
    theta = gsl_vector_calloc(1);

    gsl_vector_set(a, 0, 1);
    gsl_vector_set(b, 0, 0);
    gsl_vector_set(theta, 0, 1);
    
    CPPUNIT_ASSERT_DOUBLES_EQUAL(exp(-1), SCF::scf_func(a, b, theta, 2, 0.0), 0.001);

    gsl_vector_free(a);
    gsl_vector_free(b);
    gsl_vector_free(theta);
}

void ScfTest::testSCF3()
{
    gsl_vector *a, *b, *theta;
    
    a = gsl_vector_calloc(1);
    b = gsl_vector_calloc(1);
    theta = gsl_vector_calloc(1);

    gsl_vector_set(a, 0, 1);
    gsl_vector_set(b, 0, 0);
    gsl_vector_set(theta, 0, 1);
    
    CPPUNIT_ASSERT_DOUBLES_EQUAL(exp(-1) + 1, SCF::scf_func(a, b, theta, 2, 1.0), 0.001);

    gsl_vector_free(a);
    gsl_vector_free(b);
    gsl_vector_free(theta);
}

void ScfTest::testSCFMDim1()
{
    gsl_vector *theta;
    gsl_matrix *M, *R;
    
    theta = gsl_vector_calloc(1);

    M = gsl_matrix_calloc(2, 2);
    R = gsl_matrix_calloc(2, 2);

    CPPUNIT_ASSERT_THROW(SCF::scf(M, R, theta, 1.0, 0.0), StatsException);

    gsl_vector_free(theta);

    gsl_matrix_free(M);
    gsl_matrix_free(R);
}

void ScfTest::testSCFMDim2()
{
    gsl_vector *theta;
    gsl_matrix *M, *R;
    
    theta = gsl_vector_calloc(1);
    gsl_vector_set(theta, 0, 1);

    M = gsl_matrix_calloc(1, 2);
    R = gsl_matrix_calloc(2, 2);

    CPPUNIT_ASSERT_THROW(SCF::scf(M, R, theta, 1.0, 0.0), StatsException);

    gsl_vector_free(theta);

    gsl_matrix_free(M);
    gsl_matrix_free(R);
}

void ScfTest::testSCFMThetaPos()
{
    gsl_vector *theta;
    gsl_matrix *M, *R;
    
    theta = gsl_vector_calloc(2);

    M = gsl_matrix_calloc(2, 2);
    R = gsl_matrix_calloc(2, 2);
    
    CPPUNIT_ASSERT_THROW(SCF::scf(M, R, theta, 1.0, 0.0), StatsException);

    gsl_vector_free(theta);

    gsl_matrix_free(M);
    gsl_matrix_free(R);
}

void ScfTest::testSCFMSquare()
{
    gsl_vector *theta;
    gsl_matrix *M, *R;
    
    theta = gsl_vector_calloc(1);
    gsl_vector_set(theta, 0, 1);

    M = gsl_matrix_calloc(2, 1);
    R = gsl_matrix_calloc(2, 1);
    
    CPPUNIT_ASSERT_THROW(SCF::scf(M, R, theta, 1.0, 0.0), StatsException);

    gsl_vector_free(theta);

    gsl_matrix_free(M);
    gsl_matrix_free(R);
}

void ScfTest::testSCFMTooSmall()
{
    gsl_vector *theta;
    gsl_matrix *M, *R;
    
    theta = gsl_vector_calloc(1);
    gsl_vector_set(theta, 0, 1);

    M = gsl_matrix_calloc(1, 1);
    R = gsl_matrix_calloc(1, 1);
    
    CPPUNIT_ASSERT_THROW(SCF::scf(M, R, theta, 1.0, 0.0), StatsException);

    gsl_vector_free(theta);

    gsl_matrix_free(M);
    gsl_matrix_free(R);
}

void ScfTest::testSCFM1()
{
    gsl_vector *theta;
    gsl_matrix *M, *R;
    
    theta = gsl_vector_calloc(2);
    gsl_vector_set(theta, 0, 1);
    gsl_vector_set(theta, 1, 1);

    M = gsl_matrix_calloc(3, 2);
    R = gsl_matrix_calloc(3, 3);

    SCF::scf(M, R, theta, 1.0, 1.0);

    for (int i = 0; i < R->size1; ++i) {
        for (int j = 0; j < R->size2; ++j) {
            if (i == j) {
                CPPUNIT_ASSERT_DOUBLES_EQUAL(2, gsl_matrix_get(R, i, j), 0.01);
            } else {
                CPPUNIT_ASSERT_DOUBLES_EQUAL(1, gsl_matrix_get(R, i, j), 0.01);
            }
        }
    }
    
    gsl_vector_free(theta);

    gsl_matrix_free(M);
    gsl_matrix_free(R);
}

void ScfTest::testSCFM2()
{
    gsl_vector *theta;
    gsl_matrix *M, *R;
    
    theta = gsl_vector_calloc(2);
    gsl_vector_set(theta, 0, 1);
    gsl_vector_set(theta, 1, 1);

    M = gsl_matrix_calloc(2, 2);
    R = gsl_matrix_calloc(2, 2);
    gsl_matrix_set(M, 0, 0, 1);
    gsl_matrix_set(M, 0, 1, 0);
    gsl_matrix_set(M, 1, 0, 0);
    gsl_matrix_set(M, 1, 1, 1);

    SCF::scf(M, R, theta, 1.0, 1.0);

    for (int i = 0; i < R->size1; ++i) {
        for (int j = 0; j < R->size2; ++j) {
            if (i == j) {
                CPPUNIT_ASSERT_DOUBLES_EQUAL(2, gsl_matrix_get(R, i, j), 0.01);
            } else {
                CPPUNIT_ASSERT_DOUBLES_EQUAL(1, gsl_matrix_get(R, i, j), 0.01);
            }
        }
    }
    
    gsl_vector_free(theta);

    gsl_matrix_free(M);
    gsl_matrix_free(R);
}
