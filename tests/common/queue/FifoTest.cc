#include "FifoTest.hh"

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
    entry_t *const entry = new entry_t(0.0, 1, 1, 0);
    entry_t *result = NULL;
    
    m_fifo->enqueue(entry);
    result = m_fifo->dequeue();
    
    CPPUNIT_ASSERT_EQUAL(entry->arrival, result->arrival);
    CPPUNIT_ASSERT_EQUAL(entry->destination, result->destination);
    CPPUNIT_ASSERT_EQUAL(entry->origin, result->origin);
    CPPUNIT_ASSERT_EQUAL(entry->type, result->type);

    delete entry;
}

void FifoTest::testOrder()
{
    entry_t *const entry1 = new entry_t(0.0, 1, 1, 0);
    entry_t *const entry2 = new entry_t(1.0, 2, 2, 1);
    entry_t *result = NULL;
    
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

    delete entry1;
    delete entry2;
}

void FifoTest::testEmpty()
{
    entry_t *result = NULL;

    CPPUNIT_ASSERT(m_fifo->size() == 0);

    // empty dequeue
    result = m_fifo->dequeue();

    CPPUNIT_ASSERT(m_fifo->size() == 0);    
    CPPUNIT_ASSERT(result == NULL);
}

void FifoTest::testList()
{
    entry_t *const entry1 = new entry_t(0.0, 1, 1, 0);
    entry_t *const entry2 = new entry_t(1.0, 2, 2, 1);
    node_double_t *result = NULL;
    node_double_t *current = NULL;
    
    m_fifo->enqueue(entry1);
    m_fifo->enqueue(entry2);
    result = m_fifo->getList();

    CPPUNIT_ASSERT(m_fifo->size() == 0);    
    
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

void FifoTest::testNewList()
{
    entry_t *const entry1 = new entry_t(0.0, 1, 1, 0);
    entry_t *const entry2 = new entry_t(1.0, 2, 2, 1);
    entry_t *const entry3 = new entry_t(1.0, 2, 2, 1);
    entry_t *entry = NULL;
    node_double_t *result = NULL;
    
    m_fifo->enqueue(entry1);
    m_fifo->enqueue(entry2);
    result = m_fifo->getList();

    CPPUNIT_ASSERT(m_fifo->size() == 0);

    m_fifo->enqueue(entry3);
    
    CPPUNIT_ASSERT(m_fifo->size() == 1);

    entry = m_fifo->dequeue();
    
    CPPUNIT_ASSERT_EQUAL(entry3->arrival, entry->arrival);
    CPPUNIT_ASSERT_EQUAL(entry3->destination, entry->destination);
    CPPUNIT_ASSERT_EQUAL(entry3->origin, entry->origin);
    CPPUNIT_ASSERT_EQUAL(entry3->type, entry->type);
    CPPUNIT_ASSERT(m_fifo->size() == 0);

    delete entry1;
    delete entry2;
    delete entry3;

    delete result->next->next->next;
    delete result->next->next;
    delete result->next;
    delete result;
}
