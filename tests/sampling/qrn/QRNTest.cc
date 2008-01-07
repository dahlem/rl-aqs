// Copyright (C) 2007-2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "QRNSequence.hh"
using des::sampling::QRNSequence;

#include "QRNTest.hh"



// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(QRNTest);



void QRNTest::testGet2D() 
{
    QRNSequence *sequence1 = new QRNSequence(NULL, 2);
    QRNSequence *sequence2 = new QRNSequence(NULL, 2);
    double seqMatrix1[5][2];
    double seqMatrix2[5][2];
    double scale[2] = {1.0, 1.0};

    for (int i = 0; i < 5; ++i) {
        sequence1->get(seqMatrix1[i], scale);
        sequence2->get(seqMatrix2[i], scale);
    }

    for (int j = 0; j < 5; ++j) {
        CPPUNIT_ASSERT(seqMatrix1[j][0] == seqMatrix2[j][0]);
        CPPUNIT_ASSERT(seqMatrix1[j][1] == seqMatrix2[j][1]);
    }

    delete sequence1;
    delete sequence2;
}

void QRNTest::testGet3DScaling() 
{
    QRNSequence *sequence1 = new QRNSequence(NULL, 3);
    QRNSequence *sequence2 = new QRNSequence(NULL, 3);
    double seqMatrix1[5][3];
    double seqMatrix2[5][3];
    double scale1[3] = {1.0, 2.0, 3.0};
    double scale2[3] = {1.0, 1.0, 1.0};

    for (int i = 0; i < 5; ++i) {
        sequence1->get(seqMatrix1[i], scale1);
        sequence2->get(seqMatrix2[i], scale2);
    }

    for (int j = 0; j < 5; ++j) {
        for (int k = 0; k < 3; k++) {
            CPPUNIT_ASSERT(seqMatrix1[j][k] == (seqMatrix2[j][k] * scale1[k]));
        }
    }

    delete sequence1;
    delete sequence2;
}
