// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef LHSTEST_H
#define LHSTEST_H


#include <cppunit/extensions/HelperMacros.h>



class LHSTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(LHSTest);
    CPPUNIT_TEST(testDimDontMatch);
    CPPUNIT_TEST(testSizeTooSmall);
    CPPUNIT_TEST(testSample);
    CPPUNIT_TEST_SUITE_END();

public:

    void testDimDontMatch();
    void testSizeTooSmall();
    void testSample();
    
};


#endif
