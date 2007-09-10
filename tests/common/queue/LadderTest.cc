#include "LadderTest.hh"


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(LadderTest);



void LadderTest::setUp()
{
    m_ladder = new Ladder();
}

void LadderTest::tearDown()
{
    delete m_ladder;
}

void LadderTest::testBucketwidthException() throw (QueueException)
{
    m_ladder->getBucketwidth(6);
}

void LadderTest::testBucketwidth()
{
    for (int i = 0; i < 5; ++i) {
        CPPUNIT_ASSERT(m_ladder->getBucketwidth(i) == 0.0);
    }
}

void LadderTest::testNumberEventsInBucketsRungException() throw (QueueException) 
{
    m_ladder->getNBucket(7, 0);
}

void LadderTest::testNumberEventsInBucketsBucketException1() throw (QueueException) 
{
    m_ladder->getNBucket(1, 51);
}

void LadderTest::testNumberEventsInBucketsBucketException2() throw (QueueException) 
{
    m_ladder->getNBucket(2, 51);
}

void LadderTest::testNumberEventsInBucketsBucketException3() throw (QueueException)
{
    m_ladder->getNBucket(0, 51);
}

void LadderTest::testNumberEventsInBucketsEmpty()
{
    CPPUNIT_ASSERT(m_ladder->getNBucket(0, 0) == 0);
    // test the first rung
    for (int i = 0; i < 50; ++i) {
        CPPUNIT_ASSERT(m_ladder->getNBucket(0, i) == 0);
    }

    // test the 2nd to 5th rung
    for (int i = 1; i < 5; ++i) {
        for (int j = 0; j < 50; ++j) {
            CPPUNIT_ASSERT(m_ladder->getNBucket(i, j) == 0);
        }
    }
}

void LadderTest::testNRung()
{
    CPPUNIT_ASSERT(m_ladder->getNRung() == 5);
}

void LadderTest::testThres()
{
    CPPUNIT_ASSERT(m_ladder->getThres() == 50);
}

void LadderTest::testRCurException() throw (QueueException)
{
    m_ladder->getRCur(6);
}

void LadderTest::testRCur()
{
    for (int i = 0; i < 5; ++i) {
        CPPUNIT_ASSERT(m_ladder->getRCur(i) == 0.0);
    }
}

void LadderTest::testRStartException() throw (QueueException)
{
    m_ladder->getRStart(6);
}

void LadderTest::testRStart()
{
    for (int i = 0; i < 5; ++i) {
        CPPUNIT_ASSERT(m_ladder->getRStart(i) == 0.0);
    }
}

void LadderTest::testNBC()
{
    CPPUNIT_ASSERT(m_ladder->getNBC() == 0);
}
