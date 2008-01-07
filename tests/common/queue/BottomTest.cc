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
using std::cout;
using std::endl;

#include "BottomTest.hh"
#include "Entry.hh"
using des::common::entry_t;

#include "Fifo.hh"
using des::common::Fifo;

#include "List.hh"
using des::common::node_double_t;




// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(BottomTest);


void BottomTest::setUp()
{
    m_bottom = new Bottom();
}

void BottomTest::tearDown()
{
    delete m_bottom;
}

void BottomTest::testEnqueueNull()
{
    entry_t *result = NULL;
    
    m_bottom->enqueue(NULL);
    result = m_bottom->dequeue();
    
    CPPUNIT_ASSERT(result == NULL);
}

void BottomTest::testEnqueue()
{
    for (int i = 1; i < 6; ++i) {
        entry_t *const entry = new entry_t((double) i, 1, 1, 0);
        m_bottom->enqueue(entry);
    }
    
    CPPUNIT_ASSERT(m_bottom->size() == 5);
}

void BottomTest::testEnqueueOnce()
{
    entry_t *const entry = new entry_t(0.0, 1, 1, 0);
    entry_t *result = NULL;
    
    m_bottom->enqueue(entry);
    result = m_bottom->dequeue();
    
    CPPUNIT_ASSERT_EQUAL(entry->arrival, result->arrival);
    CPPUNIT_ASSERT_EQUAL(entry->destination, result->destination);
    CPPUNIT_ASSERT_EQUAL(entry->origin, result->origin);
    CPPUNIT_ASSERT_EQUAL(entry->type, result->type);

    delete entry;
}

void BottomTest::testEnqueueTwiceOrdered()
{
    entry_t *const entry1 = new entry_t(0.0, 1, 1, 0);
    entry_t *const entry2 = new entry_t(1.0, 1, 1, 0);
    entry_t *result = NULL;
    
    m_bottom->enqueue(entry1);
    m_bottom->enqueue(entry2);
    CPPUNIT_ASSERT(m_bottom->size() == 2);

    result = m_bottom->dequeue();
    CPPUNIT_ASSERT(m_bottom->size() == 1);
    
    CPPUNIT_ASSERT_EQUAL(entry1->arrival, result->arrival);
    CPPUNIT_ASSERT_EQUAL(entry1->destination, result->destination);
    CPPUNIT_ASSERT_EQUAL(entry1->origin, result->origin);
    CPPUNIT_ASSERT_EQUAL(entry1->type, result->type);

    result = m_bottom->dequeue();
    CPPUNIT_ASSERT(m_bottom->size() == 0);
    
    CPPUNIT_ASSERT_EQUAL(entry2->arrival, result->arrival);
    CPPUNIT_ASSERT_EQUAL(entry2->destination, result->destination);
    CPPUNIT_ASSERT_EQUAL(entry2->origin, result->origin);
    CPPUNIT_ASSERT_EQUAL(entry2->type, result->type);

    delete entry1;
    delete entry2;
}

void BottomTest::testEnqueueAtTail()
{
    entry_t *resultOld = NULL;
    entry_t *resultNew = NULL;
    
    for (int i = 6; i < 1; --i) {
        entry_t *const entry = new entry_t((double) i, 1, 1, 0);
        m_bottom->enqueue(entry);
    }

    entry_t *const entry = new entry_t(7, 1, 1, 0);
    m_bottom->enqueue(entry);

    resultOld = m_bottom->dequeue();
    resultNew = m_bottom->dequeue();
    

    while (resultNew != NULL) {
        CPPUNIT_ASSERT(resultOld->arrival
                       <= resultNew->arrival);
        delete resultOld;
        resultOld = resultNew;
        resultNew = m_bottom->dequeue();
    }

    delete resultNew;
}

