// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef CITEST_H
#define CITEST_H


#include <cppunit/extensions/HelperMacros.h>

#include "CI.hh"
using des::statistics::CI;



class CITest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CITest);
    CPPUNIT_TEST(testConf);
    CPPUNIT_TEST_SUITE_END();

public:
    void testConf();

};


#endif
