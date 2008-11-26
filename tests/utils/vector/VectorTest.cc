// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "VectorTest.hh"


#include "Vector.hh"
namespace dutils = des::utils;


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(VectorTest);



void VectorTest::test1D()
{
    double vec1[1] = {1};
    double vec2[1] = {2};
    double dotprod = dutils::Vector::dotproduct(1, vec1, vec2);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, dotprod, 0.00001);
}

void VectorTest::test2D()
{
    double vec1[2] = {1, 2};
    double vec2[2] = {2, 3};
    double dotprod = dutils::Vector::dotproduct(2, vec1, vec2);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(8.0, dotprod, 0.00001);
}

void VectorTest::test3D()
{
    double vec1[3] = {1, 2, 3};
    double vec2[3] = {2, 3, 4};
    double dotprod = dutils::Vector::dotproduct(3, vec1, vec2);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(20.0, dotprod, 0.00001);
}

void VectorTest::test4D()
{
    double vec1[4] = {1, 2, 3, 4};
    double vec2[4] = {2, 3, 4, 5};
    double dotprod = dutils::Vector::dotproduct(4, vec1, vec2);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(40.0, dotprod, 0.00001);
}

void VectorTest::test5D()
{
    double vec1[5] = {1, 2, 3, 4, 5};
    double vec2[5] = {2, 3, 4, 5, 6};
    double dotprod = dutils::Vector::dotproduct(5, vec1, vec2);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(70.0, dotprod, 0.00001);
}

void VectorTest::test6D()
{
    double vec1[6] = {1, 2, 3, 4, 5, 6};
    double vec2[6] = {2, 3, 4, 5, 6, 7};
    double dotprod = dutils::Vector::dotproduct(6, vec1, vec2);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(112.0, dotprod, 0.00001);
}

void VectorTest::test7D()
{
    double vec1[7] = {1, 2, 3, 4, 5, 6, 7};
    double vec2[7] = {2, 3, 4, 5, 6, 7, 8};
    double dotprod = dutils::Vector::dotproduct(7, vec1, vec2);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(168.0, dotprod, 0.00001);
}