void BottomTest::testEnqueueAtHead()
{
    entry_t *resultOld = NULL;
    entry_t *resultNew = NULL;
    
    for (int i = 1; i < 6; ++i) {
        entry_t *const entry = new entry_t((double) i, 1, 1, 0);
        m_bottom->enqueue(entry);
    }

    entry_t *const entry = new entry_t(0, 1, 1, 0);
    m_bottom->enqueue(entry);

    resultOld = m_bottom->dequeue();
    resultNew = m_bottom->dequeue();

    while (resultNew != NULL) {
        CPPUNIT_ASSERT(resultOld->arrival
                       <= resultNew->arrival);
        delete resultOld;
        resultOld = resultNew;
        resultNew = m_bottom->dequeue();
    }

    delete resultNew;
}

void BottomTest::testStability()
{
    entry_t *resultOld = NULL;
    entry_t *resultNew = NULL;
    
    for (int i = 0; i < 3; ++i) {
        entry_t *const entry = new entry_t((double) i, i, 1, 0);
        m_bottom->enqueue(entry);
    }
    for (int i = 0; i < 10; ++i) {
        entry_t *const entry = new entry_t(4.0, i + 3, 1, 0);
        m_bottom->enqueue(entry);
    }
    for (int i = 0; i < 3; ++i) {
        entry_t *const entry = new entry_t((double) i + 5.0, i + 13, 1, 0);
        m_bottom->enqueue(entry);
    }

    resultOld = m_bottom->dequeue();
    resultNew = m_bottom->dequeue();

    while (resultNew != NULL) {
        CPPUNIT_ASSERT(resultOld->destination < resultNew->destination);
        delete resultOld;
        resultOld = resultNew;
        resultNew = m_bottom->dequeue();
    }

    delete resultNew;
}

void BottomTest::testEnlistShort()
{
    Fifo *fifo = new Fifo();
    
    entry_t *resultOld = NULL;
    entry_t *resultNew = NULL;
    
    for (int i = 6; i < 9; ++i) {
        entry_t *const entry = new entry_t((double) i, 1, 1, 0);
        m_bottom->enqueue(entry);
    }
    CPPUNIT_ASSERT(m_bottom->size() == 3);
    for (int i = 5; i > 0; --i) {
        entry_t *const entry = new entry_t((double) i, i, 1, 0);
        fifo->enqueue(entry);
    }

    int size = fifo->size();
    node_double_t *list = fifo->delist();

    m_bottom->enlist(list->next, size);
    CPPUNIT_ASSERT(m_bottom->size() == 8);

    resultOld = m_bottom->dequeue();
    resultNew = m_bottom->dequeue();

    while (resultNew != NULL) {
        CPPUNIT_ASSERT(resultOld->arrival < resultNew->arrival);
        delete resultOld;
        resultOld = resultNew;
        resultNew = m_bottom->dequeue();
    }

    CPPUNIT_ASSERT(m_bottom->size() == 0);
    delete resultNew;
    delete fifo;
}

void BottomTest::testEnlistShortStability()
{
    Fifo *fifo = new Fifo();
    
    entry_t *resultOld = NULL;
    entry_t *resultNew = NULL;
    
    for (int i = 0; i < 5; ++i) {
        entry_t *const entry = new entry_t((double) i, i, 1, 0);
        m_bottom->enqueue(entry);
    }
    for (int i = 0; i < 5; ++i) {
        entry_t *const entry = new entry_t(5, i + 5, 1, 0);
        fifo->enqueue(entry);
    }

    int size = fifo->size();
    node_double_t *list = fifo->delist();

    m_bottom->enlist(list->next, size);

    resultOld = m_bottom->dequeue();
    resultNew = m_bottom->dequeue();

    while (resultNew != NULL) {
        CPPUNIT_ASSERT(resultOld->destination < resultNew->destination);
        delete resultOld;
        resultOld = resultNew;
        resultNew = m_bottom->dequeue();
    }

    delete resultNew;
    delete fifo;
}

