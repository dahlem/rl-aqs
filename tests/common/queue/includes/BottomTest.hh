#ifndef BOTTOMTEST_H
#define BOTTOMTEST_H


#include <cppunit/extensions/HelperMacros.h>

#include "Bottom.hh"


class BottomTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(BottomTest);
    CPPUNIT_TEST(testEnqueue);
    CPPUNIT_TEST(testEnqueueNull);
    CPPUNIT_TEST(testEnqueueOnce);
    CPPUNIT_TEST(testEnqueueTwiceOrdered);
    CPPUNIT_TEST(testEnqueueAtTail);
    CPPUNIT_TEST(testEnqueueAtHead);
    CPPUNIT_TEST(testStability);
    CPPUNIT_TEST(testEnlistShort);
    CPPUNIT_TEST(testEnlistShortStability);
    CPPUNIT_TEST(testEnlistLong);
    CPPUNIT_TEST(testEnlistLongStability);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testEnqueue();
    void testEnqueueNull();
    void testEnqueueOnce();
    void testEnqueueTwiceOrdered();
    void testEnqueueAtTail();
    void testEnqueueAtHead();
    void testStability();
    void testEnlistShort();
    void testEnlistShortStability();
    void testEnlistLong();
    void testEnlistLongStability();

private:
    Bottom *m_bottom;
};


#endif
