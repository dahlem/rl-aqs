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

/** @file Results.cc
 * Implementation of the <code>Results</code> class. This class writes lines into
 * a specified results file within a time-stamped directory.
 */
#include <iostream>

#include <sstream>
using std::stringstream;

#include <string>
using std::string;

#include <ostream>
using std::ostream;

#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
namespace bio = boost::iostreams;

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include "CurDate.hh"
using des::date::CurDate;
using des::date::CurDateSingleton;

#include "Results.hh"
using des::io::Results;


Results::Results(string filename)
{
    stringstream path_str;
    string today;

    CurDate singleton = CurDateSingleton::getInstance();
    today = singleton.get();

    path_str << "./" << today << "/";

    // create directory if it doesn't already exist
    fs::path results_path(path_str.str());

    if (!fs::exists(results_path)) {
        fs::create_directory(results_path);
    }

    path_str << filename;

    // create a buffer
    buf = tStrBufSP(new str_buf(path_str.str()));

    // create a output stream
    os = tOstreamSP(new std::ostream(buf.get()));
}


Results::~Results()
{
}


void Results::print(stringstream &line) const
{
    (*os.get()) << line.str() << std::endl;
}
