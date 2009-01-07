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

/** @file LhsUtils.hh
 * This header declares the utility methods for LHS sampling in the context of
 * des simulations.
 *
 * @author Dominik Dahlem
 */
#ifndef __DES_CORE_LHSUTILS_HH__
#define __DES_CORE_LHSUTILS_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include "CL.hh"



namespace des
{
namespace core
{


/** @class LhsUtils
 * Class providing utilities for the LHS sampling routine in the context of DES simulations.
 */
class LhsUtils
{
public:
    static int dimensions(tDesArgsSP p_desArgs);
    static int getNetSizeIndex(tDesArgsSP p_desArgs);
    static int getMaxEdgesIndex(tDesArgsSP p_desArgs);
    static int getEdgeProbIndex(tDesArgsSP p_desArgs);
    static int getEdgeBoostIndex(tDesArgsSP p_desArgs);
    static int getVertexBoostIndex(tDesArgsSP p_desArgs);

private:

    LhsUtils()
        {}

    ~LhsUtils()
        {}

    LhsUtils(const LhsUtils&)
        {}

    void operator=(const LhsUtils&)
        {}

};


}
}



#endif /* __DES_CORE_LHSUTILS_HH__ */
