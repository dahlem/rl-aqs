// Copyright (C) 2009-2010 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

/** @file CL.hh
 * Declaration of the methods for the command-line parsing of the main
 * routine for DES.
 *
 * @author Dominik Dahlem
 */
#ifndef __DES_RL_BOLTZMANN_HH__
#define __DES_RL_BOLTZMANN_HH__

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */


#include <gsl/gsl_randist.h>

#include "CRN.hh"
namespace dsample = des::sampling;

#include "DesBus.hh"
namespace dcore = des::core;

#include "Policy.hh"


namespace des
{
namespace rl
{


class BoltzmannPolicy : public Policy
{
public:
    BoltzmannPolicy(dcore::DesBus&);
    ~BoltzmannPolicy()
        {}

    virtual boost::uint16_t operator() (
        boost::uint16_t p_source, tValuesVec &p_values, PAttr p_attr);

private:
    double m_tau;
    dsample::tGslRngSP m_uniform_rng;

};


}
}



#endif /* __DES_RL_BOLTZMANN_HH__ */
