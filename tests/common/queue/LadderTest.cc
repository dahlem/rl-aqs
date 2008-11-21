// Copyright (C) 2007-2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "Compare.hh"
using des::utils::Compare;

#include "Bottom.hh"
#include "Entry.hh"
#include "Top.hh"
namespace dcommon = des::common;

#include "QueueException.hh"
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

void LadderTest::testBucketwidthException() throw (dcommon::QueueException)
{
    m_ladder->getBucketwidth(6);
}

void LadderTest::testBucketwidth()
{
    for (int i = 0; i < 5; ++i) {
        CPPUNIT_ASSERT(m_ladder->getBucketwidth(i) == 0.0);
    }
}

void LadderTest::testNumberEventsInBucketsRungException() throw (dcommon::QueueException)
{
    m_ladder->getNBucket(5, 0);
}

void LadderTest::testNumberEventsInBucketsBucketException1() throw (dcommon::QueueException)
{
    m_ladder->getNBucket(1, 50);
}

void LadderTest::testNumberEventsInBucketsBucketException2() throw (dcommon::QueueException)
{
    m_ladder->getNBucket(2, 50);
}

void LadderTest::testNumberEventsInBucketsBucketException3() throw (dcommon::QueueException)
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

void LadderTest::testRCurException() throw (dcommon::QueueException)
{
    m_ladder->getRCur(6);
}

void LadderTest::testRCur()
{
    for (int i = 0; i < 5; ++i) {
        CPPUNIT_ASSERT(m_ladder->getRCur(i) == 0.0);
    }
}

void LadderTest::testRStartException() throw (dcommon::QueueException)
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
    dcommon::Top *top = new dcommon::Top();
    dcommon::EntryList *result = NULL;
    dcommon::Entry *entry = NULL;

    for (int i = 0; i < 3; ++i) {
        entry = new dcommon::Entry(0.0, (double) i, i, 1, 0);
        top->push(entry);
    }

    boost::int32_t size = top->getNTop();
    double max = top->getMaxTS();
    double min = top->getMinTS();
    dcommon::EntryList *list = top->delist();

    // start the first epoch
    m_ladder->push(list, max, min);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
        (double) ((max - min) / size),
        m_ladder->getBucketwidth(0),
        0.01);

    // first bucket
    int nBucket = 0;
    for (int i = 0; i < 4; ++i) {
        nBucket += m_ladder->getNBucket(0, i);
    }
    CPPUNIT_ASSERT_EQUAL(3, nBucket);

    result = m_ladder->delist();
    CPPUNIT_ASSERT_EQUAL((unsigned int) 1, result->size());

    entry = reinterpret_cast<dcommon::Entry*>(&result->front());
    result->pop_front();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, entry->getArrival(), 0.00001);
    delete entry;
    CPPUNIT_ASSERT_EQUAL((unsigned int) 0, result->size());

    result = m_ladder->delist();
    CPPUNIT_ASSERT_EQUAL((unsigned int) 1, result->size());

    entry = reinterpret_cast<dcommon::Entry*>(&result->front());
    result->pop_front();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, entry->getArrival(), 0.00001);
    delete entry;
    CPPUNIT_ASSERT_EQUAL((unsigned int) 0, result->size());

    result = m_ladder->delist();
    CPPUNIT_ASSERT_EQUAL((unsigned int) 1, result->size());

    entry = reinterpret_cast<dcommon::Entry*>(&result->front());
    result->pop_front();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, entry->getArrival(), 0.00001);
    delete entry;
    CPPUNIT_ASSERT_EQUAL((unsigned int) 0, result->size());

    delete top;
}

