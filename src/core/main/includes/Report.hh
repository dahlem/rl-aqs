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

/** @file Report.hh
 * This header declares the interface of the report generation/output preparation.
 *
 * @author Dominik Dahlem
 */
#ifndef __DES_CORE_REPORT_HH__
#define __DES_CORE_REPORT_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include "WEvonet.hh"
namespace dnet = des::network;

#include "common.hh"


namespace des
{
namespace core
{

class Report
{
public:
    static sim_output accumResults(dnet::tGraphSP);

private:
    Report()
        {}

    Report(const Report&)
        {}

    ~Report()
        {}

    void operator=(const Report&)
        {}

    static double meanExpectedAverageNumberEvents(dnet::tGraphSP);
    static double meanDelay(dnet::tGraphSP);

};


}
}


#endif /* __DES_CORE_REPORT_HH__ */
