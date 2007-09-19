// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef MERGESORTTEST_H
#define MERGESORTTEST_H


#include <cppunit/extensions/HelperMacros.h>

#include "Mergesort.hh"



class MergesortTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(MergesortTest);
    CPPUNIT_TEST(testStability);
    CPPUNIT_TEST(testReverse50);
    CPPUNIT_TEST(testReverse101);
    CPPUNIT_TEST(testMergeLeftSmaller);
    CPPUNIT_TEST(testMergeEqual);
    CPPUNIT_TEST(testMergeRightSmaller);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testStability();
    void testReverse50();
    void testReverse101();
    void testMergeLeftSmaller();
    void testMergeEqual();
    void testMergeRightSmaller();

private:
    void testReverse(int p_max);
    void testBothDirections(node_double_t *p_list, int p_size);
    void testMergeLists(int p_size1, int p_size2);
    
    node_double_t *HEAD;
    node_double_t *TAIL;

};


#endif
