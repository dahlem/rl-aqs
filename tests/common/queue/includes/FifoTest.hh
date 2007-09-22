// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef FIFOTEST_H
#define FIFOTEST_H


#include <cppunit/extensions/HelperMacros.h>

#include "Fifo.hh"
using des::common::Fifo;


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
