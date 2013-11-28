// Copyright (C) 2007-2008 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include <iostream>
using std::cout;
using std::endl;

#include "BottomTest.hh"
#include "Bottom.hh"
#include "Entry.hh"
namespace dcommon = des::common;



// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(BottomTest);


void BottomTest::setUp()
{
    m_bottom = new dcommon::Bottom();
}

void BottomTest::tearDown()
{
    delete m_bottom;
}

void BottomTest::testEnqueue()
{
    for (int i = 1; i < 6; ++i) {
        dcommon::Entry *entry = new dcommon::Entry(0.0, (double) i, 1, 1, 0);
        m_bottom->push(entry);
    }

    CPPUNIT_ASSERT(m_bottom->size() == 5);

    for (int i = 1; i < 6; ++i) {
        dcommon::Entry *entryPtr = m_bottom->front();

        m_bottom->pop_front();
        delete entryPtr;
    }
}

void BottomTest::testEnqueueOnce()
{
    dcommon::Entry *entry = new dcommon::Entry(0.0, 0.0, 1, 1, 0);
    dcommon::Entry *result = NULL;

    m_bottom->push(entry);

    CPPUNIT_ASSERT(m_bottom->size() == 1);

    try {
        result = m_bottom->front();
        m_bottom->pop_front();
    } catch (dcommon::QueueException q) {
        CPPUNIT_FAIL("Did not expect queue exception!");
    }

    CPPUNIT_ASSERT_EQUAL(entry->getArrival(), result->getArrival());
    CPPUNIT_ASSERT_EQUAL(entry->getDestination(), result->getDestination());
    CPPUNIT_ASSERT_EQUAL(entry->getOrigin(), result->getOrigin());
    CPPUNIT_ASSERT_EQUAL(entry->getType(), result->getType());

    delete result;
}

void BottomTest::testEnqueueTwiceOrdered()
{
    dcommon::Entry *entry1 = new dcommon::Entry(0.0, 0.0, 1, 1, 0);
    dcommon::Entry *entry2 = new dcommon::Entry(0.0, 1.0, 1, 1, 0);
    dcommon::Entry *result;

    m_bottom->push(entry1);
    m_bottom->push(entry2);
    CPPUNIT_ASSERT(m_bottom->size() == 2);

    result = m_bottom->front();
    m_bottom->pop_front();

    CPPUNIT_ASSERT(m_bottom->size() == 1);

    CPPUNIT_ASSERT_EQUAL(entry1->getArrival(), result->getArrival());
    CPPUNIT_ASSERT_EQUAL(entry1->getDestination(), result->getDestination());
    CPPUNIT_ASSERT_EQUAL(entry1->getOrigin(), result->getOrigin());
    CPPUNIT_ASSERT_EQUAL(entry1->getType(), result->getType());
    delete result;

    result = m_bottom->front();
    m_bottom->pop_front();
    CPPUNIT_ASSERT(m_bottom->size() == 0);

    CPPUNIT_ASSERT_EQUAL(entry2->getArrival(), result->getArrival());
    CPPUNIT_ASSERT_EQUAL(entry2->getDestination(), result->getDestination());
    CPPUNIT_ASSERT_EQUAL(entry2->getOrigin(), result->getOrigin());
    CPPUNIT_ASSERT_EQUAL(entry2->getType(), result->getType());
    delete result;
}

void BottomTest::testEnqueueAtTail()
{
    dcommon::Entry *resultOld = NULL;
    dcommon::Entry *resultNew = NULL;
    bool result = false;

    for (int i = 6; i > 1; --i) {
        dcommon::Entry *entry = new dcommon::Entry(0.0, (double) i, 1, 1, 0);
        m_bottom->push(entry);
    }

    CPPUNIT_ASSERT_EQUAL((boost::uint32_t) 5, m_bottom->size());
    dcommon::Entry *entry = new dcommon::Entry(0.0, 7, 1, 1, 0);
    m_bottom->push(entry);
    CPPUNIT_ASSERT_EQUAL((boost::uint32_t) 6, m_bottom->size());

    try {
        resultOld = m_bottom->front();
        m_bottom->pop_front();
        resultNew = m_bottom->front();
        m_bottom->pop_front();
    } catch (dcommon::QueueException q) {
        CPPUNIT_FAIL("Did not expect queue exception!");
    }

    while (m_bottom->size() > 0) {
        result = (resultOld->getArrival() <= resultNew->getArrival());

        CPPUNIT_ASSERT(result);

        resultOld = resultNew;
        delete resultNew;

        resultNew = m_bottom->front();
        m_bottom->pop_front();
    }

    delete resultNew;
}

