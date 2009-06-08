// Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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
#include "ConjugateGradient.hh"
#include "FeedforwardNetwork.hh"
#include "Identity.hh"
#include "HTangent.hh"
#include "Logistic.hh"
#include "MSE.hh"
#include "Statistics.hh"
#include "DefaultLossPolicy.hh"
#include "SlidingWindowLossPolicy.hh"
namespace dnnet = des::nnet;


typedef dnnet::FeedforwardNetwork <dnnet::HTangent, dnnet::Identity> FFNet;
typedef boost::shared_ptr <FFNet> FFNetSP;

typedef dnnet::MSE <dnnet::SlidingWindowLossPolicy<50>, FFNetSP, dnnet::HTangent, dnnet::Identity> ObjMse;
typedef boost::shared_ptr <ObjMse> ObjMseSP;

typedef dnnet::Backpropagation <FFNetSP, ObjMseSP> BackProp;
typedef boost::shared_ptr <BackProp> BackPropSP;

typedef dnnet::ConjugateGradient <FFNetSP, ObjMseSP> ConjGrad;
typedef boost::shared_ptr <ConjGrad> ConjGradSP;



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
        DoubleSA target = DoubleSA(new double[1]);
        DoubleSA output = DoubleSA(new double[1]);

        out << "x,y,error" << std::endl;
        for (boost::uint16_t i = 0; i <= 600; ++i) {
            input[0] = start + i * step;
            output[0] = p_net->present(input)[0];

            target[0] = gaussian(input[0]);
            out << input[0] << "," << output[0]
                << "," << dnnet::Statistics::error(target, output, p_net->getNumOutputs()) << std::endl;
        }

        out.close();
    }
}


void printData(DoubleSA data)
{
    std::ofstream out("dataPoints.dat", std::ios::out);

    double step = 0.5;
    double start = -2.5;

    if (out.is_open()) {
        out << "x,y" << std::endl;
        for (boost::uint16_t i = 0; i <= 10; ++i) {
            data[0] = start + i * step;
            out << data[0] << "," << gaussian(data[0]) << std::endl;
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
    dsample::tGslRngSP seeds_rng = dsample::CRN::getInstance().get(seeds_rng_index);
    seed = gsl_rng_uniform_int(seeds_rng.get(), gsl_rng_max(seeds_rng.get()));
    uniform_rng_index = dsample::CRN::getInstance().init(seed);
    dsample::CRN::getInstance().log(seed, "uniform weight assignment seed");

    FFNetSP net = FFNetSP(new FFNet(1, 4, 1, uniform_rng_index));
    ObjMseSP mse = ObjMseSP(new ObjMse(net));

    // training
    // validation in the range of [-2.5; 2.5]
    double step = 0.5;
    double start = -2.5;
    DoubleSA data = DoubleSA(new double[1]);
    DoubleSA target = DoubleSA(new double[1]);

    printData(data);

    if (nnetArgs->cg) {
        ConjGradSP conjgrad = ConjGradSP(
            new ConjGrad(net, mse, nnetArgs->learning_rate, 1e-6, nnetArgs->iterations));

        for (boost::uint16_t l = 0; l < nnetArgs->epochs; ++l) {
            for (boost::uint16_t i = 0; i <= 10; ++i) {
                data[0] = start + i * step;
                target[0] = gaussian(data[0]);

                net->present(data);
                conjgrad->train(target);
            }
        }
    } else {
        BackPropSP backprop = BackPropSP(
            new BackProp(net, mse, nnetArgs->learning_rate, nnetArgs->momentum, 1e-6));

        for (boost::uint16_t l = 0; l < nnetArgs->epochs; ++l) {
            for (boost::uint16_t i = 0; i <= 10; ++i) {
                data[0] = start + i * step;
                target[0] = gaussian(data[0]);

                net->present(data);
                backprop->train(target);
            }
        }
    }

    // validation in the range of [-2.5; 2.5]
    printSample(net, nnetArgs);

    return EXIT_SUCCESS;
}
