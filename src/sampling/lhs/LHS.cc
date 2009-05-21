// Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <cmath>
#include <boost/cstdint.hpp>

#include <gsl/gsl_blas.h>
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


void LHS::sample(const gsl_rng *p_rng, const gsl_vector *p_min,
                 const gsl_vector *p_max, int p_size, gsl_matrix **p_mat,
                 const int p_r)
    throw (SamplingException)
{
    boost::uint32_t size = 1 << (p_size - 1); // 2^(m-1)
    boost::uint32_t permutations = size * (size + 1);
    gsl_permutation *p;
    gsl_matrix *M, *Mt, *S;
    gsl_vector *e, *e1, *dist;
    double minDist;
    boost::uint32_t r1, r2;

    if (p_min->size != p_max->size) {
        throw SamplingException(SamplingException::DIM_DONT_MATCH);
    }
    if (p_size < 2) {
        throw SamplingException(SamplingException::SAMPLE_SIZE_TOO_SMALL);
    }

    *p_mat = gsl_matrix_calloc((1 << p_size) + 1, 2 * p_size - 2);

    e = gsl_vector_calloc(size);
    e1 = gsl_vector_calloc(size);

    // 1. randomly select e
    p = gsl_permutation_calloc(size);
    gsl_ran_shuffle(p_rng, p->data, size, sizeof(size_t));

    // 1.1 construct LHS
    // init the e vector
    for (boost::uint16_t i = 0; i < size; ++i) {
        gsl_vector_set(e, i, gsl_permutation_get(p, i) + 1.0);
    }

    LHS::matrixM(p_size, *e, &M);
    Mt = M;

    LHS::euclideanDistance(*M, &dist);
    minDist = gsl_vector_min(dist);
    gsl_vector_free(dist);

    for (boost::uint16_t r = 0; r < p_r; ++r) {

        // 2. Generate [2^(m-1) * (2^(m-1) + 1)] permutations of e by
        // transposing i and j
        for (boost::uint16_t i = 0; i < permutations; ++i) {
            double minDistTemp;
            gsl_vector_memcpy(e1, e);
            LHS::uniqueInts(p_rng, &r1, &r2, size);
            gsl_vector_set(e1, r1, gsl_vector_get(e, r2));
            gsl_vector_set(e1, r2, gsl_vector_get(e, r1));

            // 2.1 For each e, construct LHS and evaluate minimum distance
            LHS::matrixM(p_size, *e, &M);
            LHS::euclideanDistance(*M, &dist);
            minDistTemp = gsl_vector_min(dist);
            gsl_vector_free(dist);

            // 2.2 Remember the best LHS design
            if (minDistTemp > minDist) {
                gsl_matrix_free(Mt);
                Mt = M;
                minDist = minDistTemp;
            } else {
                gsl_matrix_free(M);
            }
        }

        // 3. Repeat step 2 with r different e
        if (gsl_permutation_next(p) == GSL_FAILURE) {
            break;
        } else {
            // copy permutation into the e vector
            for (boost::uint16_t i = 0; i < size; ++i) {
                gsl_vector_set(e, i, gsl_permutation_get(p, i) + 1.0);
            }
        }
    }

    LHS::matrixS(p_size, &S);

    gsl_matrix_view T = gsl_matrix_submatrix(*p_mat, 0, 0, size, 2 * p_size - 2);
    gsl_matrix_memcpy(&T.matrix, Mt);
    gsl_matrix_mul_elements(&T.matrix, S);

    gsl_matrix_view Tmirror = gsl_matrix_submatrix(*p_mat, size + 1, 0, size, 2 * p_size - 2);
    gsl_matrix_memcpy(&Tmirror.matrix, &T.matrix);
    gsl_matrix_scale(&Tmirror.matrix, -1.0);

    // move the design matrix into having positive values
    double constant = static_cast<double> (size);
    gsl_matrix_add_constant(*p_mat, constant);

    // scale the matrix columns to be between 0 and 1
    boost::uint32_t div = 1 << p_size; // 2^m
    double scale = 1.0/static_cast<double> (div);

    gsl_matrix_scale(*p_mat, scale);

    gsl_vector *min, *max;
    if ((*p_mat)->size2 > p_min->size) {
        // allocate vectors with matching size
        min = gsl_vector_calloc((*p_mat)->size2);
        max = gsl_vector_alloc((*p_mat)->size2);
        gsl_vector_set_all(max, 1.0);

        // set the overlapping vector elements to be equal to p_min/p_max
        // the padded values will scale the matrix to unit length
        for (boost::uint16_t i = 0; i < p_min->size; ++i) {
            gsl_vector_set(min, i, gsl_vector_get(p_min, i));
            gsl_vector_set(max, i, gsl_vector_get(p_max, i));
        }
    } else {
        min = const_cast<gsl_vector*> (p_min);
        max = const_cast<gsl_vector*> (p_max);
    }

    // create the scale matrices
    gsl_matrix *lb, *lub;
    lb = gsl_matrix_calloc((*p_mat)->size1, (*p_mat)->size2);
    lub = gsl_matrix_calloc((*p_mat)->size1, (*p_mat)->size2);

    for (boost::uint16_t i = 0; i < (*p_mat)->size1; ++i) {
        gsl_vector_view rowLB = gsl_matrix_row(lb, i);
        gsl_vector_view rowLUB = gsl_matrix_row(lub, i);

        gsl_vector_memcpy(&rowLB.vector, min);
        gsl_vector_memcpy(&rowLUB.vector, max);
    }

    gsl_matrix_sub(lub, lb);
    gsl_matrix_mul_elements(*p_mat, lub);
    gsl_matrix_add(*p_mat, lb);

    // clean-up
    if ((*p_mat)->size2 > p_min->size) {
        gsl_vector_free(min);
        gsl_vector_free(max);
    }
    gsl_permutation_free(p);
    gsl_vector_free(e);
    gsl_matrix_free(Mt);
    gsl_matrix_free(S);
    gsl_matrix_free(lb);
    gsl_matrix_free(lub);
}


