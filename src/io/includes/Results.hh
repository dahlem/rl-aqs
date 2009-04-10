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

/** @file Results.hh
 * This header file specifies the class <code>Results</code> to
 * print results into files.
 */
#ifndef __RESULTS_HH__
#define __RESULTS_HH__

#include <ostream>
#include <sstream>
#include <string>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file.hpp>
namespace bio = boost::iostreams;

#include <boost/shared_ptr.hpp>


namespace des
{
    namespace io
    {

/** @typedef str_buf
 * Specifies a shared pointer to the boost iostream stream_buffer
 * in file sink mode.
 */
typedef bio::stream_buffer <bio::file_sink> str_buf;

/** @typedef tOstreamSP
 * Specifies shared pointer to the std::ostream
 */
typedef boost::shared_ptr <std::ostream> tOstreamSP;


/** @typedef tStrBufSP
 * Specifies a shared pointer to the boost::iostreams::stream_buffer
 */
typedef boost::shared_ptr <str_buf> tStrBufSP;


/** @class Results
 * This class prints the results into a file within a time-stamped directory.
 *
 * @author Dominik Dahlem
 */
class Results
{
public:

    Results(std::string&, std::string&);
    ~Results();

    /** @fn string get() const
     * This function returns the formatted date string.
     *
     * @return string the formatted date string
     */
    void print(std::stringstream &line) const;

private:

    tOstreamSP os;
    tStrBufSP buf;
};


    }
}


#endif // __RESULTS_HH__