void LadderTest::testEnlistLarge75()
{
    dcommon::Top *top = new dcommon::Top();
    dcommon::EntryList *result = NULL;
    dcommon::Entry *entry = NULL;

    for (int i = 0; i < 75; ++i) {
        entry = new dcommon::Entry(0.0, (double) i, i, 1, 0);
        top->push(entry);
    }

    boost::int32_t size = top->getNTop();
    double max = top->getMaxTS();
    double min = top->getMinTS();
    dcommon::EntryList *list = top->delist();

    // start the first epoch
    m_ladder->push(list, max, min);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
        (double) ((max - min) / size),
        m_ladder->getBucketwidth(0),
        0.01);

    // first bucket
    int nBucket = 0;
    for (int i = 0; i < 76; ++i) {
        nBucket += m_ladder->getNBucket(0, i);
    }
    CPPUNIT_ASSERT_EQUAL(75, nBucket);

    for (int i = 0; i < 75; ++i) {
        result = m_ladder->delist();
        CPPUNIT_ASSERT_EQUAL((unsigned int) 1, result->size());

        entry = reinterpret_cast<dcommon::Entry*>(&result->front());
        result->pop_front();
        CPPUNIT_ASSERT_DOUBLES_EQUAL((double) i, entry->getArrival(), 0.00001);
        delete entry;
        CPPUNIT_ASSERT_EQUAL((unsigned int) 0, result->size());
    }

    delete top;
}

void LadderTest::testEnlistLarge75Fractions()
{
    dcommon::Top *top = new dcommon::Top();
    dcommon::EntryList *result = NULL;
    dcommon::Entry *entry = NULL;

    for (int i = 0; i < 10; ++i) {
        entry = new dcommon::Entry(0.0, (double) i, i, 1, 0);
        top->push(entry);
    }
    for (int i = 0; i < 25; ++i) {
        entry = new dcommon::Entry(0.0, (double) (10.0 + ((double) i + 1) / (25.0 + 1.0)), i, 1, 0);
        top->push(entry);
    }
    for (int i = 0; i < 40; ++i) {
        entry = new dcommon::Entry(0.0, (double) i + 11.0, i, 1, 0);
        top->push(entry);
    }

    long size = top->getNTop();
    double max = top->getMaxTS();
    double min = top->getMinTS();
    result = top->delist();

    // start the first epoch
    m_ladder->push(result, max, min);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
        (double) ((max - min) / size),
        m_ladder->getBucketwidth(0),
        0.01);

    int nBucket = 0;
    for (int i = 0; i < 76; ++i) {
        nBucket += m_ladder->getNBucket(0, i);
    }
    CPPUNIT_ASSERT_EQUAL(75, nBucket);

    // individual buckets
    for (int i = 0; i < 10; ++i) {
        result = m_ladder->delist();
        double rcur = m_ladder->getRCur(0);
        CPPUNIT_ASSERT(result->size() > 0);

        entry = reinterpret_cast<dcommon::Entry*>(&result->front());
        result->pop_front();

        CPPUNIT_ASSERT(Compare::diff(
                           entry->getArrival() + m_ladder->getBucketwidth(0),
                           rcur)
                       < m_ladder->getBucketwidth(0)
            );

        CPPUNIT_ASSERT_DOUBLES_EQUAL((double) i, entry->getArrival(), 0.00001);
        delete entry;
    }


    // many events in one bucket
    result = m_ladder->delist();
    double rcur = m_ladder->getRCur(0);
    CPPUNIT_ASSERT(result->size() > 0);

    for (int i = 0; i < 17; ++i) {
        entry = reinterpret_cast<dcommon::Entry*>(&result->front());
        result->pop_front();

        CPPUNIT_ASSERT(entry->getArrival() < rcur);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(
            (double) (10.0 + ((double) i + 1.0) / (25.0 + 1.0)),
            entry->getArrival(),
            0.00001);
        delete entry;
    }

    result = m_ladder->delist();
    rcur = m_ladder->getRCur(0);
    CPPUNIT_ASSERT(result->size() > 0);

    for (int i = 0; i < 9; ++i) {
        entry = reinterpret_cast<dcommon::Entry*>(&result->front());
        result->pop_front();

        CPPUNIT_ASSERT(entry->getArrival() < rcur);
        delete entry;
    }

    for (int i = 0; i < 39; ++i) {
        result = m_ladder->delist();
        rcur = m_ladder->getRCur(0);
        CPPUNIT_ASSERT(result->size() > 0);
        entry = reinterpret_cast<dcommon::Entry*>(&result->front());
        result->pop_front();

        CPPUNIT_ASSERT(entry->getArrival() < rcur);
        delete entry;
    }

    delete top;
}

