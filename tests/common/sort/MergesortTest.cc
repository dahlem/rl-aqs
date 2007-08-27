#include "ListHelper.hh"
#include "MergesortTest.hh"



// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(MergesortTest);



void MergesortTest::testMergeLists(int p_size1, int p_size2) 
{
    node_double_t *HEAD1 = new node_double_t(NULL, NULL, NULL);
    node_double_t *TAIL1 = new node_double_t(NULL, NULL, NULL);    
    node_double_t *result = NULL;
    node_double_t *current = HEAD;
    

    for (int i = 0; i < p_size1; ++i) {
        entry_t *const entry = new entry_t((double) i, i, 1, 0);
        node_double_t *const node = new node_double_t(entry, NULL, NULL);
        current = ListHelper::link(current, node);
        current = node;
    }

    ListHelper::link(current, TAIL);
    current = HEAD1;

    for (int i = 0; i < p_size2; ++i) {
        entry_t *const entry = new entry_t((double) i + p_size1 + 1, 1, 1, 0);
        node_double_t *const node = new node_double_t(entry, NULL, NULL);
        current = ListHelper::link(current, node);
        current = node;
    }

    ListHelper::link(current, TAIL1);

    result = Mergesort::merge(HEAD->next, HEAD1->next);
    
    current = result;

    testBothDirections(current, p_size1 + p_size2 - 1);

    while (current) {
        // if tail node
        if (current->data == NULL) break;

        node_double_t *deleteNode = current;

        // advance the current pointer
        current = current->next;

        // delete the data and the node
        delete deleteNode->data;
        delete deleteNode;
    }
    
    delete HEAD1;
    delete TAIL1;
}

void MergesortTest::testBothDirections(node_double_t *p_list, int p_size)
{
    node_double_t *current = p_list;
    
    // forward test
    for (int i = 0; i < p_size; ++i) {
        CPPUNIT_ASSERT(current->data->arrival < current->next->data->arrival);
        current = current->next;
    }

    // test the tail element
    current = current->next;
    CPPUNIT_ASSERT(current->next == NULL);
    CPPUNIT_ASSERT(current->data == NULL);    

    current = p_list;

    // doubly-linked test
    for (int i = 0; i < p_size; ++i) {
        CPPUNIT_ASSERT(
            current->next->previous->data->arrival
            < current->next->data->arrival);
        current = current->next;
    }

    // test the tail element
    current = current->next;
    CPPUNIT_ASSERT(current->next == NULL);
    CPPUNIT_ASSERT(current->data == NULL);    
    CPPUNIT_ASSERT(current->previous != NULL);    
}

void MergesortTest::testReverse(int p_max)
{
    node_double_t *current = HEAD;
    node_double_t *result = NULL;

    for (int i = p_max; i >= 0; --i) {
        entry_t *const entry = new entry_t((double) i, 1, 1, 0);
        node_double_t *const node = new node_double_t(entry, NULL, NULL);
        current = ListHelper::link(current, node);
        current = node;
    }

    ListHelper::link(current, TAIL);
    result = Mergesort::sort(HEAD->next);
    
    current = result;

    CPPUNIT_ASSERT(current->next != NULL);

    testBothDirections(current, p_max);

    while (current) {
        // if tail node
        if (current->data == NULL) break;

        node_double_t *deleteNode = current;

        // advance the current pointer
        current = current->next;

        // delete the data and the node
        delete deleteNode->data;
        delete deleteNode;
    }    
}

void MergesortTest::setUp()
{
    HEAD = new node_double_t(NULL, NULL, NULL);
    TAIL = new node_double_t(NULL, NULL, NULL);
}

void MergesortTest::tearDown()
{
    delete HEAD;
    delete TAIL;
}

void MergesortTest::testStability()
{
    node_double_t *current = HEAD;
    node_double_t *result = NULL;

    for (int i = 0; i < 3; ++i) {
        entry_t *const entry = new entry_t((double) i, i, 1, 0);
        node_double_t *const node = new node_double_t(entry, NULL, NULL);
        current = ListHelper::link(current, node);
        current = node;
    }
    for (int i = 0; i < 10; ++i) {
        entry_t *const entry = new entry_t(4.0, i + 3, 1, 0);
        node_double_t *const node = new node_double_t(entry, NULL, NULL);
        current = ListHelper::link(current, node);
        current = node;
    }
    for (int i = 0; i < 3; ++i) {
        entry_t *const entry = new entry_t((double) i + 5.0, i + 13, 1, 0);
        node_double_t *const node = new node_double_t(entry, NULL, NULL);
        current = ListHelper::link(current, node);
        current = node;
    }

    ListHelper::link(current, TAIL);
    result = Mergesort::sort(HEAD->next);
    
    current = result;

    for (int i = 0; i < 15; ++i) {
        CPPUNIT_ASSERT(current->data->destination < current->next->data->destination);
        current = current->next;
    }

    while (current) {
        // if tail node
        if (current->data == NULL) break;

        node_double_t *deleteNode = current;

        // advance the current pointer
        current = current->next;

        // delete the data and the node
        delete deleteNode->data;
        delete deleteNode;
    }    
}

void MergesortTest::testReverse50()
{
    testReverse(50);
}

void MergesortTest::testReverse101()
{
    testReverse(101);
}

void MergesortTest::testMergeLeftSmaller()
{
    testMergeLists(10, 18);
}

void MergesortTest::testMergeEqual()
{
    testMergeLists(10, 10);
}

void MergesortTest::testMergeRightSmaller()
{
    testMergeLists(10, 3);
}
