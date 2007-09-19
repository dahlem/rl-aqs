// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "Bottom.hh"
#include "Top.hh"
#include "Compare.hh"

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
    m_ladder->getNBucket(5, 0);
}

void LadderTest::testNumberEventsInBucketsBucketException1() throw (QueueException)
{
    m_ladder->getNBucket(1, 50);
}

void LadderTest::testNumberEventsInBucketsBucketException2() throw (QueueException)
{
    m_ladder->getNBucket(2, 50);
}

void LadderTest::testNumberEventsInBucketsBucketException3() throw (QueueException)
{
    m_ladder->getNBucket(0, 50);
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

void LadderTest::testEnlistSmall()
{
    Top *top = new Top();

    node_double_t *result = NULL;
    node_double_t *current = NULL;
    entry_t *entry = NULL;

    for (int i = 0; i < 3; ++i) {
        entry = new entry_t((double) i, i, 1, 0);
        top->enqueue(entry);
    }

    long size = top->getNTop();
    double max = top->getMaxTS();
    double min = top->getMinTS();
    node_double_t *list = top->delist();

    // start the first epoch
    m_ladder->enlist(list->next, size, max, min);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
        (double) ((max - min) / size),
        m_ladder->getBucketwidth(0),
        0.01);

    // first bucket
    int nBucket = 0;
    for (int i = 0; i < 4; ++i) {
        nBucket += m_ladder->getNBucket(0, i);
    }
    CPPUNIT_ASSERT(nBucket == 3);

    result = m_ladder->delist();
    current = result->next;
    CPPUNIT_ASSERT(current->data->arrival == 0.0);
    delete result;

    // second non-empty bucket
    result = m_ladder->delist();
    current = result->next;
    CPPUNIT_ASSERT(current->data->arrival == 1.0);
    delete result;

    // third non-empty bucket
    result = m_ladder->delist();
    current = result->next;
    CPPUNIT_ASSERT(current->data->arrival == 2.0);
    delete result;

    delete current->next;
    delete current;
    delete top;
}

void LadderTest::testEnlistLarge75()
{
    Top *top = new Top();

    node_double_t *result = NULL;
    node_double_t *current = NULL;
    entry_t *entry = NULL;

    for (int i = 0; i < 75; ++i) {
        entry = new entry_t((double) i, i, 1, 0);
        top->enqueue(entry);
    }

    long size = top->getNTop();
    double max = top->getMaxTS();
    double min = top->getMinTS();
    node_double_t *list = top->delist();

    // start the first epoch
    m_ladder->enlist(list->next, size, max, min);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
        (double) ((max - min) / size),
        m_ladder->getBucketwidth(0),
        0.01);

    int nBucket = 0;
    for (int i = 0; i < 76; ++i) {
        nBucket += m_ladder->getNBucket(0, i);
    }
    CPPUNIT_ASSERT(nBucket == 75);

    for (int i = 0; i < 75; ++i) {
        result = m_ladder->delist();
        current = result->next;

        CPPUNIT_ASSERT_DOUBLES_EQUAL(
            (double) i,
            current->data->arrival,
            0.00001);
        delete current->next;
        delete current;
        delete result;
    }

    delete top;
}

