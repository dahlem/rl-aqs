// Copyright (C) 2007-2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#include <iostream>

#include "Entry.hh"
#include "TopTest.hh"


namespace dcommon = des::common;


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(TopTest);



void TopTest::setUp()
{
    m_top = new dcommon::Top();
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
    dcommon::Entry *entry = new dcommon::Entry(3.1, 1, 1, 0);

    CPPUNIT_ASSERT(m_top->getNTop() == 0);
    m_top->push(entry);
    CPPUNIT_ASSERT(m_top->getNTop() == 1);

    CPPUNIT_ASSERT_EQUAL(m_top->getMaxTS(), 3.1);
    CPPUNIT_ASSERT_EQUAL(m_top->getMinTS(), 3.1);
}

void TopTest::testEnqueueTwice()
{
    dcommon::Entry *entry1 = new dcommon::Entry(0.5, 1, 1, 0);
    dcommon::Entry *entry2 = new dcommon::Entry(1.0, 2, 2, 1);
    dcommon::Entry *result = NULL;

    CPPUNIT_ASSERT(m_top->getNTop() == 0);

    m_top->push(entry1);

    CPPUNIT_ASSERT(m_top->getNTop() == 1);
    CPPUNIT_ASSERT(m_top->getMaxTS() == 0.5);
    CPPUNIT_ASSERT(m_top->getMinTS() == 0.5);

    m_top->push(entry2);

    CPPUNIT_ASSERT(m_top->getNTop() == 2);
    CPPUNIT_ASSERT(m_top->getMaxTS() == 1.0);
    CPPUNIT_ASSERT(m_top->getMinTS() == 0.5);

    // first dequeue
    result = m_top->front();
    m_top->pop_front();

    CPPUNIT_ASSERT(m_top->getNTop() == 1);

    // second dequeue
    result = m_top->front();
    m_top->pop_front();

    CPPUNIT_ASSERT(m_top->getNTop() == 0);
}

void TopTest::testResetOk()
{
    dcommon::Entry *entry1 = new dcommon::Entry(0.5, 1, 1, 0);

    try {
        m_top->reset();
    } catch (dcommon::QueueException q) {
        CPPUNIT_FAIL("Did not expect queue exception!");
    }

    CPPUNIT_ASSERT(m_top->getMaxTS() == 0);
    CPPUNIT_ASSERT(m_top->getMinTS() == DBL_MAX);
    m_top->push(entry1);
    m_top->pop_front();

    try {
        m_top->reset();
    } catch (dcommon::QueueException q) {
        CPPUNIT_FAIL("Did not expect queue exception!");
    }

    CPPUNIT_ASSERT(m_top->getMaxTS() == 0);
    CPPUNIT_ASSERT(m_top->getMinTS() == DBL_MAX);
}

void TopTest::testResetThrows()
{
    dcommon::Entry *entry1 = new dcommon::Entry(0.5, 1, 1, 0);

    m_top->push(entry1);

    try {
        m_top->reset();
        CPPUNIT_FAIL("Expected an exception!");
    } catch (dcommon::QueueException &qe) {
    }
}

void TopTest::testList()
{
    dcommon::Entry *entry1 = new dcommon::Entry(0.0, 1, 1, 0);
    dcommon::Entry *entry2 = new dcommon::Entry(1.0, 2, 2, 1);
    dcommon::Entry *entry3 = new dcommon::Entry(1.0, 2, 2, 1);
    dcommon::Entry *entry = NULL;
    dcommon::EntryList *result = NULL;

    m_top->push(entry1);
    m_top->push(entry2);

    result = m_top->delist();

    while (!result->empty()) {
        entry = reinterpret_cast<dcommon::Entry*>(&result->front());
        result->pop_front();
        delete entry;
    }

    try {
        m_top->reset();
    } catch (dcommon::QueueException q) {
        CPPUNIT_FAIL("Did not expect queue exception!");
    }

    CPPUNIT_ASSERT(m_top->getNTop() == 0);

    m_top->push(entry3);

    CPPUNIT_ASSERT(m_top->getNTop() == 1);

    entry = m_top->front();
    m_top->pop_front();

    CPPUNIT_ASSERT_EQUAL(entry3->arrival, entry->arrival);
    CPPUNIT_ASSERT_EQUAL(entry3->destination, entry->destination);
    CPPUNIT_ASSERT_EQUAL(entry3->origin, entry->origin);
    CPPUNIT_ASSERT_EQUAL(entry3->type, entry->type);
    CPPUNIT_ASSERT(m_top->getNTop() == 0);
}

void TopTest::test100()
{
    for (int i = 0; i < 100; ++i) {
        dcommon::Entry *entry = new dcommon::Entry((double) i, 1, 1, 0);
        m_top->push(entry);
    }

    CPPUNIT_ASSERT(m_top->getNTop() == 100);

    for (int i = 0; i < 100; ++i) {
        dcommon::Entry *entry = m_top->front();
        m_top->pop_front();
        CPPUNIT_ASSERT(entry->arrival == i);
    }

    CPPUNIT_ASSERT(m_top->getNTop() == 0);
}

void TopTest::test100EnDe()
{
    dcommon::Entry *entry;
    dcommon::Entry *result;

    for (int i = 0; i < 100; ++i) {
        entry = new dcommon::Entry((double) i, 1, 1, 0);

        m_top->push(entry);
        CPPUNIT_ASSERT(m_top->getNTop() == 1);

        result = m_top->front();
        m_top->pop_front();
        CPPUNIT_ASSERT(result->arrival == i);
        CPPUNIT_ASSERT(m_top->getNTop() == 0);
    }
}

void TopTest::testEnlist()
{
    dcommon::EntryList *fifo = new dcommon::EntryList();
    dcommon::Entry *resultOld;
    dcommon::Entry *resultNew;
    dcommon::Entry *entry;
    bool result;

    for (int i = 0; i < 3; ++i) {
        entry = new dcommon::Entry((double) i, i, 1, 0);
        fifo->push_back(*entry);
    }

    m_top->push(fifo);

    resultOld = m_top->front();
    m_top->pop_front();
    resultNew = m_top->front();
    m_top->pop_front();

    while (m_top->getNTop() > 0) {
        result = (resultOld->destination < resultNew->destination);
        CPPUNIT_ASSERT(result);
        resultOld = resultNew;
        delete resultNew;

        resultNew = m_top->front();
        m_top->pop_front();
    }

    delete fifo;
}
