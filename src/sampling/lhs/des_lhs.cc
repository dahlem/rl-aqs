// Copyright (C) 2009 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This file is free software; as a spevectoral exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#include <cmath>
#include <iostream>

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_vector.h>

#include "LHS.hh"
namespace dsample = des::sampling;


int main(int argc, char *argv[])
{
    std::cout << "Test Kronecker Product" << std::endl;
    gsl_matrix *a, *b, *kron;

    a = gsl_matrix_calloc(2, 2);
    b = gsl_matrix_calloc(2, 2);

    gsl_matrix_set_identity(a);
    gsl_matrix_set(b, 0, 0, 0);
    gsl_matrix_set(b, 0, 1, 1);
    gsl_matrix_set(b, 1, 0, 1);
    gsl_matrix_set(b, 1, 1, 0);

    dsample::LHS::kronecker(*a, *b, &kron);

    for (int i = 0; i < kron->size1; ++i) {
        for (int j = 0; j < kron->size2; ++j) {
            std::cout << gsl_matrix_get(kron, i, j) << " ";
        }
        std::cout << std::endl;
    }

    gsl_matrix_free(a);
    gsl_matrix_free(b);
    gsl_matrix_free(kron);


    std::cout << "Test the M matrix" << std::endl;
    int m = 4;
    gsl_matrix *M;
    gsl_vector *e;

    e = gsl_vector_calloc(8);
    gsl_vector_set(e, 0, 1);
    gsl_vector_set(e, 1, 2);
    gsl_vector_set(e, 2, 3);
    gsl_vector_set(e, 3, 4);
    gsl_vector_set(e, 4, 5);
    gsl_vector_set(e, 5, 6);
    gsl_vector_set(e, 6, 7);
    gsl_vector_set(e, 7, 8);

    dsample::LHS::matrixM(m, *e, &M);

    for (int i = 0; i < M->size1; ++i) {
        for (int j = 0; j < M->size2; ++j) {
            std::cout << gsl_matrix_get(M, i, j) << "\t";
        }
        std::cout << std::endl;
    }

    std::cout << "Calculate the distance vector" << std::endl;
    std::cout << "Matrix columns: " << M->size2 << std::endl;

    gsl_vector *dist;
    dsample::LHS::euclideanDistance(*M, &dist);
    gsl_vector_fprintf(stdout, dist, "%e");

    gsl_matrix_free(M);
    gsl_vector_free(e);
    gsl_vector_free(dist);

    std::cout << "Test the S matrix" << std::endl;
    gsl_matrix *S;

    dsample::LHS::matrixS(m, &S);

    for (int i = 0; i < S->size1; ++i) {
        for (int j = 0; j < S->size2; ++j) {
            std::cout << gsl_matrix_get(S, i, j) << "\t";
        }
        std::cout << std::endl;
    }

    gsl_matrix_free(S);

    std::cout << "Test the oLHS" << std::endl;
    gsl_vector *min, *max;
    gsl_matrix *mat;
    int size = 4;
    int r = 100;

    const gsl_rng_type *T;
    gsl_rng *rng;

    gsl_rng_env_setup();

    T = gsl_rng_default;
    rng = gsl_rng_alloc(T);

    min = gsl_vector_calloc(2);
    max = gsl_vector_calloc(2);
    gsl_vector_set(min, 0, 1);
    gsl_vector_set(min, 1, 2);
    gsl_vector_set(max, 0, 2);
    gsl_vector_set(max, 1, 4);

    dsample::LHS::sample(rng, min, max, size, &mat, r);

    for (int i = 0; i < mat->size1; ++i) {
        for (int j = 0; j < mat->size2; ++j) {
            std::cout << gsl_matrix_get(mat, i, j) << "\t";
        }
        std::cout << std::endl;
    }

    gsl_matrix_free(mat);

    return EXIT_SUCCESS;
}
