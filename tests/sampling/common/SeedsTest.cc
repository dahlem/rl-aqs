// Copyright (C) 2007-2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "Seeds.hh"
using des::sampling::Seeds;
using des::sampling::SeedsSingleton;

#include "SeedsTest.hh"


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(SeedsTest);

unsigned long seeds[10] = {
    8293,
    571,
    2988,
    7637,
    4416,
    6003,
    7458,
    2110,
    5169,
    4489
};



void SeedsTest::testGetSeedNotInit() 
{
    Seeds singleton = SeedsSingleton::getInstance();
    singleton.getSeed();
}

void SeedsTest::testInitNoFile() 
{
    Seeds singleton = SeedsSingleton::getInstance();
    singleton.init("./nofile.txt");
}

void SeedsTest::testInit()
{
    Seeds singleton = SeedsSingleton::getInstance();
    singleton.init("./seeds.txt");
}

void SeedsTest::testGetSeeds()
{
    Seeds singleton = SeedsSingleton::getInstance();
    singleton.init("./seeds.txt");

    for (int i = 0; i < 10; ++i) {
        CPPUNIT_ASSERT_EQUAL(seeds[i], singleton.getSeed());
    }

    CPPUNIT_ASSERT_THROW(singleton.getSeed(), SamplingException);
    CPPUNIT_ASSERT_THROW(singleton.getSeed(), SamplingException);
}

void SeedsTest::testGetSeedsNoNumber() 
{
    Seeds singleton = SeedsSingleton::getInstance();
    singleton.init("./wrong.txt");
    singleton.getSeed();
}
