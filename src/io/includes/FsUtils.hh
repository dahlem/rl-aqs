// Copyright (C) 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file FsUtils.hh
 * This header file specifies the class <code>FsUtils</code> to
 * print results into files.
 */
#ifndef __DES_IO_UTILS_HH__
#define __DES_IO_UTILS_HH__



namespace des
{
    namespace io
    {


/** @class FsUtils
 * This class provides some helper methods for file system interaction
 *
 * @author Dominik Dahlem
 */
class FsUtils
{
public:


    /** @fn string get() const
     * This function returns the number of directories in a given directory
     *
     * @return string the parent directory
     */
    static int directories(std::string &dir);

private:
    FsUtils();
    ~FsUtils();
};


    }
}


#endif // __DES_IO_UTILS_HH__
