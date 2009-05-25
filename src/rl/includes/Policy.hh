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

/** @file Policy.hh
 * Declaration of an abstract policy class
 *
 * @author Dominik Dahlem
 */
#ifndef __DES_RL_POLICY_HH__
#define __DES_RL_POLICY_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <utility>
#include <vector>

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>


namespace des
{
namespace rl
{


typedef std::pair<boost::uint16_t, double> tValues;
typedef std::vector<tValues> tValuesVec;
typedef boost::shared_ptr<tValuesVec> tValuesVecSP;


bool val_greater(tValues const& v1, tValues const& v2);


struct PAttr
{
    double tau; // boltzmann policy temperature
};

class Policy
{
public:
    Policy()
        {}

    virtual ~Policy()
        {}

    virtual boost::uint16_t operator() (
        boost::uint16_t p_source, tValuesVec &p_values, PAttr p_attr) = 0;

};

class DummyPolicy : public Policy
{
public:
    DummyPolicy()
        {}

    virtual boost::uint16_t operator() (
        boost::uint16_t p_source, tValuesVec &p_values, PAttr p_attr)
        {}
};


}
}



#endif /* __DES_RL_POLICY_HH__ */
