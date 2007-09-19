#ifndef QRNTEST_H
#define QRNTEST_H


#include <cppunit/extensions/HelperMacros.h>

#include "QRNSequence.hh"



class QRNTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(QRNTest);
    CPPUNIT_TEST(testGet2D);
    CPPUNIT_TEST(testGet3DScaling);
    CPPUNIT_TEST_SUITE_END();

public:

    void testGet2D();
    void testGet3DScaling();
    
};


#endif
