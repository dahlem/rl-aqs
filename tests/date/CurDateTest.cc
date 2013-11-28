// Copyright (C) 2007-2008 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#include <iostream>
#include <string>

#include "CurDate.hh"
using des::date::CurDate;
using des::date::CurDateSingleton;

#include "CurDateTest.hh"



// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(CurDateTest);



void CurDateTest::testGet()
{
    std::string date;

    CurDate singleton = CurDateSingleton::getInstance();
    date = singleton.get();

    CPPUNIT_ASSERT(date == singleton.get());
}
