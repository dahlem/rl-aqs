# Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

noinst_LTLIBRARIES = libdesdate.la

libdesdate_la_SOURCES =                                              \
	CurDate.cc

libdesdate_la_CPPFLAGS =                                             \
	-I./includes                                                 \
	-I$(top_srcdir)/src/design/pattern/includes                  \
	$(OPENMP_CXXFLAGS)                                           \
	$(BOOST_CPPFLAGS)

libdesdate_la_LIBADD =                                               \
	$(OPENMP_CXXFLAGS)                                           \
	$(BOOST_LDFLAGS)

include_HEADERS = includes
MAINTAINERCLEANFILES = Makefile.in

include $(top_srcdir)/mk/mpi.am