void LadderTest::testSpawnOnce()
{
    dcommon::Top *top = new dcommon::Top();
    dcommon::EntryList *result = NULL;
    dcommon::Entry *entry = NULL;
    int actualEvents = 0;

    for (int i = 0; i < 1; ++i) {
        actualEvents++;
        entry = new dcommon::Entry(0.0, (double) i, i, 1, 0);
        top->push(entry);
    }
    for (int i = 0; i < 51; ++i) {
        actualEvents++;
        entry = new dcommon::Entry(0.0, (1.0 + ((double) i + 1.0) / (51.0)), i, 1, 0);
        top->push(entry);
    }
    for (int i = 0; i < 1; ++i) {
        actualEvents++;
        entry = new dcommon::Entry(0.0, (double) i + 200.00, i, 1, 0);
        top->push(entry);
    }

    long size = top->getNTop();
    double max = top->getMaxTS();
    double min = top->getMinTS();
    result = top->delist();

    // start the first epoch
    m_ladder->push(result, max, min);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
        (double) ((max - min) / size),
        m_ladder->getBucketwidth(0),
        0.01);

    double bucketwidth = m_ladder->getBucketwidth();

    result = m_ladder->delist();
    double rcur = m_ladder->getRCur();
    long events = m_ladder->getNBucket();
    CPPUNIT_ASSERT(events < actualEvents);

    long totalEvents = events;

    while (!result->empty()) {
        for (int i = 0; i < events; ++i) {
            entry = reinterpret_cast<dcommon::Entry*>(&result->front());
            result->pop_front();
            CPPUNIT_ASSERT(entry->getArrival() < rcur);
            delete entry;
        }

        result = m_ladder->delist();
        events = m_ladder->getNBucket();
        totalEvents += events;
        rcur = m_ladder->getRCur();
        bucketwidth = m_ladder->getBucketwidth();
    }

    CPPUNIT_ASSERT(totalEvents == actualEvents);

    delete top;
}

void LadderTest::testSpawnEqualArrivals()
{
    dcommon::Top *top = new dcommon::Top();
    dcommon::EntryList *result = NULL;
    dcommon::Entry *entry = NULL;
    int actualEvents = 0;

    for (int i = 0; i < 1; ++i) {
        actualEvents++;
        entry = new dcommon::Entry(0.0, (double) i, i, 1, 0);
        top->push(entry);
    }
    for (int i = 0; i < 51; ++i) {
        actualEvents++;
        entry = new dcommon::Entry(0.0, 1.0, i, 1, 0);
        top->push(entry);
    }
    for (int i = 0; i < 1; ++i) {
        actualEvents++;
        entry = new dcommon::Entry(0.0, 2.0, i, 1, 0);
        top->push(entry);
    }

    long size = top->getNTop();
    double max = top->getMaxTS();
    double min = top->getMinTS();
    result = top->delist();

    // start the first epoch
    m_ladder->push(result, max, min);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
        (double) ((max - min) / size),
        m_ladder->getBucketwidth(0),
        0.01);

    result = m_ladder->delist();
    double rcur = m_ladder->getRCur();
    long events = m_ladder->getNBucket();
    double bucketwidth = m_ladder->getBucketwidth();
    long totalEvents = events;

    while (!result->empty()) {
        for (int i = 0; i < events; ++i) {
            entry = reinterpret_cast<dcommon::Entry*>(&result->front());
            result->pop_front();
            CPPUNIT_ASSERT(entry->getArrival() < rcur);
            delete entry;
        }

        result = m_ladder->delist();
        events = m_ladder->getNBucket();
        totalEvents += events;
        rcur = m_ladder->getRCur();
        bucketwidth = m_ladder->getBucketwidth();
    }
    CPPUNIT_ASSERT(totalEvents == actualEvents);

    delete top;
}