void BottomTest::testEnqueueAtHead()
{
    dcommon::Entry *resultOld = NULL;
    dcommon::Entry *resultNew = NULL;
    bool result = false;

    for (int i = 1; i < 6; ++i) {
        dcommon::Entry *entry = new dcommon::Entry(0.0, (double) i, 1, 1, 0);
        m_bottom->push(entry);
    }

    dcommon::Entry *entry = new dcommon::Entry(0.0, 0, 1, 1, 0);
    m_bottom->push(entry);

    try {
        resultOld = m_bottom->front();
        m_bottom->pop_front();
        resultNew = m_bottom->front();
        m_bottom->pop_front();
    } catch (dcommon::QueueException q) {
        CPPUNIT_FAIL("Did not expect queue exception!");
    }

    while (m_bottom->size() > 0) {
        result = (resultOld->getArrival() <= resultNew->getArrival());

        CPPUNIT_ASSERT(result);

        resultOld = resultNew;
        delete resultNew;

        resultNew = m_bottom->front();
        m_bottom->pop_front();
    }

    delete resultNew;
}

void BottomTest::testStability()
{
    dcommon::Entry *resultOld = NULL;
    dcommon::Entry *resultNew = NULL;
    bool result = false;

    for (int i = 0; i < 3; ++i) {
        dcommon::Entry *entry = new dcommon::Entry(0.0, (double) i, i, 1, 0);
        m_bottom->push(entry);
    }
    for (int i = 0; i < 10; ++i) {
        dcommon::Entry *entry = new dcommon::Entry(0.0, 4.0, i + 3, 1, 0);
        m_bottom->push(entry);
    }
    for (int i = 0; i < 3; ++i) {
        dcommon::Entry *entry = new dcommon::Entry(0.0, (double) i + 5.0, i + 13, 1, 0);
        m_bottom->push(entry);
    }

    try {
        resultOld = m_bottom->front();
        m_bottom->pop_front();
        resultNew = m_bottom->front();
        m_bottom->pop_front();
    } catch (dcommon::QueueException q) {
        CPPUNIT_FAIL("Did not expect queue exception!");
    }

    while (m_bottom->size() > 0) {
        result = (resultOld->getDestination() < resultNew->getDestination());
        CPPUNIT_ASSERT(result);
        resultOld = resultNew;
        delete resultNew;

        resultNew = m_bottom->front();
        m_bottom->pop_front();
    }

    delete resultNew;
}

void BottomTest::testEnlistShort()
{
    dcommon::EntryList *list = new dcommon::EntryList();
    dcommon::Entry *resultOld = NULL;
    dcommon::Entry *resultNew = NULL;
    bool result = false;

    for (int i = 6; i < 9; ++i) {
        dcommon::Entry *entry = new dcommon::Entry(0.0, (double) i, 1, 1, 0);
        m_bottom->push(entry);
    }
    CPPUNIT_ASSERT(m_bottom->size() == 3);
    for (int i = 5; i > 0; --i) {
        dcommon::Entry *entry = new dcommon::Entry(0.0, (double) i, i, 1, 0);
        list->push_back(*entry);
    }

    m_bottom->push(list);
    CPPUNIT_ASSERT(m_bottom->size() == 8);

    try {
        resultOld = m_bottom->front();
        m_bottom->pop_front();
        resultNew = m_bottom->front();
        m_bottom->pop_front();
    } catch (dcommon::QueueException q) {
        CPPUNIT_FAIL("Did not expect queue exception!");
    }

    while (m_bottom->size() > 0) {
        result = (resultOld->getArrival() <= resultNew->getArrival());

        CPPUNIT_ASSERT(result);

        resultOld = resultNew;
        delete resultNew;

        resultNew = m_bottom->front();
        m_bottom->pop_front();
    }

    CPPUNIT_ASSERT(m_bottom->size() == 0);

    delete resultNew;
}

// void BottomTest::testEnlistShortStability()
// {
//     dcommon::EntryList *list = new dcommon::EntryList();
//     dcommon::Entry *resultOld = NULL;
//     dcommon::Entry *resultNew = NULL;
//     bool result = false;

//     for (int i = 0; i < 5; ++i) {
//         dcommon::Entry *entry = new dcommon::Entry(0.0, (double) i, i, 1, 0);
//         m_bottom->push(entry);
//     }
//     for (int i = 0; i < 5; ++i) {
//         dcommon::Entry *entry = new dcommon::Entry(0.0, 5, i + 5, 1, 0);
//         list->push_back(*entry);
//     }

//     m_bottom->push(list);

