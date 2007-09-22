// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef TOPTEST_H
#define TOPTEST_H


#include <cppunit/extensions/HelperMacros.h>

#include "Top.hh"
using des::common::Top;



class TopTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TopTest);
    CPPUNIT_TEST(testInitial);
    CPPUNIT_TEST(testEnqueueOnce);
    CPPUNIT_TEST(testEnqueueTwice);
    CPPUNIT_TEST(testResetOk);
    CPPUNIT_TEST(testResetThrows);
    CPPUNIT_TEST(testList);
    CPPUNIT_TEST(test100);
    CPPUNIT_TEST(test100EnDe);
    CPPUNIT_TEST(testEnlist);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testInitial();
    void testEnqueueOnce();
    void testEnqueueTwice();
    void testResetOk();
    void testResetThrows();
    void testList();
    void test100();
    void test100EnDe();
    void testEnlist();

private:
    Top *m_top;
};


#endif