void LadderTest::testEnlistLarge75Fractions()
{
    Top *top = new Top();

    node_double_t *result = NULL;
    node_double_t *current = NULL;
    entry_t *entry = NULL;

    for (int i = 0; i < 10; ++i) {
        entry = new entry_t((double) i, i, 1, 0);
        top->enqueue(entry);
    }
    for (int i = 0; i < 25; ++i) {
        entry = new entry_t((double) (10.0 + ((double) i + 1) / (25.0 + 1.0)), i, 1, 0);
        top->enqueue(entry);
    }
    for (int i = 0; i < 40; ++i) {
        entry = new entry_t((double) i + 11.0, i, 1, 0);
        top->enqueue(entry);
    }

    long size = top->getNTop();
    double max = top->getMaxTS();
    double min = top->getMinTS();
    node_double_t *list = top->delist();

    // start the first epoch
    m_ladder->enlist(list->next, size, max, min);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
        (double) ((max - min) / size),
        m_ladder->getBucketwidth(0),
        0.01);

    int nBucket = 0;
    for (int i = 0; i < 76; ++i) {
        nBucket += m_ladder->getNBucket(0, i);
    }
    CPPUNIT_ASSERT(nBucket == 75);

    // individual buckets
    for (int i = 0; i < 10; ++i) {
        double rcur = m_ladder->getRCur(0);

        result = m_ladder->delist();
        current = result->next;

        CPPUNIT_ASSERT(Compare::diff(
                           current->data->arrival
                           + m_ladder->getBucketwidth(0), rcur
                           )
                       < m_ladder->getBucketwidth(0)
            );
        CPPUNIT_ASSERT_DOUBLES_EQUAL(
            (double) i,
            current->data->arrival,
            0.00001);
        delete current->next;
        delete current;
        delete result;
    }

    // many events in one bucket
    double rcur = m_ladder->getRCur(0);
    result = m_ladder->delist();
    node_double_t *temp = result;

    for (int i = 0; i < 17; ++i) {
        temp = temp->next;
        current = temp;

        CPPUNIT_ASSERT(current->data->arrival < rcur);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(
            (double) (10.0 + ((double) i + 1.0) / (25.0 + 1.0)),
            current->data->arrival,
            0.00001);
        delete current;
    }

    rcur = m_ladder->getRCur(0);
    result = m_ladder->delist();
    temp = result;

    for (int i = 0; i < 9; ++i) {
        temp = temp->next;
        current = temp;

        CPPUNIT_ASSERT(current->data->arrival < rcur);
    }

    for (int i = 0; i < 39; ++i) {
        rcur = m_ladder->getRCur(0);
        result = m_ladder->delist();
        current = result->next;

        CPPUNIT_ASSERT(current->data->arrival < rcur);
        delete current->next;
        delete current;
        delete result;
    }

    delete top;
}

void LadderTest::testSpawnOnce()
{
    Top *top = new Top();
    int actualEvents = 0;
    node_double_t *result = NULL;
    node_double_t *current = NULL;
    entry_t *entry = NULL;

    for (int i = 0; i < 1; ++i) {
        actualEvents++;
        entry = new entry_t((double) i, i, 1, 0);
        top->enqueue(entry);
    }
    for (int i = 0; i < 51; ++i) {
        actualEvents++;
        entry = new entry_t((1.0 + ((double) i + 1.0) / (51.0)), i, 1, 0);
        top->enqueue(entry);
    }
    for (int i = 0; i < 1; ++i) {
        actualEvents++;
        entry = new entry_t((double) i + 200.0, i, 1, 0);
        top->enqueue(entry);
    }

    long size = top->getNTop();
    double max = top->getMaxTS();
    double min = top->getMinTS();
    node_double_t *list = top->delist();

    // start the first epoch
    m_ladder->enlist(list->next, size, max, min);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
        (double) ((max - min) / size),
        m_ladder->getBucketwidth(0),
        0.01);

    double rcur = m_ladder->getRCur();
    long events = m_ladder->getNBucket();
    double bucketwidth = m_ladder->getBucketwidth();
    long totalEvents = events;

    result = m_ladder->delist();

    while (result->next->data != NULL) {
        for (int i = 0; i < events; ++i) {
            result = result->next;
            current = result;
            CPPUNIT_ASSERT(current->data->arrival < rcur);
            delete current;
        }

        events = m_ladder->getNBucket();
        totalEvents += events;
        rcur = m_ladder->getRCur();
        bucketwidth = m_ladder->getBucketwidth();
        result = m_ladder->delist();
    }
    CPPUNIT_ASSERT(totalEvents == actualEvents);


    delete top;
}

