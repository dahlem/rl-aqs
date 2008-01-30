// Copyright (C) 2007-2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "Rng.hh"
using des::sampling::Rng;


Rng::Rng() 
{}

Rng::~Rng() 
{}

void Rng::uniform(const gsl_rng *p_rng, gsl_matrix *p_mat)
{
    for (int i = 0; i < p_mat->size1; ++i) {
        for (int j = 0; j < p_mat->size2; ++j) {
            double u = gsl_rng_uniform(p_rng);
            gsl_matrix_set(p_mat, i, j, u);
        }
    }
}
