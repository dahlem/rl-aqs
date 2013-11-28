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

/** @file CL.hh
 * Declaration of the methods for the command-line parsing of the main
 * routine for mfrw examples
 *
 * @author Dominik Dahlem
 */
#ifndef __MFRW_CL_HH__
#define __MFRW_CL_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <string>

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/program_options/options_description.hpp>
namespace po = boost::program_options;


namespace des { namespace sampling {


/**
 * const variables specifying the allowed options.
 */
const std::string HELP = "help";
const std::string CL_D0 = "d0";
const std::string CL_A0 = "a0";
const std::string CL_B = "b";
const std::string CL_LAMBDA = "lambda";
const std::string CL_NC = "nc";
const std::string CL_T = "T";
const std::string CL_N0 = "n0";
const std::string CL_NMAX = "nmax";
const std::string CL_SEED_BIN = "seed_bin";
const std::string CL_SEED_NORM = "seed_norm";


/** @typedef tOptDescSP
 * Specifies shared pointer to the boost options description
 */
typedef boost::shared_ptr <po::options_description> tOptDescSP;


/** @struct
 * structure specifying the command line variables.
 */
struct mfrwArgs_t {
    double d0;
    double a0;
    double b;
    double lambda;
    boost::uint16_t Nc;
    boost::uint16_t T;
    double n0;
    double nmax;
    boost::uint32_t seed_bin;
    boost::uint32_t seed_norm;
};

/** @typedef tMfrwArgsSP
 * Specifies shared pointer to the command line options
 */
typedef boost::shared_ptr <mfrwArgs_t> tMfrwArgsSP;



/** @class CL
 * This class uses the boost program-options library to parse the command-line
 * parameters for the main routine of the discrete event simulator.
 */
class CL
{
public:
    CL();

    /** @fn parse(int argc, char *argv[], tDesArgsSP desArgs);
     * Parse the command-line parameters and store the relevant information
     * in a shared pointer of a structure.
     *
     * @param int number of command-line arguments
     * @param char** the command-line arguments
     * @param tMfrwArgsSP a shared pointer to the structure of the command-line
     *        arguments
     * @return either success or failure. In case of a failure then the help
     *        message was requested.
     */
    int parse(int argc, char *argv[], tMfrwArgsSP desArgs);

private:

    /**
     * A shared pointer to the description of the command-line arguments.
     */
    tOptDescSP opt_desc;
};


    }
}


#endif
