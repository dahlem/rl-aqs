# Copyright (C) 2007, 2008 Dominik Dahlem <Dominik.Dahlem@gmail.com>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

noinst_LTLIBRARIES = libcommonsort.la

libcommonsort_la_SOURCES = Mergesort.cc

libcommonsort_la_CPPFLAGS =                                           \
	$(BOOST_CPPFLAGS)                                             \
	-I./includes                                                  \
	-I../list/includes

libcommonsort_la_LIBADD =                                             \
	$(BOOST_LDFLAGS)                                              \
	../list/libcommonlist.la


include_HEADERS = includes
MAINTAINERCLEANFILES = Makefile.in

include $(top_srcdir)/mk/mpi.am