void LHS::uniqueInts(const gsl_rng *p_rng, boost::uint32_t *r1, boost::uint32_t *r2,
                     boost::uint32_t maxVal)
{
    *r1 = gsl_rng_uniform_int(p_rng, maxVal);

    do {
        *r2 = gsl_rng_uniform_int(p_rng, maxVal);
    } while (*r1 != *r2);
}


void LHS::euclideanDistance(gsl_matrix &p_mat, gsl_vector **p_dist)
{
    gsl_vector *diff = gsl_vector_calloc(p_mat.size1);
    int n = p_mat.size2 - 1;
    int dim = 0.5 * n * (1 + n);
    int vec_row = 0;

    *p_dist = gsl_vector_alloc(dim);

    for (boost::uint16_t i = 0; i < (p_mat.size2 - 1); ++i) {
        for (boost::uint16_t j = (i + 1); j < p_mat.size2; ++j) {
            double dot = 0;
            gsl_vector_view mi = gsl_matrix_column(&p_mat, i);
            gsl_vector_view mj = gsl_matrix_column(&p_mat, j);

            gsl_vector_memcpy(diff, &mi.vector);
            gsl_vector_sub(diff, &mj.vector);
            gsl_blas_ddot(diff, diff, &dot);
            gsl_vector_set(*p_dist, vec_row, sqrt(dot));
            vec_row++;
        }
    }
}


void LHS::matrixM(int p_m, gsl_vector &p_e, gsl_matrix **p_M)
{
    gsl_matrix *I, *R;
    gsl_matrix **A;
    boost::uint32_t dims = 1 << (p_m - 1); // 2^(m-1)

    I = gsl_matrix_calloc(2, 2);
    R = gsl_matrix_calloc(2, 2);
    *p_M = gsl_matrix_calloc(dims, 2 * p_m - 2);

    A = new gsl_matrix*[p_m - 1];
    for (boost::uint32_t i = 0; i < p_m - 1; ++i) {
        A[i] = gsl_matrix_calloc(dims, dims);
    }

    // initialise the matrices
    gsl_matrix_set_identity(I);
    gsl_matrix_set(R, 0, 0, 0);
    gsl_matrix_set(R, 0, 1, 1);
    gsl_matrix_set(R, 1, 0, 1);
    gsl_matrix_set(R, 1, 1, 0);

    // set the e vector as the first column
    gsl_matrix_set_col(*p_M, 0, &p_e);

    for (boost::uint16_t k = 1; k <= (p_m-1); ++k) {
        gsl_matrix *t1, *t2;
        t1 = gsl_matrix_alloc(2, 2);

        for (boost::uint16_t i = 1; i <= (p_m-1-k); ++i) {
            if (i == 1) {
                gsl_matrix_set_identity(t1);
            } else {
                LHS::kronecker(*t1, *I, &t2);

                // delete t1 and assign t2 to it
                gsl_matrix_free(t1);
                t1 = t2;
            }
        }

        for (boost::uint16_t i = 1; i <= k; ++i) {
            if (((p_m - 1 - k) == 0) && (i == 1)) {
                gsl_matrix_memcpy(t1, R);
            } else {
                LHS::kronecker(*t1, *R, &t2);

                // delete t1 and assign t2 to it
                gsl_matrix_free(t1);
                t1 = t2;
            }
        }

        gsl_matrix_memcpy(A[k-1], t1);
        gsl_vector_view Mk = gsl_matrix_column(*p_M, k);
        gsl_blas_dgemv(CblasNoTrans, 1.0, t1, &p_e, 0.0, &Mk.vector);
        gsl_matrix_free(t1);
    }

    gsl_matrix *t = gsl_matrix_calloc(dims, dims);
    for (boost::uint16_t j = 0; j < (p_m - 2); ++j) {
        gsl_vector_view Mk = gsl_matrix_column(*p_M, p_m + j);
        gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, A[p_m - 2], A[j], 0.0, t);
        gsl_blas_dgemv(CblasNoTrans, 1.0, t, &p_e, 0.0, &Mk.vector);
    }
    gsl_matrix_free(t);

    // clean-up
    gsl_matrix_free(I);
    gsl_matrix_free(R);

    for (boost::uint32_t i = 0; i < p_m - 1; ++i) {
        gsl_matrix_free(A[i]);
    }

    delete [] A;
}


