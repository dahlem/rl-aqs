// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef LADDERQUEUETEST_HH
#define LADDERQUEUETEST_HH


#include <cppunit/extensions/HelperMacros.h>

#include "LadderQueue.hh"



class LadderQueueTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(LadderQueueTest);
    CPPUNIT_TEST(testEnqueueVeryLarge);
    CPPUNIT_TEST(testEnqueue10);
    CPPUNIT_TEST(testEnqueueDequeue20);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();
    void testEnqueue10();
    void testEnqueueVeryLarge();
    void testEnqueueDequeue20();


private:
    LadderQueue *m_ladderqueue;
};



#endif
