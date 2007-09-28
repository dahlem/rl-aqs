// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include <gsl/gsl_statistics.h>

#include "CITest.hh"



// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(CITest);



void CITest::testConf()
{
    double data[5] = {15.2, 18.1, 16.5, 18.3, 16.6};
    int replications = 3;

    while (!CI::isConfidentWithPrecision(
               data,
               replications,
               0.1,
               0.1)) {
        replications++;

        if (replications == 6) {
            break;
        }
    }

    double mean = gsl_stats_mean(data, 1, replications - 1);

    CPPUNIT_ASSERT_EQUAL(4, replications - 1);
    CPPUNIT_ASSERT_EQUAL((double) 17.025, mean);
}
