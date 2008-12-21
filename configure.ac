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
        [1.9],
        [Dominik Dahlem Dominik.Dahlem@cs.tcd.ie], 
        [des])

AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])

# place where extra autoconf macros are kept
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE


#LF_CONFIGURE_CC
LF_CONFIGURE_CXX
LF_HOST_TYPE
LF_SET_WARNINGS

AC_PROG_CXX
define([AC_LIBTOOL_LANG_CXX_CONFIG], [:])dnl
define([AC_LIBTOOL_LANG_F77_CONFIG], [:])dnl
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

# Checks for header files.
# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_LONG_LONG
AC_TYPE_SIZE_T
AC_CXX_BOOL
AC_CXX_CONST_CAST
AC_CXX_EXPLICIT
AC_CXX_EXCEPTIONS
AC_CXX_TEMPLATES
AC_CXX_HAVE_STD
AC_CXX_NAMESPACES
AC_CXX_HAVE_NUMERIC_LIMITS
AC_CXX_HAVE_STL

# Checks for library functions.
AC_HEADER_STDC
AC_CHECK_FUNCS([floor memset sqrt pow])

# GSL library checks
AX_PATH_GSL([1.9])

# Boost library check
AX_BOOST_BASE([1.35])
AX_BOOST_GRAPH
AX_BOOST_DATE_TIME
AX_BOOST_PROGRAM_OPTIONS
AX_BOOST_FILESYSTEM
AX_BOOST_SYSTEM

AX_LIB_EXPAT

CPPFLAGS="-W -Wall -Wno-long-long -pedantic ${CPPFLAGS}"

# Check for MPI
AC_ARG_ENABLE(mpi, AS_HELP_STRING(
		   [--enable-mpi], 
		   [enable MPI @<:@default=no@:>@]), 
		   [USE_MPI=$enableval], [USE_MPI=no])
if test "x$USE_MPI" = xyes; then
   AC_LANG([C++])
   ACX_MPI([], [AC_MSG_WARN([cannot configure MPI (compile in serial)])])
   SNOW=", snow"
   AC_SUBST(SNOW)
else
   MPICXX="${CXX}"
   AC_SUBST(MPICXX)
   SNOW=""
   AC_SUBST(SNOW)
fi
AM_CONDITIONAL(COMPILE_PARALLEL, test "x$USE_MPI" = xyes)


# Check for debug
AC_ARG_ENABLE(debug, AS_HELP_STRING(
		     [--enable-debug], 
		     [enable debug @<:@default=no@:>@]), 
		     [DEBUGGING=$enableval], [DEBUGGING=no])
if test "x$DEBUGGING" = xyes; then
  AC_DEFINE(BOOST_DISABLE_ASSERTS, 0, [disable boost asserts, if enabled])
else
  AC_DEFINE(NDEBUG, 1, [define debugging, if disabled])
  AC_DEFINE(BOOST_DISABLE_ASSERTS, 1, [disable boost asserts, if enabled])
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
else
  AM_CONDITIONAL(CPPUNIT_SUPPORT, false)
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
   des.pbs
   crn_test.pbs
   seeds_read.pbs
   struct_test.pbs
   read_graphml.pbs
   doc/Makefile
   m4/Makefile
   R/Makefile
   R/analysis/Makefile
   R/analysis/assortativity-analysis.R
   R/desGraph/Makefile
   R/desGraph/DESCRIPTION
   R/desSim/Makefile
   R/desSim/DESCRIPTION
   src/Makefile
   src/common/Makefile
   src/common/list/Makefile
   src/common/queue/Makefile
   src/core/Makefile
   src/core/event/Makefile
   src/core/main/Makefile
   src/date/Makefile
   src/design/Makefile
   src/design/pattern/Makefile
   src/io/Makefile
   src/mpi/Makefile
   src/mpi/test/Makefile
   src/network/Makefile
   src/network/evolution/Makefile
   src/network/main/Makefile
   src/nnet/Makefile
   src/sampling/Makefile
   src/sampling/common/Makefile
   src/sampling/crn/Makefile
   src/sampling/lhs/Makefile
   src/sampling/qrn/Makefile
   src/statistics/Makefile
   src/statistics/analysis/Makefile
   src/statistics/common/Makefile
   src/statistics/correlation/Makefile
   src/utils/Makefile
   src/utils/numbers/Makefile
   src/utils/vector/Makefile
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
   tests/utils/Makefile
   tests/utils/vector/Makefile
])


AC_OUTPUT

for top_builddir in . .. ../.. $ac_auxdir $ac_auxdir/..; do
  test -f $top_builddir/configure && break
done

AC_MSG_RESULT([

$PACKAGE $VERSION: Automatic configuration OK.

Type 'make' to compile des.

])
