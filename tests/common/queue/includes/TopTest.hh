#ifndef TOPTEST_H
#define TOPTEST_H


#include <cppunit/extensions/HelperMacros.h>

#include "Top.hh"


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
