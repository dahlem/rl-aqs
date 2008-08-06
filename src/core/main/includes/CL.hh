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

/** @file CL.hh
 * Declaration of the methods for the command-line parsing of the main
 * routine for DES.
 *
 * @author Dominik Dahlem
 */
#ifndef __CL_HH__
#define __CL_HH__

#include <string>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <boost/program_options/options_description.hpp>
namespace po = boost::program_options;


namespace des { namespace core {


/**
 * const variables specifying the allowed options.
 */
const std::string STOPTIME = "stop_time";
const std::string GRAPH = "graph";
const std::string HELP = "help";


/** @typedef tOptDescSP
 * Specifies shared pointer to the boost options description
 */
typedef shared_ptr <po::options_description> tOptDescSP;


/** @struct
 * structure specifying the command line variables.
 */
struct desArgs_t {
    long stop_time;               /* stopping time of the DES */
    std::string graph_filename;   /* filename for the graph */
};

/** @typedef tDesArgsSP
 * Specifies shared pointer to the command line options
 */
typedef shared_ptr <desArgs_t> tDesArgsSP;



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
     * @param tDesArgsSP a shared pointer to the structure of the command-line
     *        arguments
     * @return either success or failure. In case of a failure then the help
     *        message was requested.
     */
    int parse(int argc, char *argv[], tDesArgsSP desArgs);

private:

    /**
     * A shared pointer to the description of the command-line arguments.
     */
    tOptDescSP opt_desc;
};


    }
}


#endif
