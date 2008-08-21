// Copyright (C) 2007-2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "FifoTest.hh"

#include "Entry.hh"
using des::common::entry_t;

#include "List.hh"
using des::common::node_double_t;



// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(FifoTest);


void FifoTest::setUp()
{
    m_fifo = new Fifo();
}

void FifoTest::tearDown()
{
    delete(m_fifo);
}

void FifoTest::testEnqueue()
{
    dcommon::tEntrySP entry = dcommon::tEntrySP(new entry_t(0.0, 1, 1, 0));
    dcommon::tEntrySP result;

    m_fifo->enqueue(entry);
    result = m_fifo->dequeue();

    CPPUNIT_ASSERT_EQUAL(entry->arrival, result->arrival);
    CPPUNIT_ASSERT_EQUAL(entry->destination, result->destination);
    CPPUNIT_ASSERT_EQUAL(entry->origin, result->origin);
    CPPUNIT_ASSERT_EQUAL(entry->type, result->type);
}

void FifoTest::testOrder()
{
    dcommon::tEntrySP entry1 = dcommon::tEntrySP(new entry_t(0.0, 1, 1, 0));
    dcommon::tEntrySP entry2 = dcommon::tEntrySP(new entry_t(1.0, 2, 2, 1));
    dcommon::tEntrySP result;

    CPPUNIT_ASSERT(m_fifo->size() == 0);

    m_fifo->enqueue(entry1);

    CPPUNIT_ASSERT(m_fifo->size() == 1);

    m_fifo->enqueue(entry2);

    CPPUNIT_ASSERT(m_fifo->size() == 2);

    // first dequeue
    result = m_fifo->dequeue();

    CPPUNIT_ASSERT(m_fifo->size() == 1);

    CPPUNIT_ASSERT_EQUAL(entry1->arrival, result->arrival);
    CPPUNIT_ASSERT_EQUAL(entry1->destination, result->destination);
    CPPUNIT_ASSERT_EQUAL(entry1->origin, result->origin);
    CPPUNIT_ASSERT_EQUAL(entry1->type, result->type);

    // second dequeue
    result = m_fifo->dequeue();

    CPPUNIT_ASSERT(m_fifo->size() == 0);

    CPPUNIT_ASSERT_EQUAL(entry2->arrival, result->arrival);
    CPPUNIT_ASSERT_EQUAL(entry2->destination, result->destination);
    CPPUNIT_ASSERT_EQUAL(entry2->origin, result->origin);
    CPPUNIT_ASSERT_EQUAL(entry2->type, result->type);
}

void FifoTest::testEmpty()
{
    dcommon::tEntrySP result;

    CPPUNIT_ASSERT(m_fifo->size() == 0);

    // empty dequeue
    result = m_fifo->dequeue();

    CPPUNIT_ASSERT(m_fifo->size() == 0);
    CPPUNIT_ASSERT(result.get() == NULL);
}

void FifoTest::testList()
{
    dcommon::tEntrySP entry1 = dcommon::tEntrySP(new entry_t(0.0, 1, 1, 0));
    dcommon::tEntrySP entry2 = dcommon::tEntrySP(new entry_t(1.0, 2, 2, 1));
    node_double_t *result = NULL;
    node_double_t *current = NULL;

    m_fifo->enqueue(entry1);
    m_fifo->enqueue(entry2);
    result = m_fifo->delist();

    CPPUNIT_ASSERT(m_fifo->size() == 0);

    current = result->next;
    CPPUNIT_ASSERT_EQUAL(entry1->arrival, current->data->arrival);

    current = current->next;
    CPPUNIT_ASSERT_EQUAL(entry2->arrival, current->data->arrival);

    current = current->next;
    CPPUNIT_ASSERT((current->data).get() == NULL);

    delete result->next->next->next;
    delete result->next->next;
    delete result->next;
    delete result;
}

void FifoTest::testNewList()
{
    dcommon::tEntrySP entry1 = dcommon::tEntrySP(new entry_t(0.0, 1, 1, 0));
    dcommon::tEntrySP entry2 = dcommon::tEntrySP(new entry_t(1.0, 2, 2, 1));
    dcommon::tEntrySP entry3 = dcommon::tEntrySP(new entry_t(1.0, 2, 2, 1));
    dcommon::tEntrySP entry;
    node_double_t *result = NULL;

    m_fifo->enqueue(entry1);
    m_fifo->enqueue(entry2);
    result = m_fifo->delist();

    CPPUNIT_ASSERT(m_fifo->size() == 0);

    m_fifo->enqueue(entry3);

    CPPUNIT_ASSERT(m_fifo->size() == 1);

    entry = m_fifo->dequeue();

    CPPUNIT_ASSERT_EQUAL(entry3->arrival, entry->arrival);
    CPPUNIT_ASSERT_EQUAL(entry3->destination, entry->destination);
    CPPUNIT_ASSERT_EQUAL(entry3->origin, entry->origin);
    CPPUNIT_ASSERT_EQUAL(entry3->type, entry->type);
    CPPUNIT_ASSERT(m_fifo->size() == 0);

    delete result->next->next->next;
    delete result->next->next;
    delete result->next;
    delete result;
}

void FifoTest::testEnlist()
{
    Fifo *fifo = new Fifo();

    dcommon::tEntrySP resultOld;
    dcommon::tEntrySP resultNew;
    dcommon::tEntrySP entry;

    for (int i = 0; i < 3; ++i) {
        entry = dcommon::tEntrySP(new entry_t((double) i, i, 1, 0));
        fifo->enqueue(entry);
    }

    long size = fifo->size();
    node_double_t *list = fifo->delist();

    m_fifo->enlist(list->next, size);

    resultOld = m_fifo->dequeue();
    resultNew = m_fifo->dequeue();

    while (resultNew.get() != NULL) {
        CPPUNIT_ASSERT(resultOld->destination < resultNew->destination);
        resultOld = resultNew;
        resultNew = m_fifo->dequeue();
    }

    delete fifo;
}
