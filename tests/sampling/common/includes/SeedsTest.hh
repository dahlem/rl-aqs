// Copyright (C) 2007, 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef SEEDSTEST_H
#define SEEDSTEST_H


#include <cppunit/extensions/HelperMacros.h>

#include "SamplingException.hh"
using des::sampling::SamplingException;


class SeedsTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(SeedsTest);
//    CPPUNIT_TEST(testInit);
//    CPPUNIT_TEST_EXCEPTION(testInitNoFile, SamplingException);
//    CPPUNIT_TEST_EXCEPTION(testGetSeedNotInit, SamplingException);
    CPPUNIT_TEST(testGetSeeds);
//    CPPUNIT_TEST_EXCEPTION(testGetSeedsNoNumber, SamplingException);
    CPPUNIT_TEST_SUITE_END();

public:

    void testInit();
    void testInitNoFile();
    void testGetSeedNotInit();
    void testGetSeeds();
    void testGetSeedsNoNumber();

};


#endif
