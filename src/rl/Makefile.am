# Copyright (C) 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

noinst_LTLIBRARIES = librl.la

librl_la_SOURCES =                                                      \
	EpsilonGreedy.cc                                                \
	RandomSelection.cc                                              \
	OnPolicySelection.cc

librl_la_CPPFLAGS =                                                     \
	$(BOOST_CPPFLAGS)                                               \
	$(GSL_CFLAGS)                                                   \
	-I$(top_srcdir)/src/sampling/crn/includes                       \
	-I$(top_srcdir)/src/design/pattern/includes                     \
	-I$(top_srcdir)/src/sampling/common/includes                    \
	-I$(top_srcdir)/src/network/evolution/includes                  \
	-I./includes

librl_la_LIBADD =                                                       \
	$(BOOST_LDFLAGS)                                                \
	$(GSL_LIBS)                                                     \
	$(top_srcdir)/src/sampling/common/libsamplingcommon.la

include_HEADERS = includes
MAINTAINERCLEANFILES = Makefile.in

include $(top_srcdir)/mk/mpi.am
