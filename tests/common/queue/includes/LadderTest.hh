#ifndef LADDERTEST_H
#define LADDERTEST_H


#include <cppunit/extensions/HelperMacros.h>

#include "Ladder.hh"


class LadderTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(LadderTest);
    CPPUNIT_TEST_EXCEPTION(testBucketwidthException, QueueException);
    CPPUNIT_TEST_EXCEPTION(testNumberEventsInBucketsRungException, QueueException);
    CPPUNIT_TEST_EXCEPTION(testNumberEventsInBucketsBucketException1, QueueException);
    CPPUNIT_TEST_EXCEPTION(testNumberEventsInBucketsBucketException2, QueueException);
    CPPUNIT_TEST(testNumberEventsInBucketsEmpty);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testBucketwidthException() throw (QueueException);
    void testNumberEventsInBucketsRungException() throw (QueueException);
    void testNumberEventsInBucketsBucketException1() throw (QueueException);
    void testNumberEventsInBucketsBucketException2() throw (QueueException);
    void testNumberEventsInBucketsEmpty();

private:
    Ladder *m_ladder;
};


#endif
