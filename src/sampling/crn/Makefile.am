# Copyright (C) 2007, 2008, 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

noinst_LTLIBRARIES = libsamplingcrn.la

libsamplingcrn_la_SOURCES =                                           \
	CRN.cc                                                        \
	Seeds.cc


libsamplingcrn_la_CPPFLAGS =                                          \
	$(GSL_CFLAGS)                                                 \
	$(BOOST_CPPFLAGS)                                             \
	$(OPENMP_CXXFLAGS)                                            \
	-I$(top_srcdir)/src/sampling/crn/includes                     \
	-I$(top_srcdir)/src/design/pattern/includes                   \
	-I$(top_srcdir)/src/sampling/common/includes


libsamplingcrn_la_LIBADD =                                            \
	$(BOOST_LDFLAGS)                                              \
	$(OPENMP_CXXFLAGS)                                            \
	$(GSL_LIBS)                                                   \
	$(top_srcdir)/src/sampling/common/libsamplingcommon.la


include_HEADERS = includes


include $(top_srcdir)/mk/mpi.am