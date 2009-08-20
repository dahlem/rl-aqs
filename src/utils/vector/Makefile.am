# Copyright (C) 2007, 2008, 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

noinst_LTLIBRARIES = libutils_vector.la

libutils_vector_la_SOURCES = Vector.cc


libutils_vector_la_CPPFLAGS =                                           \
	$(BOOST_CPPFLAGS)                                               \
	-I./includes

libutils_vector_la_LIBADD =                                             \
	-lm

libutils_vector_la_LDFLAGS =                                            \
	$(BOOST_LDFLAGS)


include_HEADERS = includes
MAINTAINERCLEANFILES = Makefile.in

include $(top_srcdir)/mk/mpi.am