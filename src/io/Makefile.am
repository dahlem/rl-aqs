# Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@gmail.com>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

noinst_LTLIBRARIES = libdesio.la

libdesio_la_SOURCES =                                                \
	FsUtils.cc                                                   \
	Results.cc

libdesio_la_CPPFLAGS =                                               \
	-I./includes                                                 \
	-I$(top_srcdir)/src/design/pattern/includes                  \
	$(BOOST_CPPFLAGS)

libdesio_la_LIBADD =                                                 \
	$(BOOST_FILESYSTEM_LIB)                                      \
	$(BOOST_SYSTEM_LIB)

libdesio_la_LDFLAGS =                                                \
	$(BOOST_LDFLAGS)


include_HEADERS = includes
MAINTAINERCLEANFILES = Makefile.in

include $(top_srcdir)/mk/mpi.am