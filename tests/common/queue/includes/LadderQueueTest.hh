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
