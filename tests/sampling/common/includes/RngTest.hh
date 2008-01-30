// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef RNGTEST_H
#define RNGTEST_H


#include <cppunit/extensions/HelperMacros.h>


class RngTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(RngTest);
    CPPUNIT_TEST(testUniform);
    CPPUNIT_TEST_SUITE_END();
    
public:

    void testUniform();
    
};


#endif
