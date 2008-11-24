# Copyright (C) 2007, 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

AC_PREREQ(2.61)
AC_INIT([Discrete Event Simulation for Service Interaction],
        [1.3],
        [Dominik Dahlem Dominik.Dahlem@cs.tcd.ie], 
        [des])

AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])

# place where extra autoconf macros are kept
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([dist-bzip2])


LF_CONFIGURE_CC
LF_CONFIGURE_CXX
LF_HOST_TYPE
LF_SET_WARNINGS

AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

# Checks for header files.
# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
AC_HEADER_STDC
AC_CHECK_FUNCS([floor memset sqrt pow])


# GSL library checks
AC_CHECK_LIB([m],[cos])
AC_CHECK_LIB([gslcblas],[cblas_dgemm])
AC_CHECK_LIB([gsl],[gsl_blas_dgemm])


# Boost library check
AC_ARG_ENABLE(boost, AS_HELP_STRING(
		     [--enable-boost], 
		     [enable the Boost library @<:@default=no@:>@]), 
		     [BOOSTLIB=$enableval], [BOOSTLIB=no])

if test x$BOOSTLIB = xyes; then
  AX_BOOST_BASE([1.35])
  AX_BOOST_SYSTEM
  AX_BOOST_DATE_TIME
  AX_BOOST_FILESYSTEM
  AX_BOOST_IOSTREAMS
  AX_BOOST_PROGRAM_OPTIONS
  AM_CONDITIONAL(BOOST_SUPPORT, true)
else
  AM_CONDITIONAL(BOOST_SUPPORT, false)
fi

# Check for debug
AC_ARG_ENABLE(debug, AS_HELP_STRING(
		     [--enable-debug], 
		     [enable debug @<:@default=no@:>@]), 
		     [DEBUGGING=$enableval], [DEBUGGING=no])
if test "x$DEBUGGING" = xyes; then
  AC_DEFINE(NDEBUG, 1, [define debugging, if enabled])
  AC_DEFINE(BOOST_DISABLE_ASSERTS, 0, [define boost asserts, if enabled])
else
  AC_DEFINE(BOOST_DISABLE_ASSERTS, 1, [define boost asserts, if enabled])
fi

# cppunit library check
AC_ARG_ENABLE(test, AS_HELP_STRING(
		     [--enable-test], 
		     [enable the tests @<:@default=no@:>@]), 
		     [TESTLIB=$enableval], [TESTLIB=no])

if test x$TESTLIB = xyes; then
  # CPPUnit library checks
  PKG_CHECK_MODULES(CPPUNIT, cppunit >= 1.12.0)
  AC_SUBST(CPPUNIT_CFLAGS)
  AC_SUBST(CPPUNIT_LIBS)
  AM_CONDITIONAL(CPPUNIT_SUPPORT, true)
  CPPFLAGS="-W -Wall -pedantic -ggdb3 ${CPPFLAGS}"
else
  AM_CONDITIONAL(CPPUNIT_SUPPORT, false)
  CPPFLAGS="-W -Wall -pedantic -ggdb0 ${CPPFLAGS}"
fi


# Check for MPI
AC_ARG_ENABLE(mpi, AS_HELP_STRING(
		   [--enable-mpi], 
		   [enable MPI @<:@default=no@:>@]), 
		   [USE_MPI=$enableval], [USE_MPI=no])
if test "x$USE_MPI" = xyes; then
   HAVE_MPI=TRUE
   SNOW=", snow"
   AC_SUBST(HAVE_MPI)
   AC_SUBST(SNOW)
else
   HAVE_MPI=FALSE
   SNOW=""
   AC_SUBST(HAVE_MPI)
   AC_SUBST(SNOW)
fi


# Check for ladder stats
# can be enabled to record statistics for the ladder queue
AC_ARG_ENABLE(ladder-stats, AS_HELP_STRING(
		   [--enable-ladder-stats], 
		   [enable Ladder Queue statistics @<:@default=no@:>@]), 
		   [USE_LADDERSTATS=$enableval], [USE_LADDERSTATS=no])
if test "x$USE_LADDERSTATS" = xyes; then
   AC_DEFINE(HAVE_LADDERSTATS, 1, [Define to 1 if ladder queue statistics are requested.])]
fi

# Check for ladder timing
# can be enabled to record statistics for the ladder queue
AC_ARG_ENABLE(ladder-timing, AS_HELP_STRING(
		   [--enable-ladder-timing], 
		   [enable Ladder Queue timing @<:@default=no@:>@]), 
		   [USE_LADDERTIMING=$enableval], [USE_LADDERTIMING=no])
if test "x$USE_LADDERTIMING" = xyes; then
   AC_DEFINE(HAVE_LADDERTIMING, 1, [Define to 1 if ladder queue timings are requested.])]
fi


# optional libraries
AC_GCOV
AC_VALGRIND



# doxygen
DX_DOXYGEN_FEATURE([ON])
DX_HTML_FEATURE([ON])
DX_CHM_FEATURE([OFF])
DX_CHI_FEATURE([OFF])
DX_MAN_FEATURE([OFF])
DX_RTF_FEATURE([OFF])
DX_XML_FEATURE([OFF])
DX_PDF_FEATURE([OFF])
DX_PS_FEATURE([OFF])
DX_INIT_DOXYGEN([DES], [Doxyfile], [doc/doxygen])



AC_CONFIG_FILES([
   README
   Makefile
   doc/Makefile
   m4/Makefile
   R/Makefile
   R/desGraph/Makefile
   R/desGraph/DESCRIPTION
   R/desSim/Makefile
   R/desSim/DESCRIPTION
   R/analysis/Makefile
   R/analysis/assortativity-analysis.R
   src/Makefile
   src/date/Makefile
   src/io/Makefile
   src/design/Makefile
   src/design/pattern/Makefile
   src/sampling/Makefile
   src/sampling/common/Makefile
   src/sampling/crn/Makefile
   src/sampling/lhs/Makefile
   src/sampling/qrn/Makefile
   src/utils/Makefile
   src/utils/numbers/Makefile
   src/common/Makefile
   src/common/list/Makefile
   src/common/queue/Makefile
   src/statistics/Makefile
   src/statistics/analysis/Makefile
   src/statistics/common/Makefile
   src/statistics/correlation/Makefile
   src/network/Makefile
   src/network/evolution/Makefile
   src/network/main/Makefile
   src/core/Makefile
   src/core/mgmt/Makefile
   src/core/event/Makefile
   src/core/main/Makefile
   tests/Makefile
   tests/common/Makefile
   tests/common/queue/Makefile
   tests/date/Makefile
   tests/sampling/Makefile
   tests/sampling/common/Makefile
   tests/sampling/crn/Makefile	
   tests/sampling/lhs/Makefile	
   tests/sampling/qrn/Makefile
   tests/statistics/Makefile
   tests/statistics/analysis/Makefile
   tests/statistics/correlation/Makefile
])


AC_OUTPUT

for top_builddir in . .. ../.. $ac_auxdir $ac_auxdir/..; do
  test -f $top_builddir/configure && break
done

AC_MSG_RESULT([

$PACKAGE $VERSION: Automatic configuration OK.

Type 'make' to compile des.

])
