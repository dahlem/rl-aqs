// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include <cmath>

#include "StatsTest.hh"



// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(StatsTest);



void StatsTest::testMean()
{
    double data[5] = {15.2, 18.1, 16.5, 18.3, 16.6};
    double means[4] = {16.65, 16.6, 17.025, 16.94};
    double x_bar = 0.0;

    for (int i = 0; i < 5; ++i) {
        x_bar = Stats::mean(i + 1, x_bar, data[i]);

        if (i > 0) {
            CPPUNIT_ASSERT_DOUBLES_EQUAL(means[i-1], x_bar, 0.001);
        }
    }
}


void StatsTest::testSd()
{
    double data[5] = {15.2, 18.1, 16.5, 18.3, 16.6};
    double means[4] = {16.65, 16.6, 17.025, 16.94};
    double sds[4] = {1.45, 1.186029792, 1.263675196, 1.142978565};
    double x_bar = 0.0;
    double ssd = 0.0;

    for (int i = 0; i < 5; ++i) {
        ssd = Stats::ssd(i + 1, x_bar, ssd, data[i]);
        x_bar = Stats::mean(i + 1, x_bar, data[i]);;

        if (i > 0) {
            CPPUNIT_ASSERT_DOUBLES_EQUAL(sds[i-1], std::sqrt(ssd / (i + 1)), 0.001);
        }
    }
}
