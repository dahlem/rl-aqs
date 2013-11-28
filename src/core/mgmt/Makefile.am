# Copyright (C) 2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

noinst_LTLIBRARIES = libcoremgmt.la

libcoremgmt_la_SOURCES =                                                  \
	DesBus.cc                                                         \
	Arrivals.cc                                                       \
	CJYArrivals.cc                                                    \
	MgmtException.cc

libcoremgmt_la_CPPFLAGS =                                                 \
	$(GSL_CFLAGS)                                                     \
	$(BOOST_CPPFLAGS)                                                 \
	-I$(top_srcdir)/src/design/pattern/includes                       \
	-I$(top_srcdir)/src/sampling/crn/includes                         \
	-I$(top_srcdir)/src/sampling/common/includes                      \
	-I$(top_srcdir)/src/sampling/mfrw/includes                        \
	-I$(top_srcdir)/src/network/evolution/includes                    \
	-I$(top_srcdir)/src/common/queue/includes                         \
	-I./includes

libcoremgmt_la_LDFLAGS =                                                  \
	$(GSL_LIBS)                                                       \
	$(BOOST_LDFLAGS)                                                  \
	$(top_srcdir)/src/sampling/common/libsamplingcommon.la            \
	$(top_srcdir)/src/sampling/crn/libsamplingcrn.la                  \
	$(top_srcdir)/src/sampling/lhs/libsamplinglhs.la                  \
	$(top_srcdir)/src/sampling/mfrw/libsamplingmfrw.la

include_HEADERS = includes
MAINTAINERCLEANFILES = Makefile.in

include $(top_srcdir)/mk/mpi.am