void LadderTest::testSpawnEqualArrivals()
{
    Top *top = new Top();
    int actualEvents = 0;
    node_double_t *result = NULL;
    node_double_t *current = NULL;
    entry_t *entry = NULL;

    for (int i = 0; i < 1; ++i) {
        actualEvents++;
        entry = new entry_t((double) i, i, 1, 0);
        top->enqueue(entry);
    }
    for (int i = 0; i < 51; ++i) {
        actualEvents++;
        entry = new entry_t(1.0, i, 1, 0);
        top->enqueue(entry);
    }
    for (int i = 0; i < 1; ++i) {
        actualEvents++;
        entry = new entry_t(2.0, i, 1, 0);
        top->enqueue(entry);
    }

    long size = top->getNTop();
    double max = top->getMaxTS();
    double min = top->getMinTS();
    node_double_t *list = top->delist();

    // start the first epoch
    m_ladder->enlist(list->next, size, max, min);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
        (double) ((max - min) / size),
        m_ladder->getBucketwidth(0),
        0.01);

    double rcur = m_ladder->getRCur();
    long events = m_ladder->getNBucket();
    double bucketwidth = m_ladder->getBucketwidth();
    long totalEvents = events;

    result = m_ladder->delist();

    while (result->next->data != NULL) {
        for (int i = 0; i < events; ++i) {
            result = result->next;
            current = result;
            CPPUNIT_ASSERT(current->data->arrival < rcur);
            delete current;
        }

        events = m_ladder->getNBucket();
        totalEvents += events;
        rcur = m_ladder->getRCur();
        bucketwidth = m_ladder->getBucketwidth();
        result = m_ladder->delist();
    }
    CPPUNIT_ASSERT(totalEvents == actualEvents);

    delete top;
}

void LadderTest::testSpawnAll()
{
    Top *top = new Top();
    Ladder *ladder = new Ladder(3);
    long expectedEvents = 0;
    node_double_t *result = NULL;
    node_double_t *current = NULL;
    entry_t *entry = NULL;
    double epsilon = 1/101;


    for (int i = 0; i < 100; ++i) {
        expectedEvents++;
        entry = new entry_t(1.0 + (double) i * epsilon, i, 1, 0);
        top->enqueue(entry);
    }
    for (int i = 100; i < 200; ++i) {
        expectedEvents++;
        entry = new entry_t(2.0 + (double) i * epsilon , i, 1, 0);
        top->enqueue(entry);
    }
    for (int i = 1; i < 3; ++i) {
        expectedEvents++;
        entry = new entry_t(100.0 * (double) i , i, 1, 0);
        top->enqueue(entry);
    }

    long size = top->getNTop();
    double max = top->getMaxTS();
    double min = top->getMinTS();
    node_double_t *list = top->delist();

    // start the first epoch
    ladder->enlist(list->next, size, max, min);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
        (double) ((max - min) / size),
        ladder->getBucketwidth(0),
        0.01);

    double rcur = ladder->getRCur();
    long events = ladder->getNBucket();
    double bucketwidth = ladder->getBucketwidth();
    long totalEvents = events;

    result = ladder->delist();

    while (result->next->data != NULL) {
        for (int i = 0; i < events; ++i) {
            result = result->next;
            current = result;
            CPPUNIT_ASSERT(current->data->arrival < rcur);
            delete current;
        }

        events = ladder->getNBucket();
        totalEvents += events;
        rcur = ladder->getRCur();
        bucketwidth = ladder->getBucketwidth();
        result = ladder->delist();
    }
    CPPUNIT_ASSERT_EQUAL(expectedEvents, totalEvents);


    delete top;
    delete ladder;
}

void LadderTest::testEnlistException() throw (QueueException)
{
    entry_t *newEntry1 = new entry_t(1.0, 1, 1, 0);
    entry_t *newEntry2 = new entry_t(2.0, 1, 1, 0);
    node_double_t *node1 = new node_double_t(newEntry1, NULL, NULL);
    node_double_t *node2 = new node_double_t(newEntry2, NULL, NULL);
    
    m_ladder->enlist(node1, 1, 1.0, 1.0);
    m_ladder->enlist(node2, 1, 2.0, 2.0);
}

void LadderTest::testEnqueueException() throw (QueueException)
{
    entry_t *newEntry1 = new entry_t(1.0, 1, 1, 0);
    m_ladder->enqueue(newEntry1);
}

void LadderTest::testEnqueue()
{
    entry_t *newEntry1 = new entry_t(1.0, 1, 1, 0);
    entry_t *newEntry2 = new entry_t(2.0, 1, 1, 0);
    node_double_t *node1 = new node_double_t(newEntry1, NULL, NULL);
    node_double_t *result = NULL;
    node_double_t *current = NULL;
    
    m_ladder->enlist(node1, 1, 1.0, 1.0);
    m_ladder->enqueue(newEntry2);

    double rcur = m_ladder->getRCur();
    long events = m_ladder->getNBucket();
    double bucketwidth = m_ladder->getBucketwidth();
    long totalEvents = events;

    result = m_ladder->delist();
    while (result->next->data != NULL) {
        for (int i = 0; i < events; ++i) {
            result = result->next;
            current = result;
            CPPUNIT_ASSERT(current->data->arrival < rcur);
            delete current;
        }

        events = m_ladder->getNBucket();
        totalEvents += events;
        rcur = m_ladder->getRCur();
        bucketwidth = m_ladder->getBucketwidth();
        result = m_ladder->delist();
    }
    CPPUNIT_ASSERT_EQUAL((long) 2, totalEvents);
}

