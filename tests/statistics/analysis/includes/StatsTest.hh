// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __STATSTEST_HH__
#define __STATSTEST_HH__


#include <cppunit/extensions/HelperMacros.h>

#include "Stats.hh"
using des::statistics::Stats;



class StatsTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(StatsTest);
    CPPUNIT_TEST(testMean);
    CPPUNIT_TEST(testSd);
    CPPUNIT_TEST_SUITE_END();

public:
    void testMean();
    void testSd();

};


#endif
