# Copyright (C) 2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

noinst_LTLIBRARIES = libsamplingmfrw.la

libsamplingmfrw_la_SOURCES =                                          \
	MFRW.cc


libsamplingmfrw_la_CPPFLAGS =                                         \
	$(GSL_CFLAGS)                                                 \
	$(BOOST_CPPFLAGS)                                             \
	$(OPENMP_CXXFLAGS)                                            \
	-I$(top_srcdir)/src/sampling/common/includes                  \
	-I$(top_srcdir)/src/sampling/mfrw/includes


libsamplingmfrw_la_LIBADD =                                           \
	$(OPENMP_CXXFLAGS)                                            \
	$(MPILIBS)                                                    \
	$(GSL_LIBS)                                                   \
	$(top_srcdir)/src/sampling/common/libsamplingcommon.la

libsamplingmfrw_la_LDFLAGS =                                          \
	$(BOOST_LDFLAGS)


include_HEADERS = includes


include $(top_srcdir)/mk/mpi.am