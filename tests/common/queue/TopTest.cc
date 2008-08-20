// Copyright (C) 2007-2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "Entry.hh"
using des::common::entry_t;

#include "Fifo.hh"
using des::common::Fifo;

#include "List.hh"
using des::common::node_double_t;

#include "TopTest.hh"


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TopTest);



void TopTest::setUp()
{
    m_top = new Top();
}

void TopTest::tearDown()
{
    delete m_top;
}

void TopTest::testInitial()
{
    CPPUNIT_ASSERT(m_top->getMaxTS() == 0.0);
    CPPUNIT_ASSERT(m_top->getMinTS() == DBL_MAX);
    CPPUNIT_ASSERT(m_top->getTopStart() == 0.0);
    CPPUNIT_ASSERT(m_top->getNTop() == 0);
}
void TopTest::testEnqueueOnce()
{
    entry_t *const entry = new entry_t(3.1, 0.0, 1, 1, 0);

    CPPUNIT_ASSERT(m_top->getNTop() == 0);
    m_top->enqueue(entry);
    CPPUNIT_ASSERT(m_top->getNTop() == 1);

    CPPUNIT_ASSERT_EQUAL(m_top->getMaxTS(), 3.1);
    CPPUNIT_ASSERT_EQUAL(m_top->getMinTS(), 3.1);

    delete entry;
}

void TopTest::testEnqueueTwice()
{
    entry_t *const entry1 = new entry_t(0.5, 0.0, 1, 1, 0);
    entry_t *const entry2 = new entry_t(1.0, 0.0, 2, 2, 1);
    entry_t *result = NULL;

    CPPUNIT_ASSERT(m_top->getNTop() == 0);

    m_top->enqueue(entry1);

    CPPUNIT_ASSERT(m_top->getNTop() == 1);
    CPPUNIT_ASSERT(m_top->getMaxTS() == 0.5);
    CPPUNIT_ASSERT(m_top->getMinTS() == 0.5);

    m_top->enqueue(entry2);

    CPPUNIT_ASSERT(m_top->getNTop() == 2);
    CPPUNIT_ASSERT(m_top->getMaxTS() == 1.0);
    CPPUNIT_ASSERT(m_top->getMinTS() == 0.5);

    // first dequeue
    result = m_top->dequeue();

    CPPUNIT_ASSERT(m_top->getNTop() == 1);

    // second dequeue
    result = m_top->dequeue();

    CPPUNIT_ASSERT(m_top->getNTop() == 0);

    delete entry1;
    delete entry2;
}

void TopTest::testResetOk()
{
    entry_t *const entry1 = new entry_t(0.5, 0.0, 1, 1, 0);

    m_top->reset();

    m_top->enqueue(entry1);
    m_top->dequeue();

    m_top->reset();

    delete entry1;
}

void TopTest::testResetThrows()
{
    entry_t *const entry1 = new entry_t(0.5, 0.0, 1, 1, 0);

    m_top->enqueue(entry1);

    try {
        m_top->reset();
        CPPUNIT_FAIL("Expected an exception!");
    } catch (QueueException &qe) {
    }

    delete entry1;
}

void TopTest::testList()
{
    entry_t *const entry1 = new entry_t(0.0, 0.0, 1, 1, 0);
    entry_t *const entry2 = new entry_t(1.0, 0.0, 2, 2, 1);
    entry_t *const entry3 = new entry_t(1.0, 0.0, 2, 2, 1);
    entry_t *entry = NULL;
    node_double_t *result = NULL;

    m_top->enqueue(entry1);
    m_top->enqueue(entry2);
    result = m_top->delist();

    CPPUNIT_ASSERT(m_top->getNTop() == 0);

    m_top->enqueue(entry3);

    CPPUNIT_ASSERT(m_top->getNTop() == 1);

    entry = m_top->dequeue();

    CPPUNIT_ASSERT_EQUAL(entry3->arrival, entry->arrival);
    CPPUNIT_ASSERT_EQUAL(entry3->destination, entry->destination);
    CPPUNIT_ASSERT_EQUAL(entry3->origin, entry->origin);
    CPPUNIT_ASSERT_EQUAL(entry3->type, entry->type);
    CPPUNIT_ASSERT(m_top->getNTop() == 0);

    delete entry1;
    delete entry2;
    delete entry3;

    delete result->next->next->next;
    delete result->next->next;
    delete result->next;
    delete result;
}

void TopTest::test100()
{
    for (int i = 0; i < 100; ++i) {
        struct entry_t *const entry = new entry_t((double) i, 0.0, 1, 1, 0);
        m_top->enqueue(entry);
    }

    CPPUNIT_ASSERT(m_top->getNTop() == 100);

    for (int i = 0; i < 100; ++i) {
        struct entry_t *const entry = m_top->dequeue();
        CPPUNIT_ASSERT(entry->arrival == i);
        delete entry;
    }

    CPPUNIT_ASSERT(m_top->getNTop() == 0);
}

void TopTest::test100EnDe()
{
    entry_t *entry = NULL;
    entry_t *result = NULL;

    for (int i = 0; i < 100; ++i) {
        entry = new entry_t((double) i, 0.0, 1, 1, 0);

        m_top->enqueue(entry);
        CPPUNIT_ASSERT(m_top->getNTop() == 1);

        result = m_top->dequeue();
        CPPUNIT_ASSERT(result->arrival == i);
        CPPUNIT_ASSERT(m_top->getNTop() == 0);

        delete entry;
    }
}

void TopTest::testEnlist()
{
    Fifo *fifo = new Fifo();

    entry_t *resultOld = NULL;
    entry_t *resultNew = NULL;
    entry_t *entry = NULL;

    for (int i = 0; i < 3; ++i) {
        entry = new entry_t((double) i, 0.0, i, 1, 0);
        fifo->enqueue(entry);
    }

    long size = fifo->size();
    node_double_t *list = fifo->delist();

    m_top->enlist(list->next, size);

    resultOld = m_top->dequeue();
    resultNew = m_top->dequeue();

    while (resultNew != NULL) {
        CPPUNIT_ASSERT(resultOld->destination < resultNew->destination);
        delete resultOld;
        resultOld = resultNew;
        resultNew = m_top->dequeue();
    }

    delete resultOld;
    delete resultNew;
    delete fifo;
}
