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

/** @file FsUtils.cc
 * Implementation of the <code>FsUtils</code> class.
 */
#include <exception>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include "FsUtils.hh"

namespace des
{
namespace io
{


int FsUtils::directories(std::string &dir)
{
    int dir_count = 0;

    // how many simulation have been executed already?
    if (fs::is_directory(dir)) {
        fs::directory_iterator end_iter;

        for (fs::directory_iterator dir_itr(dir); dir_itr != end_iter; ++dir_itr) {
            try {
                if (fs::is_directory(dir_itr->status())) {
                    ++dir_count;
                }
            } catch (const std::exception &ex) {
                dir_count = -1;
            }
        }
    }

    return dir_count;
}


}
}