//     try {
//         resultOld = m_bottom->front();
//         m_bottom->pop_front();
//         resultNew = m_bottom->front();
//         m_bottom->pop_front();
//     } catch (dcommon::QueueException q) {
//         CPPUNIT_FAIL("Did not expect queue exception!");
//     }

//     while (m_bottom->size() > 0) {
//         result = (resultOld->getDestination() < resultNew->getDestination());
//         CPPUNIT_ASSERT(result);
//         resultOld = resultNew;
//         delete resultNew;

//         resultNew = m_bottom->front();
//         m_bottom->pop_front();
//     }

//     delete resultNew;
// }

void BottomTest::testEnlistLong()
{
    dcommon::EntryList *list = new dcommon::EntryList();
    dcommon::Entry *resultOld = NULL;
    dcommon::Entry *resultNew = NULL;
    bool result = false;

    for (int i = 16; i < 19; ++i) {
        dcommon::Entry *entry = new dcommon::Entry(0.0, (double) i, 1, 1, 0);
        m_bottom->push(entry);
    }
    CPPUNIT_ASSERT(m_bottom->size() == 3);
    for (int i = 15; i > 0; --i) {
        dcommon::Entry *entry = new dcommon::Entry(0.0, (double) i, i, 1, 0);
        list->push_back(*entry);
    }

    m_bottom->push(list);
    CPPUNIT_ASSERT(m_bottom->size() == 18);

    try {
        resultOld = m_bottom->front();
        m_bottom->pop_front();
        resultNew = m_bottom->front();
        m_bottom->pop_front();
    } catch (dcommon::QueueException q) {
        CPPUNIT_FAIL("Did not expect queue exception!");
    }

    while (m_bottom->size() > 0) {
        result = (resultOld->getArrival() <= resultNew->getArrival());

        CPPUNIT_ASSERT(result);

        resultOld = resultNew;
        delete resultNew;

        resultNew = m_bottom->front();
        m_bottom->pop_front();
    }

    CPPUNIT_ASSERT(m_bottom->size() == 0);

    delete resultNew;
}

// void BottomTest::testEnlistLongStability()
// {
//     Fifo *fifo = new Fifo();

//     dcommon::tEntrySP resultOld;
//     dcommon::tEntrySP resultNew;
//     dcommon::tEntrySP entry;

//     for (int i = 0; i < 3; ++i) {
//         entry = dcommon::tEntrySP(new entry_t((double) i, i, 1, 0));
//         m_bottom->enqueue(entry);
//     }
//     for (int i = 0; i < 15; ++i) {
//         entry = dcommon::tEntrySP(new entry_t(3.0, i + 3, 1, 0));
//         fifo->enqueue(entry);
//     }

//     long size = fifo->size();
//     node_double_t *list = fifo->delist();

//     m_bottom->enlist(list->next, size);

//     resultOld = m_bottom->dequeue();
//     resultNew = m_bottom->dequeue();

//     while (resultNew.get() != NULL) {
//         CPPUNIT_ASSERT(resultOld->getDestination() < resultNew->getDestination());
//         resultOld = resultNew;
//         resultNew = m_bottom->dequeue();
//     }
// }

void BottomTest::testDelist()
{
    dcommon::Entry *entry1 = new dcommon::Entry(0.0, 0.0, 1, 1, 0);
    dcommon::Entry *entry2 = new dcommon::Entry(0.0, 1.0, 2, 2, 1);
    dcommon::EntryList *result;
    dcommon::EntryListIterator it, itend;

    m_bottom->push(entry1);
    m_bottom->push(entry2);
    result = m_bottom->list();

    it = result->begin();
    itend = result->end();

    CPPUNIT_ASSERT_EQUAL(entry1->getArrival(), it->getArrival());
    CPPUNIT_ASSERT_EQUAL(entry2->getArrival(), (++it)->getArrival());

    result->clear_and_dispose(dcommon::delete_disposer());
    CPPUNIT_ASSERT(m_bottom->size() == 0);
}

void BottomTest::testMaxTS()
{
    CPPUNIT_ASSERT_EQUAL(m_bottom->getMaxTS(), (double) 0.0);

    dcommon::Entry *entry1 = new dcommon::Entry(0.0, 3.1, 1, 1, 0);
    m_bottom->push(entry1);
    CPPUNIT_ASSERT_EQUAL(m_bottom->getMaxTS(), (double) 3.1);
}

void BottomTest::testMinTS()
{
    CPPUNIT_ASSERT_EQUAL(m_bottom->getMinTS(), (double) 0.0);

    dcommon::Entry *entry1 = new dcommon::Entry(0.0, 3.1, 1, 1, 0);
    m_bottom->push(entry1);
    CPPUNIT_ASSERT_EQUAL(m_bottom->getMinTS(), (double) 3.1);
}
