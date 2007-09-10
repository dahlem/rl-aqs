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
    m_ladder->getBucketwidth(1);
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
