# Copyright (C) 2007, 2008, 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

noinst_LTLIBRARIES = libsamplinglhs.la

libsamplinglhs_la_SOURCES = LHS.cc


libsamplinglhs_la_CPPFLAGS =                                          \
	$(BOOST_CPPFLAGS)                                             \
	$(GSL_CFLAGS)                                                 \
	-I$(top_srcdir)/src/sampling/lhs/includes                     \
	-I$(top_srcdir)/src/sampling/common/includes

libsamplinglhs_la_LIBADD =                                            \
	$(top_srcdir)/src/sampling/common/libsamplingcommon.la        \
	$(BOOST_LDFLAGS)                                              \
	$(GSL_LIBS)


bin_PROGRAMS = des_lhs

des_lhs_SOURCES =                                                     \
	LHS.cc                                                        \
	des_lhs.cc

des_lhs_CPPFLAGS =                                                    \
	-I./includes                                                  \
	-I$(top_srcdir)/src/sampling/lhs/includes                     \
	-I$(top_srcdir)/src/sampling/common/includes                  \
	$(BOOST_CPPFLAGS)                                             \
	$(GSL_CFLAGS)


des_lhs_LDADD =                                                       \
	$(top_srcdir)/src/sampling/common/libsamplingcommon.la        \
	$(BOOST_LDFLAGS)                                              \
	$(GSL_LIBS)


include_HEADERS = includes

include $(top_srcdir)/mk/mpi.am