void LHS::matrixS(int p_m, gsl_matrix **p_S)
{
    gsl_matrix *Bk, *Bi;
    boost::uint32_t dims = 1 << (p_m - 1); // 2^(m-1)

    Bk = gsl_matrix_calloc(2, 1);
    Bi = gsl_matrix_calloc(2, 1);
    *p_S = gsl_matrix_calloc(dims, 2 * p_m - 2);

    // initialise the matrices
    gsl_matrix_set(Bk, 0, 0, -1);
    gsl_matrix_set(Bk, 1, 0, 1);
    gsl_matrix_set(Bi, 0, 0, 1);
    gsl_matrix_set(Bi, 1, 0, 1);

    // set the 1 vector as the first column
    gsl_matrix_set_all(*p_S, 1.0);

    for (boost::uint16_t k = 1; k <= (p_m-1); ++k) {
        gsl_matrix *t1, *t2;
        t1 = gsl_matrix_alloc(2, 1);

        if (k == (p_m-1)) {
            gsl_matrix_memcpy(t1, Bk);
        } else {
            gsl_matrix_memcpy(t1, Bi);
        }

        for (boost::uint16_t i = 1; i < (p_m-1); ++i) {
            if (i != (p_m - 1 - k)) {
                LHS::kronecker(*t1, *Bi, &t2);

                // delete t1 and assign t2 to it
                gsl_matrix_free(t1);
                t1 = t2;
            } else {
                LHS::kronecker(*t1, *Bk, &t2);

                // delete t1 and assign t2 to it
                gsl_matrix_free(t1);
                t1 = t2;
            }
        }

        gsl_matrix_view Sk = gsl_matrix_submatrix(*p_S, 0, k, dims, 1);
        gsl_matrix_memcpy(&Sk.matrix, t1);
        gsl_matrix_free(t1);
    }

    gsl_vector_view S1 = gsl_matrix_column(*p_S, 1);
    for (boost::uint16_t j = 0; j < (p_m - 2); ++j) {
        gsl_vector_view Sj = gsl_matrix_column(*p_S, j + 2);
        gsl_vector_view Sk = gsl_matrix_column(*p_S, p_m + j);
        gsl_vector_memcpy(&Sk.vector, &S1.vector);
        gsl_vector_mul(&Sk.vector, &Sj.vector);
    }

    // clean-up
    gsl_matrix_free(Bk);
    gsl_matrix_free(Bi);
}


void LHS::kronecker(const gsl_matrix &p_a, const gsl_matrix &p_b,
                    gsl_matrix **p_mat)
{
    boost::uint32_t rowA, rowB, colA, colB;
    rowA = p_a.size1;
    colA = p_a.size2;
    rowB = p_b.size1;
    colB = p_b.size2;

    *p_mat = gsl_matrix_calloc(rowA * rowB, colA * colB);

    // copy the matrix p_b into p_mat blocks
    for (boost::uint32_t p = 0; p < rowA; ++p) {
        for (boost::uint32_t q = 0; q < colA; ++q) {
            // create a view of p_mat representing the current block
            gsl_matrix_view block_view = gsl_matrix_submatrix(*p_mat, p * rowB, q * colB, rowB, colB);
            gsl_matrix *block = &block_view.matrix;

            // copy matrix p_b into this view
            gsl_matrix_memcpy(block, &p_b);

            // scale the matrix block by p_a(p, q)
            gsl_matrix_scale(block, gsl_matrix_get (&p_a, p, q));
        }
    }
}



}
}
