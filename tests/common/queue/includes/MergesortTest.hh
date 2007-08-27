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
};


#endif
