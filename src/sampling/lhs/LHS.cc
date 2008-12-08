// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include <gsl/gsl_permutation.h>
#include <gsl/gsl_randist.h>

#include "LHS.hh"
#include "Rng.hh"


namespace des
{
namespace sampling
{


LHS::LHS()
{
}

LHS::~LHS()
{
}

void LHS::sample(const gsl_rng *p_rng, const gsl_vector *p_min,
                 const gsl_vector *p_max, int p_size, gsl_matrix **p_mat)
    throw (SamplingException)
{
    int nvar;
    gsl_matrix *u;
    gsl_permutation *p;
    double temp;

    if (p_min->size != p_max->size) {
        throw SamplingException(SamplingException::DIM_DONT_MATCH);
    }
    if (p_size < 2) {
        throw SamplingException(SamplingException::SAMPLE_SIZE_TOO_SMALL);
    }

    nvar = p_max->size;

    u = gsl_matrix_alloc(p_size, nvar);
    Rng::uniform(p_rng, u);

    *p_mat = gsl_matrix_calloc(p_size, nvar);

    p = gsl_permutation_calloc(p_size);

    for (int j = 0; j < nvar; ++j) {
        gsl_ran_shuffle(p_rng, p->data, p_size, sizeof(size_t));

        for (int i = 0; i < p_size; ++i) {
            temp = (p->data[i] + 1 - gsl_matrix_get(u, i, j)) / p_size;
            gsl_matrix_set(*p_mat, i, j,
                           gsl_vector_get(p_min, j) + temp *
                           (gsl_vector_get(p_max, j) - gsl_vector_get(p_min, j)));
        }
    }

    gsl_permutation_free(p);
    gsl_matrix_free(u);
}


}
}