void LadderTest::testSpawnAll()
{
    dcommon::Top *top = new dcommon::Top();
    dcommon::EntryList *result = NULL;
    dcommon::Entry *entry = NULL;
    Ladder *ladder = new Ladder(3);
    long expectedEvents = 0;
    double epsilon = 1/101;


    for (int i = 0; i < 100; ++i) {
        expectedEvents++;
        entry = new dcommon::Entry(0.0, 1.0 + (double) i * epsilon, i, 1, 0);
        top->push(entry);
    }
    for (int i = 100; i < 200; ++i) {
        expectedEvents++;
        entry = new dcommon::Entry(0.0, 2.0 + (double) i * epsilon, i, 1, 0);
        top->push(entry);
    }
    for (int i = 1; i < 3; ++i) {
        expectedEvents++;
        entry = new dcommon::Entry(0.0, 100.0 * (double) i, i, 1, 0);
        top->push(entry);
    }

    long size = top->getNTop();
    double max = top->getMaxTS();
    double min = top->getMinTS();
    result = top->delist();

    // start the first epoch
    ladder->push(result, max, min);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
        (double) ((max - min) / size),
        ladder->getBucketwidth(0),
        0.01);

    result = ladder->delist();
    double rcur = ladder->getRCur();
    long events = ladder->getNBucket();
    double bucketwidth = ladder->getBucketwidth();
    long totalEvents = events;

    while (!result->empty()) {
        for (int i = 0; i < events; ++i) {
            entry = reinterpret_cast<dcommon::Entry*>(&result->front());
            result->pop_front();
            CPPUNIT_ASSERT(entry->getArrival() < rcur);
            delete entry;
        }

        result = ladder->delist();
        events = ladder->getNBucket();
        totalEvents += events;
        rcur = ladder->getRCur();
        bucketwidth = ladder->getBucketwidth();
    }
    CPPUNIT_ASSERT_EQUAL(expectedEvents, totalEvents);

    delete top;
    delete ladder;
}

void LadderTest::testEnlistException() throw (dcommon::QueueException)
{
    dcommon::Entry *newEntry1 = new dcommon::Entry(0.0, 1.0, 1, 1, 0);
    dcommon::Entry *newEntry2 = new dcommon::Entry(0.0, 2.0, 1, 1, 0);
    dcommon::Top *top1 = new dcommon::Top();
    dcommon::Top *top2 = new dcommon::Top();
    dcommon::EntryList *list1 = NULL;
    dcommon::EntryList *list2 = NULL;

    top1->push(newEntry1);
    top2->push(newEntry2);

    list1 = top1->delist();
    list2 = top2->delist();

    m_ladder->push(list1, 1.0, 1.0);
    m_ladder->push(list2, 2.0, 2.0);
}

void LadderTest::testEnqueueException() throw (dcommon::QueueException)
{
    dcommon::Entry *newEntry1 = new dcommon::Entry(0.0, 1.0, 1, 1, 0);
    m_ladder->push(newEntry1);
}

void LadderTest::testEnqueue()
{
    dcommon::Entry *newEntry1 = new dcommon::Entry(0.0, 1.0, 1, 1, 0);
    dcommon::Entry *newEntry2 = new dcommon::Entry(0.0, 2.0, 1, 1, 0);
    dcommon::Entry *entry = NULL;
    dcommon::Top *top1 = new dcommon::Top();
    dcommon::EntryList *list1 = NULL;
    dcommon::EntryList *result = NULL;

    top1->push(newEntry1);
    list1 = top1->delist();

    m_ladder->push(list1, 1.0, 1.0);
    m_ladder->push(newEntry2);

    result = m_ladder->delist();
    double rcur = m_ladder->getRCur();
    long events = m_ladder->getNBucket();
    double bucketwidth = m_ladder->getBucketwidth();
    long totalEvents = events;

    while (!result->empty()) {
        for (int i = 0; i < events; ++i) {
            entry = reinterpret_cast<dcommon::Entry*>(&result->front());
            result->pop_front();
            CPPUNIT_ASSERT(entry->getArrival() < rcur);
            delete entry;
        }

        result = m_ladder->delist();
        events = m_ladder->getNBucket();
        totalEvents += events;
        rcur = m_ladder->getRCur();
        bucketwidth = m_ladder->getBucketwidth();
    }

    CPPUNIT_ASSERT_EQUAL((long) 2, totalEvents);
}

