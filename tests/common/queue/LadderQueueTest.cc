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

#include <boost/random/uniform_real.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>


#include "Entry.hh"
namespace dcommon = des::common;

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
        dcommon::Entry *entry = new dcommon::Entry((double) i, 1, 1, 1);
        m_ladderqueue->push(entry);
    }

    dcommon::Entry *oldE = m_ladderqueue->dequeue();

    for (int i = 0; i < 9; ++i) {
        dcommon::Entry *newE = m_ladderqueue->dequeue();
        CPPUNIT_ASSERT((double) oldE->arrival <= (double) newE->arrival);
        delete oldE;
        oldE = newE;
    }
}

void LadderQueueTest::testEnqueueVeryLarge()
{
    boost::mt19937 generator(42u);
    boost::uniform_real<> uni_dist(0,1);
    boost::variate_generator<boost::mt19937&, boost::uniform_real<> >
        uni(generator, uni_dist);

    for (int i = 2000000; i > 0; --i) {
        dcommon::Entry *entry = new dcommon::Entry(uni(), 1, 1, 1);
        m_ladderqueue->push(entry);
    }

    dcommon::Entry *oldE = m_ladderqueue->dequeue();

    for (int i = 0; i < 1999999; ++i) {
        dcommon::Entry *newE = m_ladderqueue->dequeue();
        CPPUNIT_ASSERT((double) oldE->arrival <= (double) newE->arrival);
        delete oldE;
        oldE = newE;
    }

    delete oldE;
}

void LadderQueueTest::testEnqueueDequeue20()
{
    for (int i = 10; i > 0; --i) {
        dcommon::Entry *entry = new dcommon::Entry((double) i, 1, 1, 1);
        m_ladderqueue->push(entry);
    }
    for (int i = 10; i > 0; --i) {
        dcommon::Entry *entry = new dcommon::Entry(100.0 + (double) i, 1, 1, 1);
        m_ladderqueue->push(entry);
    }

    dcommon::Entry *oldE = m_ladderqueue->dequeue();

    for (int i = 0; i < 9; ++i) {
        dcommon::Entry *newE = m_ladderqueue->dequeue();
        CPPUNIT_ASSERT(oldE->arrival <= newE->arrival);
        delete oldE;
        oldE = newE;
    }

    for (int i = 1; i < 10; ++i) {
        try {
            dcommon::Entry *entry = new dcommon::Entry((double) i, 1, 1, 1);
            m_ladderqueue->push(entry);
            CPPUNIT_FAIL("Expected queue exception!");
        } catch (dcommon::QueueException qe) {
            // we expect those here
        }
    }

    for (int i = 0; i < 10; ++i) {
        dcommon::Entry *newE = m_ladderqueue->dequeue();
        CPPUNIT_ASSERT(oldE->arrival <= newE->arrival);
        delete oldE;
        oldE = newE;
    }

    delete oldE;
}
