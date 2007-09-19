# Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

AC_INIT([des],[0.0.1],[Dominik Dahlem Dominik.Dahlem@cs.tcd.ie],[des])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_HEADERS([config.h])

# place where extra autoconf macros are kept
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([dist-bzip2])

LF_CONFIGURE_CC
LF_CONFIGURE_CXX
LF_HOST_TYPE
LF_SET_WARNINGS

AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

AC_SUBST([MAINTAINERCLEANFILES],['Makefile.in configure semantic.cache'])


# CPPUnit library checks
PKG_CHECK_MODULES(CPPUNIT, cppunit >= 1.12.0)
AC_SUBST(CPPUNIT_CFLAGS)
AC_SUBST(CPPUNIT_LIBS)

# GSL library checks
AC_CHECK_LIB([m],[cos])
AC_CHECK_LIB([gslcblas],[cblas_dgemm])
AC_CHECK_LIB([gsl],[gsl_blas_dgemm])

# Boost library check
AX_BOOST_BASE([1.33.1])


# optional libraries
AC_GCOV
AC_VALGRIND


AC_CONFIG_FILES([
   Makefile
   README
   doc/Makefile
   m4/Makefile
   src/Makefile
   src/design/Makefile
   src/design/pattern/Makefile
   src/sampling/Makefile
   src/sampling/common/Makefile
   src/sampling/crn/Makefile
   src/sampling/qrn/Makefile
   src/utils/Makefile
   src/utils/numbers/Makefile
   src/test-utils/Makefile
   src/test-utils/list/Makefile
   src/common/Makefile
   src/common/list/Makefile
   src/common/sort/Makefile
   src/common/queue/Makefile
   tests/Makefile
   tests/common/Makefile
   tests/common/sort/Makefile
   tests/common/queue/Makefile
   tests/sampling/Makefile
   tests/sampling/crn/Makefile
])

AC_OUTPUT

for top_builddir in . .. ../.. $ac_auxdir $ac_auxdir/..; do
  test -f $top_builddir/configure && break
done
