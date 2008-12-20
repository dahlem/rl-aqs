export LD_LIBRARY_PATH=/usr/support/arch/x86_64/MVAPICH/mvapich--0.9.8-gcc-3.4.6-pgf90/lib:/usr/support/arch/x86_64/BOOST/1.37.0/lib:/usr/support/arch/x86_64/GSL/gsl-1.9-gcc-3.4.6/lib:$LD_LIBRARY_PATH
export PATH=/usr/support/arch/x86_64/MVAPICH/mvapich--0.9.8-gcc-3.4.6-pgf90/bin:$PATH

./configure CPPFLAGS="-I/usr/support/arch/x86_64/GSL/gsl-1.9-gcc-3.4.6/include -I/usr/support/arch/x86_64/BOOST/1.37.0/include/boost-1_37" --with-boost-libdir=/usr/support/arch/x86_64/BOOST/1.37.0/lib --with-boost=/usr/support/arch/x86_64/BOOST/1.37.0 --with-gsl-prefix=/usr/support/arch/x86_64/GSL/gsl-1.9-gcc-3.4.6 --with-expat-lib=/usr/lib64/libexpat.so --enable-mpi --enable-debug
