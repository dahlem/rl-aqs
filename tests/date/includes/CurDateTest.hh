// Copyright (C) 2007, 2008 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __CURDATETEST_HH__
#define __CURDATETEST_HH__


#include <cppunit/extensions/HelperMacros.h>



class CurDateTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CurDateTest);
    CPPUNIT_TEST(testGet);
    CPPUNIT_TEST_SUITE_END();

public:
    void testGet();

};


#endif
