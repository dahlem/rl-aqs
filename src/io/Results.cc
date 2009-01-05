// Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file Results.cc
 * Implementation of the <code>Results</code> class. This class writes lines into
 * a specified results file within a time-stamped directory.
 */
#include <iostream>
#include <sstream>
#include <string>
#include <ostream>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include "Results.hh"
using des::io::Results;


Results::Results(std::string &filename, std::string &dir)
{
    std::stringstream path_str;
    std::string today;


    // create the results directory
    if (!fs::exists(dir)) {
        fs::create_directories(dir);
    }

    path_str << dir << "/" << filename;

    // create a buffer, if it exists append at the end of the file
    buf = tStrBufSP(
        new str_buf(
            path_str.str(), std::ios_base::out | std::ios_base::app));

    // create a output stream
    os = tOstreamSP(new std::ostream(buf.get()));
}


Results::~Results()
{
}


void Results::print(std::stringstream &line) const
{
    (*os.get()) << line.str() << std::endl;
    (*os.get()).flush();
}
