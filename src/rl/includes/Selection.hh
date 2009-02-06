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

/** @file Selection.hh
 * Declaration of an abstract selection class
 *
 * @author Dominik Dahlem
 */
#ifndef __DES_RL_SELECTION_HH__
#define __DES_RL_SELECTION_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>

#include "Policy.hh"


namespace des
{
namespace rl
{


class Selection
{
public:
    Selection(tPolicySP p_policy)
        : m_policy(p_policy)
        {}

    virtual ~Selection()
        {}

    virtual boost::int32_t operator() (boost::int32_t p_source) = 0;

protected:
    tPolicySP m_policy;

};

typedef boost::shared_ptr<Selection> tSelectionSP;


}
}



#endif /* __DES_RL_SELECTION_HH__ */