void LadderTest::testEnqueueNotAllowed() throw (QueueException)
{
    entry_t *newEntry1 = new entry_t(1.0, 1, 1, 0);
    entry_t *newEntry2 = new entry_t(0.5, 1, 1, 0);
    node_double_t *node1 = new node_double_t(newEntry1, NULL, NULL);
    
    m_ladder->enlist(node1, 1, 1.0, 1.0);
    m_ladder->enqueue(newEntry2);
}

void LadderTest::testPushBack() 
{
    Bottom *bottom = new Bottom();

    for (int i = 0; i < 51; ++i) {
        entry_t *newEntry = new entry_t((double) i, 1, 1, 0);
        bottom->enqueue(newEntry);
    }

    CPPUNIT_ASSERT(bottom->size() > m_ladder->getThres());
    CPPUNIT_ASSERT(m_ladder->getNBucket() == 0);
    
    double max = bottom->getMaxTS();
    double min = bottom->getMinTS();
    long size = bottom->size();

    node_double_t *list = bottom->delist();
    m_ladder->enlist(list->next, size, max, min);

    double rcur = m_ladder->getRCur();
    long events = m_ladder->getNBucket();
    double bucketwidth = m_ladder->getBucketwidth();
    long totalEvents = events;

    node_double_t *result = m_ladder->delist();
    node_double_t *current = NULL;

    while (result->next->data != NULL) {
        for (int i = 0; i < events; ++i) {
            result = result->next;
            current = result;
            CPPUNIT_ASSERT(current->data->arrival < rcur);
            delete current;
        }

        events = m_ladder->getNBucket();
        totalEvents += events;
        rcur = m_ladder->getRCur();
        bucketwidth = m_ladder->getBucketwidth();
        result = m_ladder->delist();
    }

    CPPUNIT_ASSERT_EQUAL((long) 51, totalEvents);

    delete bottom;
}

void LadderTest::testPushBackSpawn() 
{
    Top *top = new Top();
    Bottom *bottom = new Bottom();
    entry_t *entry = NULL;

    for (int i = 0; i < 10; ++i) {
        entry = new entry_t((double) i, i, 1, 0);
        top->enqueue(entry);
    }

    long size = top->getNTop();
    double max = top->getMaxTS();
    double min = top->getMinTS();
    node_double_t *list = top->delist();

    // start the first epoch
    m_ladder->enlist(list->next, size, max, min);    
    
    for (int i = 0; i < 51; ++i) {
        entry_t *newEntry = new entry_t((double) i * 0.9/75, 1, 1, 0);
        bottom->enqueue(newEntry);
    }

    CPPUNIT_ASSERT(bottom->size() > m_ladder->getThres());
    CPPUNIT_ASSERT(m_ladder->getNBucket() != 0);
    
    bool success = m_ladder->spawn(false);

    CPPUNIT_ASSERT(success);
    size = bottom->size();
    list = bottom->delist();
    
    m_ladder->pushBack(list->next, size);

    double rcur = m_ladder->getRCur();
    long events = m_ladder->getNBucket();
    double bucketwidth = m_ladder->getBucketwidth();
    long totalEvents = events;

    node_double_t *result = m_ladder->delist();
    node_double_t *current = NULL;

    while (result->next->data != NULL) {
        for (int i = 0; i < events; ++i) {
            result = result->next;
            current = result;
            CPPUNIT_ASSERT(current->data->arrival < rcur);
            delete current;
        }

        events = m_ladder->getNBucket();
        totalEvents += events;
        rcur = m_ladder->getRCur();
        bucketwidth = m_ladder->getBucketwidth();
        result = m_ladder->delist();
    }

    CPPUNIT_ASSERT_EQUAL((long) 61, totalEvents);

    delete bottom;
    delete top;
}
