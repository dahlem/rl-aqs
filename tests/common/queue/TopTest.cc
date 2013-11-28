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
    dcommon::Entry *entry = new dcommon::Entry(0.0, 3.1, 1, 1, 0);

    CPPUNIT_ASSERT(m_top->getNTop() == 0);
    m_top->push(entry);
    CPPUNIT_ASSERT(m_top->getNTop() == 1);

    CPPUNIT_ASSERT_EQUAL(m_top->getMaxTS(), 3.1);
    CPPUNIT_ASSERT_EQUAL(m_top->getMinTS(), 3.1);
}

void TopTest::testEnqueueTwice()
{
    dcommon::Entry *entry1 = new dcommon::Entry(0.0, 0.5, 1, 1, 0);
    dcommon::Entry *entry2 = new dcommon::Entry(0.0, 1.0, 2, 2, 1);
    dcommon::EntryList *result = NULL;
    dcommon::Entry entry;
    dcommon::Entry *toDelete;

    CPPUNIT_ASSERT(m_top->getNTop() == 0);

    m_top->push(entry1);

    CPPUNIT_ASSERT(m_top->getNTop() == 1);
    CPPUNIT_ASSERT(m_top->getMaxTS() == 0.5);
    CPPUNIT_ASSERT(m_top->getMinTS() == 0.5);

    m_top->push(entry2);

    CPPUNIT_ASSERT(m_top->getNTop() == 2);
    CPPUNIT_ASSERT(m_top->getMaxTS() == 1.0);
    CPPUNIT_ASSERT(m_top->getMinTS() == 0.5);

    result = m_top->delist();

    CPPUNIT_ASSERT_EQUAL((unsigned int) 2, result->size());
    toDelete = reinterpret_cast<dcommon::Entry*>(&result->front());
    result->pop_front();
    delete toDelete;

    toDelete = reinterpret_cast<dcommon::Entry*>(&result->front());
    result->pop_front();
    delete toDelete;

    CPPUNIT_ASSERT_EQUAL((unsigned int) 0, m_top->getNTop());
}

void TopTest::testResetOk()
{
    dcommon::Entry *entry1 = new dcommon::Entry(0.0, 0.5, 1, 1, 0);

    try {
        m_top->reset();
    } catch (dcommon::QueueException q) {
        CPPUNIT_FAIL("Did not expect queue exception!");
    }

    CPPUNIT_ASSERT(m_top->getMaxTS() == 0);
    CPPUNIT_ASSERT(m_top->getMinTS() == DBL_MAX);
    m_top->push(entry1);

    try {
        m_top->reset();
        CPPUNIT_FAIL("Did expect queue exception!");
    } catch (dcommon::QueueException q) {
    }

    CPPUNIT_ASSERT(m_top->getMaxTS() == 0.5);
    CPPUNIT_ASSERT(m_top->getMinTS() == 0.5);
}

void TopTest::testResetThrows()
{
    dcommon::Entry *entry1 = new dcommon::Entry(0.0, 0.5, 1, 1, 0);

    m_top->push(entry1);

    try {
        m_top->reset();
        CPPUNIT_FAIL("Expected an exception!");
    } catch (dcommon::QueueException &qe) {
    }
}

void TopTest::testList()
{
    dcommon::Entry *entry1 = new dcommon::Entry(0.0, 0.0, 1, 1, 0);
    dcommon::Entry *entry2 = new dcommon::Entry(0.0, 1.0, 2, 2, 1);
    dcommon::Entry *entry3 = new dcommon::Entry(0.0, 1.0, 2, 2, 1);
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
}

void TopTest::test100()
{
    dcommon::EntryList *result = NULL;

    for (int i = 0; i < 100; ++i) {
        dcommon::Entry *entry = new dcommon::Entry(0.0, (double) i, 1, 1, 0);
        m_top->push(entry);
    }

    CPPUNIT_ASSERT(m_top->getNTop() == 100);

    result = m_top->delist();

    int i = 0;
    while (!result->empty()) {
        dcommon::Entry *entry = reinterpret_cast<dcommon::Entry*>(&result->front());
        result->pop_front();
        CPPUNIT_ASSERT(entry->getArrival() == i);
        delete entry;
        i++;
    }

    CPPUNIT_ASSERT(m_top->getNTop() == 0);
}


void TopTest::testEnlist()
{
    dcommon::EntryList *fifo = new dcommon::EntryList();
    dcommon::EntryList *result = NULL;
    dcommon::Entry *resultOld;
    dcommon::Entry *resultNew;
    dcommon::Entry *entry;
    bool test;

    for (int i = 0; i < 3; ++i) {
        entry = new dcommon::Entry(0.0, (double) i, i, 1, 0);
        fifo->push_back(*entry);
    }

    m_top->push(fifo);

    result = m_top->delist();
    resultOld = reinterpret_cast<dcommon::Entry*>(&result->front());
    result->pop_front();

    while (!result->empty()) {
        resultNew = reinterpret_cast<dcommon::Entry*>(&result->front());
        result->pop_front();
        test = (resultOld->getDestination() < resultNew->getDestination());
        CPPUNIT_ASSERT(test);
        resultOld = resultNew;
        delete resultNew;
    }

    delete fifo;
}
