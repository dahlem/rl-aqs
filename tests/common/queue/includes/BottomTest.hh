// Copyright (C) 2007-2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef BOTTOMTEST_H
#define BOTTOMTEST_H


#include <cppunit/extensions/HelperMacros.h>

#include "Bottom.hh"
namespace dcommon = des::common;


class BottomTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(BottomTest);
    CPPUNIT_TEST(testEnqueue);
    CPPUNIT_TEST(testEnqueueOnce);
    CPPUNIT_TEST(testEnqueueTwiceOrdered);
    CPPUNIT_TEST(testEnqueueAtTail);
    CPPUNIT_TEST(testEnqueueAtHead);
    CPPUNIT_TEST(testStability);
    CPPUNIT_TEST(testEnlistShort);
//    CPPUNIT_TEST(testEnlistShortStability);
    CPPUNIT_TEST(testEnlistLong);
//     CPPUNIT_TEST(testEnlistLongStability);
    CPPUNIT_TEST(testDelist);
    CPPUNIT_TEST(testMaxTS);
    CPPUNIT_TEST(testMinTS);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testEnqueue();
    void testEnqueueOnce();
    void testEnqueueTwiceOrdered();
    void testEnqueueAtTail();
    void testEnqueueAtHead();
    void testStability();
    void testEnlistShort();
//    void testEnlistShortStability();
    void testEnlistLong();
//     void testEnlistLongStability();
    void testDelist();
    void testMaxTS();
    void testMinTS();

private:
    dcommon::Bottom *m_bottom;
};


#endif
