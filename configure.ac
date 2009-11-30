# Copyright (C) 2007-2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

LT_PREREQ([1.5.26])
AC_PREREQ(2.61)
AC_INIT([Discrete Event Simulation for Service Interaction],
        [3.7],
        [Dominik Dahlem Dominik.Dahlem@cs.tcd.ie], 
        [des])

AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])

# place where extra autoconf macros are kept
AC_CONFIG_MACRO_DIR([m4])

AC_LANG([C++])

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
   HAVE_MPI=TRUE
   ACX_MPI([], [AC_MSG_WARN([cannot configure MPI (compile in serial)])])
   SNOW="snow"
   AC_SUBST(HAVE_MPI)
   AC_SUBST(SNOW)
else
   HAVE_MPI=FALSE
   MPICXX="${CXX}"
   AC_SUBST(MPICXX)
   SNOW=""
   AC_SUBST(HAVE_MPI)
   AC_SUBST(SNOW)
fi
AM_CONDITIONAL(COMPILE_PARALLEL, test "x$USE_MPI" = xyes)

AC_ARG_ENABLE(openmpi, AS_HELP_STRING(
		   [--enable-openmpi], 
		   [enable Open MPI @<:@default=no@:>@]), 
		   [USE_OPENMPI=$enableval], [USE_OPENMPI=no])
if test "x$USE_OPENMPI" = xyes; then
   AC_DEFINE(HAVE_OPENMPI,1,[Define if you have the MPI library.])
fi



# Check for OpenMP
AC_ARG_ENABLE(openmp, AS_HELP_STRING(
		   [--enable-openmp], 
		   [enable OpenMP @<:@default=no@:>@]), 
		   [USE_OPENMP=$enableval], [USE_OPENMP=no])
if test "x$USE_OPENMP" = xyes; then
   AC_OPENMP
   AC_DEFINE(HAVE_OPENMP, 1, [define openmp, if enabled])
fi


# Check for WPL debug
AC_ARG_ENABLE(debug-wpl, AS_HELP_STRING(
		     [--enable-debug-wpl], 
		     [enable WPL debugging @<:@default=no@:>@]), 
		     [WPLDEBUGGING=$enableval], [WPLDEBUGGING=no])
if test "x$WPLDEBUGGING" = xno; then
  AC_DEFINE(NDEBUG_WPL, 1, [define wpl debugging, if disabled])
fi

# Check for queue debug
AC_ARG_ENABLE(debug-queue, AS_HELP_STRING(
		     [--enable-debug-queue], 
		     [enable queue debugging @<:@default=no@:>@]), 
		     [QUEUEDEBUGGING=$enableval], [QUEUEDEBUGGING=no])
if test "x$QUEUEDEBUGGING" = xno; then
  AC_DEFINE(NDEBUG_QUEUE, 1, [define queue debugging, if disabled])
fi

# Check for event debug
AC_ARG_ENABLE(debug-events, AS_HELP_STRING(
		     [--enable-debug-events], 
		     [enable events debugging @<:@default=no@:>@]), 
		     [EVENTSDEBUGGING=$enableval], [EVENTSDEBUGGING=no])
if test "x$EVENTSDEBUGGING" = xno; then
  AC_DEFINE(NDEBUG_EVENTS, 1, [define event debugging, if disabled])
fi

# Check for network evo debug
AC_ARG_ENABLE(debug-network, AS_HELP_STRING(
		     [--enable-debug-network], 
		     [enable network debugging @<:@default=no@:>@]), 
		     [NETDEBUGGING=$enableval], [NETDEBUGGING=no])
if test "x$NETDEBUGGING" = xno; then
  AC_DEFINE(NDEBUG_NETWORK, 1, [define network debugging, if disabled])
fi

# Check for sampling debug
AC_ARG_ENABLE(debug-sampling, AS_HELP_STRING(
		     [--enable-debug-sampling], 
		     [enable sampling debugging @<:@default=no@:>@]), 
		     [SAMPLINGDEBUGGING=$enableval], [SAMPLINGDEBUGGING=no])
if test "x$SAMPLINGDEBUGGING" = xno; then
  AC_DEFINE(NDEBUG_SAMPLING, 1, [define sampling debugging, if disabled])
fi

# Check for debug
#  AC_DEFINE(BOOST_ENABLE_ASSERT_HANDLER, 1, [enable boost asserts])
AC_ARG_ENABLE(debug, AS_HELP_STRING(
		     [--enable-debug], 
		     [enable debug @<:@default=no@:>@]), 
		     [DEBUGGING=$enableval], [DEBUGGING=no])
if test "x$DEBUGGING" = xno; then
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
   pbs/des.pbs
   pbs/des_soc_500s_3e_1000t.pbs
   pbs/des_rand_500s_1000e_1000t.pbs
   pbs/des_soc_1000s_5e_1000t.pbs
   pbs/des_soc_5000s_5e_1000t.pbs
   pbs/allreduce_test.sh
   pbs/crn_test.pbs
   pbs/seeds_read.pbs
   pbs/struct_test.pbs
   pbs/read_graphml.pbs
   pbs/write_seeds.pbs
   pbs/des-soc-1000_3-sarsa-epsilon.pbs
   doc/Makefile
   m4/Makefile
   R/Makefile
   R/plot-diagnostics.R
   R/analysis/Makefile
   R/analysis/assortativity-analysis.R
   R/desGraph/Makefile
   R/desGraph/DESCRIPTION
   R/desInfluence/Makefile
   R/desInfluence/DESCRIPTION
   R/desStats/Makefile
   R/desStats/DESCRIPTION
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
   src/rl/Makefile
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
   src/utils/simplex/Makefile
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
