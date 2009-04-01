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

/** @file OnlineStats.cc
 * Implementation of the online (one-pass) statistics class
 *
 * @author Dominik Dahlem
 */

#include "OnlineStats.hh"

namespace des
{
namespace statistics
{


void OnlineStats::clear()
{
    m_n = 0;
}

void OnlineStats::push(double x)
{
    m_n++;

    // See Knuth TAOCP vol 2, 3rd edition, page 232
    if (m_n == 1) {
        m_oldM = x;
        m_oldS = 0.0;
    } else {
        m_newM = m_oldM + (x - m_oldM) / m_n;
        m_newS = m_oldS + (x - m_oldM) * (x - m_newM);

        // set up for next iteration
        m_oldM = m_newM;
        m_oldS = m_newS;
    }
}

int OnlineStats::getNumValues() const
{
    return m_n;
}

double OnlineStats::mean() const
{
    double mean = 0.0;

    if (m_n > 1) {
        mean = m_newM;
    } else if (m_n == 1) {
        mean = m_oldM;
    }

    return mean;
}

double OnlineStats::variance() const
{
    return ((m_n > 1) ? (m_newS / (m_n - 1)) : 0.0);
}

double OnlineStats::standardDeviation() const
{
    return sqrt(variance());
}


}
}
