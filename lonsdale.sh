./configure LIBS="-lpmi" CPPFLAGS="-DMPICH_IGNORE_CXX_SEEK -I/cvos/shared/apps/gcc/64/4.4.0/include -I/cvos/shared/apps/gsl/1.9/include -I/cvos/shared/apps/boost/gcc/64/1.37/include/boost-1_37" LDFLAGS="-Wl,--rpath -Wl,/cvos/shared/apps/gcc/64/4.4.0/lib64 -L/cvos/shared/apps/gcc/64/4.4.0/lib64"  --with-boost=/cvos/shared/apps/boost/gcc/64/1.37/ --with-gsl-prefix=/cvos/shared/apps/gsl/1.9  --with-expat-lib=/usr/lib64/libexpat.so --with-expat=/usr/lib64/ --enable-mpi --enable-debug