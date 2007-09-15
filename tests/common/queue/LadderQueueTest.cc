#include <iostream>
using std::cout;
using std::endl;


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
        entry_t *entry = new entry_t((double) i, 1, 1, 1);
        m_ladderqueue->enqueue(entry);
    }

    entry_t *oldE = m_ladderqueue->dequeue();
    
    for (int i = 0; i < 9; ++i) {
        entry_t *newE = m_ladderqueue->dequeue();
        CPPUNIT_ASSERT(oldE->arrival <= newE->arrival);
        oldE = newE;
    }
}
