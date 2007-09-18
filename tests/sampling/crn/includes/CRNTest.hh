#ifndef CRNTEST_H
#define CRNTEST_H


#include <cppunit/extensions/HelperMacros.h>

#include "CRN.hh"



class CRNTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CRNTest);
    CPPUNIT_TEST(testInit);
    CPPUNIT_TEST_EXCEPTION(testGetException, SamplingException);
    CPPUNIT_TEST_SUITE_END();

public:

    void testInit();
    void testGetException();
    
};


#endif
