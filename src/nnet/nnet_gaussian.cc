// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

#include <cmath>
#include <cstring>
#include <fstream>

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>

#include <gsl/gsl_rng.h>

#include "CRN.hh"
namespace dsample = des::sampling;

#include "Backpropagation.hh"
#include "CL.hh"
#include "FeedforwardNetwork.hh"
#include "Identity.hh"
#include "HTangent.hh"
#include "Logistic.hh"
#include "MSE.hh"
namespace dnnet = des::nnet;


typedef dnnet::FeedforwardNetwork <dnnet::HTangent, dnnet::MSE, dnnet::Identity> FFNet;
typedef boost::shared_ptr <FFNet> FFNetSP;

typedef dnnet::Backpropagation <FFNetSP, dnnet::HTangent, dnnet::Identity> BackProp;
typedef boost::shared_ptr <BackProp> BackPropSP;



double gaussian(double x)
{
    return exp(- x * x);
}


void printSample(FFNetSP p_net, dnnet::tNnetArgsSP p_netArgs)
{
    std::ofstream out(p_netArgs->filename.c_str(), std::ios::out);

    if (out.is_open()) {
        double step = 6.0 / 600;
        double start = -3.0;
        DoubleSA input = DoubleSA(new double[1]);

        out << "x,y,z" << std::endl;
        for (boost::uint16_t i = 0; i <= 600; ++i) {
            input[0] = start + i * step;
            out << input[0] << "," << p_net->present(input)[0] << "," << gaussian(input[0]) << std::endl;
        }

        out.close();
    }
}


int main(int argc, char *argv[])
{
    dnnet::tNnetArgsSP nnetArgs(new dnnet::nnetArgs_t);
    boost::int32_t seeds_rng_index, uniform_rng_index;
    boost::uint32_t seed = 0;
    dnnet::CL cl;

    if (cl.parse(argc, argv, nnetArgs)) {
        return EXIT_SUCCESS;
    }

    seeds_rng_index = dsample::CRN::getInstance().init(gsl_rng_default_seed);
    dsample::CRN::getInstance().log(gsl_rng_default_seed, "seeds");

    // init the crn for the uniform weight assignment
    dsample::tGslRngSP seeds_rng = dsample::CRN::getInstance().get(seeds_rng_index - 1);
    seed = gsl_rng_uniform_int(seeds_rng.get(), gsl_rng_max(seeds_rng.get()));
    uniform_rng_index = dsample::CRN::getInstance().init(seed);
    dsample::CRN::getInstance().log(seed, "uniform weight assignment seed");

    FFNetSP net = FFNetSP(new FFNet(1, 4, 1, uniform_rng_index));
    BackPropSP backprop = BackPropSP(
        new BackProp(net, nnetArgs->learning_rate, nnetArgs->momentum));

    // training
    // validation in the range of [-2.5; 2.5]
    double step = 5.0 * 0.5;
    double start = -2.5;
    DoubleSA data = DoubleSA(new double[1]);
    DoubleSA target = DoubleSA(new double[1]);

    for (boost::uint16_t l = 0; l < nnetArgs->iterations; ++l) {
        for (boost::uint16_t i = 0; i <= 10; ++i) {
            data[0] = start + i * step;
            target[0] = gaussian(data[0]);

            net->present(data);
            backprop->train(target);
        }
    }

    // validation in the range of [-2.5; 2.5]
    printSample(net, nnetArgs);

    return EXIT_SUCCESS;
}
