// Copyright (C) 2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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

/** @file Arrivals.hh
 * Specification of the Arrivals interface.
 */
#ifndef __DES_CORE_MGMT_CJYARRIVALS_HH__
#define __DES_CORE_MGMT_CJYARRIVALS_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <iostream>

#include <boost/cstdint.hpp>

#include <gsl/gsl_matrix.h>

#include "DirectedGraph.hh"
namespace dnet = des::network;

#include "clconfig.hh"
#include "DesBus.hh"


namespace des {
namespace core {


class CJYArrivals : public Arrivals
{
public:
    CJYArrivals(DesBus&, std::string&, bool);
    ~CJYArrivals();

    void generate(bool);
    void serialise(boost::uint16_t, boost::uint16_t) throw (MgmtException);

private:
    std::string &m_serialisedMatrixFilename;
    bool m_readMFRW;
    
    dnet::Graph &m_graph;
    desArgs_t &m_desArgs;
    gsl_matrix *m_arrivalRates;

};



}
}


#endif
