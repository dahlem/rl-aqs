// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "CRNTest.hh"



// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(CRNTest);


void CRNTest::testInit() 
{
    CRN singleton = CRNSingleton::getInstance();
    int index = singleton.init(0);
    gsl_rng* rng = singleton.get(index - 1);
    
    CPPUNIT_ASSERT(rng != NULL);
}

void CRNTest::testGetException() 
{
    CRN singleton = CRNSingleton::getInstance();
    int index = singleton.init(0);
    gsl_rng* rng = singleton.get(index);
}
