// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a spevectoral exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __VECTORTEST_H__
#define __VECTORTEST_H__


#include <cppunit/extensions/HelperMacros.h>



class VectorTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(VectorTest);
    CPPUNIT_TEST(test1D);
    CPPUNIT_TEST(test2D);
    CPPUNIT_TEST(test3D);
    CPPUNIT_TEST(test4D);
    CPPUNIT_TEST(test5D);
    CPPUNIT_TEST(test6D);
    CPPUNIT_TEST(test7D);
    CPPUNIT_TEST_SUITE_END();

public:
    void test1D();
    void test2D();
    void test3D();
    void test4D();
    void test5D();
    void test6D();
    void test7D();

};


#endif
