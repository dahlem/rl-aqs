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
/** @file CurDate.hh
 * This header file specifies the class <code>CurDate</code> to
 * retrieve the current date given the format yyyymmddhhmmss.
 */
#ifndef __CURDATE_HH__
#define __CURDATE_HH__

#include <string>
using std::string;

#include "Singleton.hh"
using des::design::Singleton;


namespace des
{
    namespace date
    {


/** @class CurDate
 * This class provides access to the date string with the specified format
 * yyyymmddhhmmss.
 * This class can also be wrapped into a singleton using the <code>Singleton</code>
 * template, which allows to use this class for logging output into the
 * same a time-stamped directory while the application is running.
 *
 * @author Dominik Dahlem
 */
class CurDate
{
public:

    /** @fn CurDate()
     * Constructor which initialises the internal date string to the current date.
     */
    CurDate();
    ~CurDate();

    /** @fn string get() const
     * This function returns the formatted date string.
     *
     * @return string the formatted date string
     */
    string get() const;

private:
    string m_cur_date; /** current date string */
};

/**
 * Define a singleton type of the <code>CurDate</code> class.
 */
typedef Singleton<CurDate> CurDateSingleton;


    }
}


#endif // __CURDATE_HH__
