// Copyright (C) 2007-2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include "CRNTest.hh"

#include "CRN.hh"
using des::sampling::CRN;
using des::sampling::CRNSingleton;



// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(CRNTest);


void CRNTest::testInit()
{
    CRN singleton = CRNSingleton::getInstance();
    int index = singleton.init(0);
    shared_ptr<gsl_rng> rng = singleton.get(index - 1);

    CPPUNIT_ASSERT(rng.get() != NULL);
}


void CRNTest::testGetException()
{
    CRN singleton = CRNSingleton::getInstance();
    int index = singleton.init(0);
    shared_ptr<gsl_rng> rng = singleton.get(index);
}


void CRNTest::testStreamEqual()
{
    CRN singleton = CRNSingleton::getInstance();
    int index1 = singleton.init(0);
    int index2 = singleton.init(0);
    shared_ptr<gsl_rng> rng1 = singleton.get(index1 - 1);
    shared_ptr<gsl_rng> rng2 = singleton.get(index2 - 1);

    for (int i = 0; i < 10; ++i) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(gsl_rng_uniform(rng1.get()),
                                     gsl_rng_uniform(rng2.get()),
                                     0.00001);
    }
}


void CRNTest::testStreamDifferent()
{
    CRN singleton = CRNSingleton::getInstance();
    int index1 = singleton.init(999);
    int index2 = singleton.init(0);
    shared_ptr<gsl_rng> rng1 = singleton.get(index1 - 1);
    shared_ptr<gsl_rng> rng2 = singleton.get(index2 - 1);

    for (int i = 0; i < 10; ++i) {
        CPPUNIT_ASSERT(gsl_rng_uniform(rng1.get()) != gsl_rng_uniform(rng2.get()));
    }
}

