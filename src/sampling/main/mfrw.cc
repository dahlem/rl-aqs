// Copyright (C) 2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This file is free software; as a spevectoral exception the author gives
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

#include <stdio.h>

#include <boost/shared_ptr.hpp>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_vector.h>

#include "CL.hh"
#include "MFRW.hh"
namespace dsample = des::sampling;


/** @typedef tGslRngSP
 * Typedef of the shared pointer to a GSL random number generator.
 */
typedef boost::shared_ptr<gsl_rng> tGslRngSP;


int main(int argc, char *argv[])
{
    dsample::tMfrwArgsSP args(new dsample::mfrwArgs_t);
    dsample::CL cl;
    FILE *out_file;

    out_file = fopen("mfrw.txt", "w");

    if (cl.parse(argc, argv, args)) {
        return EXIT_SUCCESS;
    }

    gsl_vector *walk = gsl_vector_calloc(args->T);

    const gsl_rng_type *rng_type = gsl_rng_default;
    tGslRngSP rngBin = tGslRngSP(gsl_rng_alloc(rng_type), gsl_rng_free);
    tGslRngSP rngNorm = tGslRngSP(gsl_rng_alloc(rng_type), gsl_rng_free);

    gsl_rng_set(rngBin.get(), args->seed_bin);
    gsl_rng_set(rngNorm.get(), args->seed_norm);

    dsample::MFRW::path(walk, rngBin, rngNorm, args->d0, args->a0,
                        args->b, args->lambda, args->Nc, args->T,
                        args->n0, args->nmax);

    gsl_vector_fprintf(out_file, walk, "%f");
    fclose(out_file);

    gsl_vector_free(walk);

    return EXIT_SUCCESS;
}
