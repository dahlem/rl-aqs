#ifndef FIFOTEST_H
#define FIFOTEST_H


#include <cppunit/extensions/HelperMacros.h>

#include "Fifo.hh"


class FifoTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(FifoTest);
    CPPUNIT_TEST(testEnqueue);
    CPPUNIT_TEST(testOrder);
    CPPUNIT_TEST(testEmpty);
    CPPUNIT_TEST(testList);
    CPPUNIT_TEST(testNewList);
    CPPUNIT_TEST(testEnlist);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testEnqueue();
    void testOrder();
    void testEmpty();
    void testList();
    void testNewList();
    void testEnlist();

private:
    Fifo *m_fifo;
};


#endif
