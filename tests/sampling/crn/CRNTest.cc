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