void BottomTest::testEnlistLong()
{
    Fifo *fifo = new Fifo();
    
    entry_t *resultOld = NULL;
    entry_t *resultNew = NULL;
    
    for (int i = 16; i < 19; ++i) {
        entry_t *const entry = new entry_t((double) i, 1, 1, 0);
        m_bottom->enqueue(entry);
    }
    CPPUNIT_ASSERT(m_bottom->size() == 3);
    for (int i = 15; i > 0; --i) {
        entry_t *const entry = new entry_t((double) i, i, 1, 0);
        fifo->enqueue(entry);
    }

    int size = fifo->size();
    node_double_t *list = fifo->delist();

    m_bottom->enlist(list->next, size);
    CPPUNIT_ASSERT(m_bottom->size() == 18);

    resultOld = m_bottom->dequeue();
    resultNew = m_bottom->dequeue();

    while (resultNew != NULL) {
        CPPUNIT_ASSERT(resultOld->arrival < resultNew->arrival);
        delete resultOld;
        resultOld = resultNew;
        resultNew = m_bottom->dequeue();
    }

    delete resultNew;
    delete fifo;
}

void BottomTest::testEnlistLongStability()
{
    Fifo *fifo = new Fifo();
    
    entry_t *resultOld = NULL;
    entry_t *resultNew = NULL;
    entry_t *entry = NULL;
    
    for (int i = 0; i < 3; ++i) {
        entry = new entry_t((double) i, i, 1, 0);
        m_bottom->enqueue(entry);
    }
    for (int i = 0; i < 15; ++i) {
        entry = new entry_t(3.0, i + 3, 1, 0);
        fifo->enqueue(entry);
    }

    long size = fifo->size();
    node_double_t *list = fifo->delist();

    m_bottom->enlist(list->next, size);

    resultOld = m_bottom->dequeue();
    resultNew = m_bottom->dequeue();

    while (resultNew != NULL) {
        CPPUNIT_ASSERT(resultOld->destination < resultNew->destination);
        delete resultOld;
        resultOld = resultNew;
        resultNew = m_bottom->dequeue();
    }

    delete resultNew;
    delete fifo;
}

void BottomTest::testDelist()
{
    entry_t *const entry1 = new entry_t(0.0, 1, 1, 0);
    entry_t *const entry2 = new entry_t(1.0, 2, 2, 1);
    node_double_t *result = NULL;
    node_double_t *current = NULL;
    
    m_bottom->enqueue(entry1);
    m_bottom->enqueue(entry2);
    result = m_bottom->delist();

    CPPUNIT_ASSERT(m_bottom->size() == 0);    
    
    current = result->next;
    CPPUNIT_ASSERT_EQUAL(entry1->arrival, current->data->arrival);

    current = current->next;
    CPPUNIT_ASSERT_EQUAL(entry2->arrival, current->data->arrival);

    current = current->next;
    CPPUNIT_ASSERT(current->data == NULL);

    delete entry1;
    delete entry2;

    delete result->next->next->next;
    delete result->next->next;
    delete result->next;
    delete result;
}

void BottomTest::testMaxTS() 
{
    CPPUNIT_ASSERT_EQUAL(m_bottom->getMaxTS(), (double) 0.0);

    entry_t *const entry1 = new entry_t(3.1, 1, 1, 0);
    m_bottom->enqueue(entry1);
    CPPUNIT_ASSERT_EQUAL(m_bottom->getMaxTS(), (double) 3.1);
}

void BottomTest::testMinTS() 
{
    CPPUNIT_ASSERT_EQUAL(m_bottom->getMinTS(), (double) 0.0);

    entry_t *const entry1 = new entry_t(3.1, 1, 1, 0);
    m_bottom->enqueue(entry1);
    CPPUNIT_ASSERT_EQUAL(m_bottom->getMinTS(), (double) 3.1);
}
