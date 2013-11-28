// Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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

/** @file OnlineStats.hh
 * This header declares the simulation method, so that it can be considered a
 * black-box simulation by the main method.
 *
 * @author Dominik Dahlem
 */
#ifndef __DES_STATISTICS_ONLINESTATS_HH__
#define __DES_STATISTICS_ONLINESTATS_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <cmath>

#include <boost/cstdint.hpp>


namespace des
{
namespace statistics
{


class OnlineStats
{
public:
    OnlineStats() : m_n(0), m_oldM(0.0), m_newM(0.0), m_oldS(0.0), m_newS(0.0) {}

    void clear();

    void push(double x);

    int getNumValues() const;

    double mean() const;

    double variance() const;

    double standardDeviation() const;

private:
    boost::uint32_t m_n;
    double m_oldM, m_newM, m_oldS, m_newS;
};

}
}


#endif /*  __DES_STATISTICS_ONLINESTATS_HH__ */
