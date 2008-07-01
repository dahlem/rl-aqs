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
#include <iostream>
#include <sstream>
using std::istringstream;

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include "WEvonet.hh"
using des::network::WEvonet;

int main(int argc, char *argv[])
{
    int net_size;
    const gsl_rng_type * T;
    shared_ptr <gsl_rng> r1, r2;

    if (argc < 2) {
        net_size = 10;
    } else {
        istringstream first_arg(argv[1]);

        if (!(first_arg >> net_size)) {
            std::cerr << "Error! Could not read the first command-line argument." << std::endl;
        }
    }

    gsl_rng_env_setup();

    T = gsl_rng_default;
    r1 = shared_ptr <gsl_rng>(gsl_rng_alloc(T), gsl_rng_free);
    r2 = shared_ptr <gsl_rng>(gsl_rng_alloc(T), gsl_rng_free);

    // use the WEvonet class
    std::cout << "Using WEvonet..." << std::endl;

    WEvonet net(net_size, r1, r2);
    net.print("test.dot");

    return EXIT_SUCCESS;
}
