// Copyright (C) 2007-2008 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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

#include "List.hh"
using des::common::node_double_t;

#include "ListHelper.hh"
using des::testutils::ListHelper;

#include "MergesortTest.hh"



// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(MergesortTest);



void MergesortTest::testMergeLists(int p_size1, int p_size2)
{
    dcommon::entry_t *e = NULL;
    dcommon::tEntrySP dummy = dcommon::tEntrySP(e);

    node_double_t *HEAD1 = new node_double_t(dummy, NULL, NULL);
    node_double_t *TAIL1 = new node_double_t(dummy, NULL, NULL);
    node_double_t *result = NULL;
    node_double_t *current = HEAD;


    for (int i = 0; i < p_size1; ++i) {
        dcommon::tEntrySP entry = dcommon::tEntrySP(new entry_t((double) i, i, 1, 0));
        node_double_t *const node = new node_double_t(entry, NULL, NULL);
        current = ListHelper::link(current, node);
        current = node;
    }

    ListHelper::link(current, TAIL);
    current = HEAD1;

    for (int i = 0; i < p_size2; ++i) {
        dcommon::tEntrySP entry = dcommon::tEntrySP(new entry_t((double) i + p_size1 + 1, 1, 1, 0));
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

        // delete the node
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
        dcommon::tEntrySP entry = dcommon::tEntrySP(new entry_t((double) i, 1, 1, 0));
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

        // delete the node
        delete deleteNode;
    }
}

void MergesortTest::setUp()
{
    dcommon::entry_t *e = NULL;
    dcommon::tEntrySP dummy = dcommon::tEntrySP(e);

    HEAD = new node_double_t(dummy, NULL, NULL);
    TAIL = new node_double_t(dummy, NULL, NULL);
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
        dcommon::tEntrySP entry = dcommon::tEntrySP(new entry_t((double) i, i, 1, 0));
        node_double_t *const node = new node_double_t(entry, NULL, NULL);
        current = ListHelper::link(current, node);
        current = node;
    }
    for (int i = 0; i < 10; ++i) {
        dcommon::tEntrySP entry = dcommon::tEntrySP(new entry_t(4.0, i + 3, 1, 0));
        node_double_t *const node = new node_double_t(entry, NULL, NULL);
        current = ListHelper::link(current, node);
        current = node;
    }
    for (int i = 0; i < 3; ++i) {
        dcommon::tEntrySP entry = dcommon::tEntrySP(new entry_t((double) i + 5.0, i + 13, 1, 0));
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
