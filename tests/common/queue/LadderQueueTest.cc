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


#include "Entry.hh"
using des::common::entry_t;

#include "LadderQueueTest.hh"


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(LadderQueueTest);



void LadderQueueTest::setUp()
{
    m_ladderqueue = new LadderQueue();
}

void LadderQueueTest::tearDown()
{
    delete m_ladderqueue;
}

void LadderQueueTest::testEnqueue10()
{
    for (int i = 10; i > 0; --i) {
        entry_t *entry = new entry_t((double) i, 0.0, 1, 1, 1);
        m_ladderqueue->enqueue(entry);
    }

    entry_t *oldE = m_ladderqueue->dequeue();

    for (int i = 0; i < 9; ++i) {
        entry_t *newE = m_ladderqueue->dequeue();
        CPPUNIT_ASSERT((double) oldE->arrival <= (double) newE->arrival);
        delete oldE;
        oldE = newE;
    }
}

void LadderQueueTest::testEnqueueVeryLarge()
{
    for (int i = 2000000; i > 0; --i) {
        entry_t *entry = new entry_t((double) i, 0.0, 1, 1, 1);
        m_ladderqueue->enqueue(entry);
    }

    entry_t *oldE = m_ladderqueue->dequeue();

    for (int i = 0; i < 1999999; ++i) {
        entry_t *newE = m_ladderqueue->dequeue();
        CPPUNIT_ASSERT((double) oldE->arrival <= (double) newE->arrival);
        delete oldE;
        oldE = newE;
    }
}

void LadderQueueTest::testEnqueueDequeue20()
{
    for (int i = 10; i > 0; --i) {
        entry_t *entry = new entry_t((double) i, 0.0, 1, 1, 1);
        m_ladderqueue->enqueue(entry);
    }
    for (int i = 10; i > 0; --i) {
        entry_t *entry = new entry_t(100.0 + (double) i, 0.0, 1, 1, 1);
        m_ladderqueue->enqueue(entry);
    }

    entry_t *oldE = m_ladderqueue->dequeue();

    for (int i = 0; i < 9; ++i) {
        entry_t *newE = m_ladderqueue->dequeue();

        CPPUNIT_ASSERT(oldE->arrival <= newE->arrival);
        delete oldE;
        oldE = newE;
    }

    for (int i = 1; i < 11; ++i) {
        try {
            entry_t *entry = new entry_t((double) i, 0.0, 1, 1, 1);
            m_ladderqueue->enqueue(entry);
        } catch (QueueException qe) {
            // we expect those here
        }
    }

    for (int i = 0; i < 11; ++i) {
        entry_t *newE = m_ladderqueue->dequeue();
        CPPUNIT_ASSERT(oldE->arrival <= newE->arrival);
        delete oldE;
        oldE = newE;
    }
}
