// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef SCFTEST_H
#define SCFTEST_H


#include <cppunit/extensions/HelperMacros.h>

#include "SCF.hh"
using des::statistics::SCF;



class ScfTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(ScfTest);
    CPPUNIT_TEST(testSCFDim);
    CPPUNIT_TEST(testSCFThetaPos);
    CPPUNIT_TEST(testSCF1);
    CPPUNIT_TEST(testSCF2);
    CPPUNIT_TEST(testSCF3);
    CPPUNIT_TEST(testSCFMDim1);
    CPPUNIT_TEST(testSCFMDim2);
    CPPUNIT_TEST(testSCFMThetaPos);
    CPPUNIT_TEST(testSCFMSquare);
    CPPUNIT_TEST(testSCFMTooSmall);
    CPPUNIT_TEST(testSCFM1);
    CPPUNIT_TEST(testSCFM2);
    CPPUNIT_TEST_SUITE_END();

public:
    void testSCFDim();
    void testSCFThetaPos();
    void testSCF1();
    void testSCF2();
    void testSCF3();
    void testSCFMDim1();
    void testSCFMDim2();
    void testSCFMThetaPos();
    void testSCFMSquare();
    void testSCFMTooSmall();
    void testSCFM1();
    void testSCFM2();

};


#endif
