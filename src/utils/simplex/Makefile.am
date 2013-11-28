# Copyright (C) 2009 Dominik Dahlem <Dominik.Dahlem@gmail.com>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

noinst_LTLIBRARIES = libutils_simplex.la

libutils_simplex_la_SOURCES = Simplex.cc

libutils_simplex_la_CPPFLAGS =                                          \
	$(BOOST_CPPFLAGS)                                               \
	$(GSL_CFLAGS)                                                   \
	-I$(top_srcdir)/src/sampling/crn/includes                       \
	-I$(top_srcdir)/src/design/pattern/includes                     \
	-I$(top_srcdir)/src/sampling/common/includes                    \
	-I./includes

libutils_simplex_la_LIBADD =                                            \
	-lm                                                             \
	$(GSL_LIBS)                                                     \
	$(top_srcdir)/src/sampling/common/libsamplingcommon.la

libutils_simplex_la_LIBFLAGS =                                          \
	$(BOOST_LDFLAGS)


include_HEADERS = includes
MAINTAINERCLEANFILES = Makefile.in

include $(top_srcdir)/mk/mpi.am
