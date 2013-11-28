// Copyright (C) 2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

/** @file CL.cc
 * Implementation of the command-line parsing of the main routine for DES.
 *
 * @author Dominik Dahlem
 */
#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <iostream>
#include <string>

#include <boost/cstdint.hpp>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>
namespace po = boost::program_options;

#include "CL.hh"


namespace des
{
namespace sampling
{



CL::CL()
{
    opt_desc = tOptDescSP(new po::options_description());

    // Declare the supported options.
    po::options_description opt_general("General Configuration");
    opt_general.add_options()
        (HELP.c_str(), "produce help message")
        ;

    po::options_description opt_app("Application Configuration");
    opt_app.add_options()
        (CL_D0.c_str(), po::value <double>()->default_value(1),
         "amplitute of the fluctuations.")
        (CL_A0.c_str(), po::value <double>()->default_value(0.1),
         "alpha 0.")
        (CL_B.c_str(), po::value <double>()->default_value(1.3),
         "drift strength.")
        (CL_LAMBDA.c_str(), po::value <double>()->default_value(1.05),
         "intermittency of the fluctuations.")
        (CL_NC.c_str(), po::value <boost::uint16_t>()->default_value(1000),
         "memory steps.")
        (CL_T.c_str(), po::value <boost::uint16_t>()->default_value(5000),
         "time scale.")
        (CL_N0.c_str(), po::value <double>()->default_value(0.0),
         "initial value of the MFRW.")
        (CL_NMAX.c_str(), po::value <double>()->default_value(30.0),
         "upper bound of the fluctuations.")
        (CL_SEED_BIN.c_str(), po::value <boost::uint32_t>()->default_value(123),
         "seed for the binomial random numbers.")
        (CL_SEED_NORM.c_str(), po::value <boost::uint32_t>()->default_value(321),
         "seed for the normal random numbers.")
        ;

    opt_desc->add(opt_general);
    opt_desc->add(opt_app);
}


int CL::parse(int argc, char *argv[], tMfrwArgsSP mfrwArgs)
{
    po::variables_map vm;

    po::store(po::parse_command_line(argc, argv, (*opt_desc.get())), vm);
    po::notify(vm);


    if (vm.count(HELP)) {
        std::cout << (*opt_desc.get()) << std::endl;
        return EXIT_FAILURE;
    }

    if (vm.count(CL_D0.c_str())) {
        mfrwArgs->d0 = vm[CL_D0.c_str()].as <double>();
    }
    std::cout << "d0: " << mfrwArgs->d0 << "." << std::endl;

    if (vm.count(CL_A0.c_str())) {
        mfrwArgs->a0 = vm[CL_A0.c_str()].as <double>();
    }
    std::cout << "a0: " << mfrwArgs->a0 << "." << std::endl;

    if (vm.count(CL_B.c_str())) {
        mfrwArgs->b = vm[CL_B.c_str()].as <double>();
    }
    std::cout << "b: " << mfrwArgs->b << "." << std::endl;

    if (vm.count(CL_LAMBDA.c_str())) {
        mfrwArgs->lambda = vm[CL_LAMBDA.c_str()].as <double>();
    }
    std::cout << "lambda: " << mfrwArgs->lambda << "." << std::endl;

    if (vm.count(CL_NC.c_str())) {
        mfrwArgs->Nc = vm[CL_NC.c_str()].as <boost::uint16_t>();
    }
    std::cout << "Nc: " << mfrwArgs->Nc << "." << std::endl;

    if (vm.count(CL_T.c_str())) {
        mfrwArgs->T = vm[CL_T.c_str()].as <boost::uint16_t>();
    }
    std::cout << "T: " << mfrwArgs->T << "." << std::endl;

    if (vm.count(CL_N0.c_str())) {
        mfrwArgs->n0 = vm[CL_N0.c_str()].as <double>();
    }
    std::cout << "n0: " << mfrwArgs->n0 << "." << std::endl;

    if (vm.count(CL_NMAX.c_str())) {
        mfrwArgs->nmax = vm[CL_NMAX.c_str()].as <double>();
    }
    std::cout << "nmax: " << mfrwArgs->nmax << "." << std::endl;

    if (vm.count(CL_SEED_BIN.c_str())) {
        mfrwArgs->seed_bin = vm[CL_SEED_BIN.c_str()].as <boost::uint32_t>();
    }
    std::cout << "seed binomial: " << mfrwArgs->seed_bin << "." << std::endl;

    if (vm.count(CL_SEED_NORM.c_str())) {
        mfrwArgs->seed_norm = vm[CL_SEED_NORM.c_str()].as <boost::uint32_t>();
    }
    std::cout << "seed normal: " << mfrwArgs->seed_norm << "." << std::endl;

    std::cout << "******************************" << std::endl << std::endl;

    return EXIT_SUCCESS;
}


}
}
