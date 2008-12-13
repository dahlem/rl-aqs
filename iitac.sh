#!/bin/sh
export LD_LIBRARY_PATH=/usr/support/arch/x86_64/BOOST/1.37.0/lib:$LD_LIBRARY_PATH

./configure LDFLAGS="-L/usr/support/arch/x86_64/BOOST/1.37.0/lib -L/usr/support/arch/x86_64/GSL/gsl-1.9-gcc-3.4.6/lib" CPPFLAGS="-I/usr/support/arch/x86_64/GSL/gsl-1.9-gcc-3.4.6/include -I/usr/support/arch/x86_64/BOOST/1.37.0/include/boost-1_37" --with-boost-libdir=/usr/support/arch/x86_64/BOOST/1.37.0/lib --with-boost=/usr/support/arch/x86_64/BOOST/1.37.0 --with-gsl-prefix=/usr/support/arch/x86_64/GSL/gsl-1.9-gcc-3.4.6 --enable-mpi
