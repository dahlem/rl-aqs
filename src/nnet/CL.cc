// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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
using des::nnet::CL;
using des::nnet::tNnetArgsSP;


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
        (CL_LEARNING_RATE.c_str(), po::value <double>()->default_value(0.001), "set the learning rate for the Neural Network Backpropagation.")
        (CL_MOMENTUM.c_str(), po::value <double>()->default_value(0.9), "set the momentum for the Neural Network Backpropagation.")
        ;

    opt_desc->add(opt_general);
    opt_desc->add(opt_app);
}


int CL::parse(int argc, char *argv[], tNnetArgsSP nnetArgs)
{
    po::variables_map vm;

    po::store(po::parse_command_line(argc, argv, (*opt_desc.get())), vm);
    po::notify(vm);


    if (vm.count(HELP)) {
        std::cout << (*opt_desc.get()) << std::endl;
        return EXIT_FAILURE;
    }

    if (vm.count(CL_LEARNING_RATE.c_str())) {
        nnetArgs->learning_rate = vm[CL_LEARNING_RATE.c_str()].as <double>();
    }
    std::cout << "Learning Rate: "
              << nnetArgs->learning_rate << "." << std::endl;

    if (vm.count(CL_MOMENTUM.c_str())) {
        nnetArgs->momentum = vm[CL_MOMENTUM.c_str()].as <double>();
    }
    std::cout << "Momentum: "
              << nnetArgs->momentum << "." << std::endl;

    std::cout << "******************************" << std::endl << std::endl;

    return EXIT_SUCCESS;
}
