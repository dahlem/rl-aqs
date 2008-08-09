// Copyright (C) 2007, 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef CRNTEST_H
#define CRNTEST_H


#include <cppunit/extensions/HelperMacros.h>

#include "SamplingException.hh"
using des::sampling::SamplingException;


class CRNTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CRNTest);
    CPPUNIT_TEST(testInit);
    CPPUNIT_TEST(testStreamEqual);
    CPPUNIT_TEST(testStreamDifferent);
    CPPUNIT_TEST_EXCEPTION(testGetException, SamplingException);
    CPPUNIT_TEST_SUITE_END();

public:

    void testInit();
    void testStreamEqual();
    void testStreamDifferent();
    void testGetException();

};


#endif
