// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef LADDERTEST_HH
#define LADDERTEST_HH


#include <cppunit/extensions/HelperMacros.h>

#include "Ladder.hh"



class LadderTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(LadderTest);
    CPPUNIT_TEST_EXCEPTION(testBucketwidthException, QueueException);
    CPPUNIT_TEST(testBucketwidth);
    CPPUNIT_TEST_EXCEPTION(testNumberEventsInBucketsRungException, QueueException);
    CPPUNIT_TEST_EXCEPTION(testNumberEventsInBucketsBucketException1, QueueException);
    CPPUNIT_TEST_EXCEPTION(testNumberEventsInBucketsBucketException2, QueueException);
    CPPUNIT_TEST_EXCEPTION(testNumberEventsInBucketsBucketException3, QueueException);
    CPPUNIT_TEST(testNumberEventsInBucketsEmpty);
    CPPUNIT_TEST(testNRung);
    CPPUNIT_TEST(testThres);
    CPPUNIT_TEST_EXCEPTION(testRCurException, QueueException);
    CPPUNIT_TEST(testRCur);
    CPPUNIT_TEST_EXCEPTION(testRStartException, QueueException);
    CPPUNIT_TEST(testRStart);
    CPPUNIT_TEST(testNBC);
    CPPUNIT_TEST(testEnlistSmall);
    CPPUNIT_TEST(testEnlistLarge75);
    CPPUNIT_TEST(testEnlistLarge75Fractions);
    CPPUNIT_TEST(testSpawnOnce);
    CPPUNIT_TEST(testSpawnEqualArrivals);
    CPPUNIT_TEST(testSpawnAll);
    CPPUNIT_TEST_EXCEPTION(testEnlistException, QueueException);
    CPPUNIT_TEST_EXCEPTION(testEnqueueException, QueueException);
    CPPUNIT_TEST(testEnqueue);
    CPPUNIT_TEST_EXCEPTION(testEnqueueNotAllowed, QueueException);
    CPPUNIT_TEST(testPushBack);
    CPPUNIT_TEST(testPushBackSpawn);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testBucketwidthException() throw (QueueException);
    void testBucketwidth();
    void testNumberEventsInBucketsRungException() throw (QueueException);
    void testNumberEventsInBucketsBucketException1() throw (QueueException);
    void testNumberEventsInBucketsBucketException2() throw (QueueException);
    void testNumberEventsInBucketsBucketException3() throw (QueueException);
    void testNumberEventsInBucketsEmpty();
    void testNRung();
    void testThres();
    void testRCurException() throw (QueueException);
    void testRCur();
    void testRStartException() throw (QueueException);
    void testRStart();
    void testNBC();
    void testEnlistSmall();
    void testEnlistLarge75();
    void testEnlistLarge75Fractions();
    void testSpawnOnce();
    void testSpawnEqualArrivals();
    void testSpawnAll();
    void testEnlistException() throw (QueueException);
    void testEnqueueException() throw (QueueException);
    void testEnqueue();
    void testEnqueueNotAllowed() throw (QueueException);
    void testPushBack();
    void testPushBackSpawn();

private:
    Ladder *m_ladder;
};


#endif