void LadderTest::testEnqueueNotAllowed() throw (dcommon::QueueException)
{
    dcommon::Entry *newEntry1 = new dcommon::Entry(0.0, 1.0, 1, 1, 0);
    dcommon::Entry *newEntry2 = new dcommon::Entry(0.0, 0.5, 1, 1, 0);
    dcommon::Top *top1 = new dcommon::Top();
    dcommon::EntryList *list1 = NULL;

    top1->push(newEntry1);
    list1 = top1->delist();

    m_ladder->push(list1, 1.0, 1.0);
    m_ladder->push(newEntry2);
}

void LadderTest::testPushBack()
{
    dcommon::Top *top = new dcommon::Top();
    dcommon::Bottom *bottom = new dcommon::Bottom();
    dcommon::EntryList *result = NULL;
    dcommon::Entry *entry = NULL;
    int actualEvents = 0;

    for (int i = 0; i < 1; ++i) {
        actualEvents++;
        entry = new dcommon::Entry(0.0, (double) i, i, 1, 0);
        top->push(entry);
    }
    for (int i = 0; i < 51; ++i) {
        actualEvents++;
        entry = new dcommon::Entry(0.0, (1.0 + ((double) i + 1.0) / (51.0)), i, 1, 0);
        top->push(entry);
    }
    for (int i = 0; i < 1; ++i) {
        actualEvents++;
        entry = new dcommon::Entry(0.0, (double) i + 200.00, i, 1, 0);
        top->push(entry);
    }

    long size = top->getNTop();
    double max = top->getMaxTS();
    double min = top->getMinTS();
    result = top->delist();

    // start the first epoch
    m_ladder->push(result, max, min);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
        (double) ((max - min) / size),
        m_ladder->getBucketwidth(0),
        0.01);

    double bucketwidth = m_ladder->getBucketwidth();

    result = m_ladder->delist();
    double rcur = m_ladder->getRCur();
    long events = m_ladder->getNBucket();
    CPPUNIT_ASSERT(events < actualEvents);

    long totalEvents = events;

    for (int i = 0; i < events; ++i) {
        entry = reinterpret_cast<dcommon::Entry*>(&result->front());
        result->pop_front();
        CPPUNIT_ASSERT(entry->getArrival() < rcur);
        delete entry;
    }

    result = m_ladder->delist();
    events = m_ladder->getNBC();
    rcur = m_ladder->getRCur();
    bucketwidth = m_ladder->getBucketwidth();

    // simulate events coming from bottom
    bottom->push(result);

    try {
        result = bottom->list();
        m_ladder->pushBack(result);
    } catch (dcommon::QueueException &qe) {
        CPPUNIT_FAIL(qe.what());
    }

    result = m_ladder->delist();
    events = m_ladder->getNBucket();
    totalEvents += events;
    rcur = m_ladder->getRCur();

    std::cout << result->size() << ", " << events << std::endl;

    while (!result->empty()) {
        std::cout << "start while" << std::endl;
        for (int i = 0; i < events; ++i) {
            std::cout << "start for" << std::endl;
            entry = reinterpret_cast<dcommon::Entry*>(&result->front());
            result->pop_front();
            CPPUNIT_ASSERT(entry->getArrival() < rcur);
            delete entry;
            std::cout << "end for" << std::endl;
        }

        std::cout << "delist" << std::endl;
        result = m_ladder->delist();
        std::cout << "buckets" << std::endl;
        events = m_ladder->getNBucket();
        std::cout << result->size() << ", " << events << std::endl;
        totalEvents += events;
        std::cout << "tot events" << std::endl;
        rcur = m_ladder->getRCur();
        std::cout << "end while" << std::endl;
    }

    CPPUNIT_ASSERT(totalEvents == actualEvents);

    delete top;
    delete bottom;
}
