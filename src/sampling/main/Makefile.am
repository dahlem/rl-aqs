# Copyright (C) 2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

bin_PROGRAMS = mfrw

mfrw_SOURCES =                                                        \
	CL.cc                                                         \
	mfrw.cc

mfrw_CPPFLAGS =                                                       \
	$(BOOST_CPPFLAGS)                                             \
	$(OPENMP_CXXFLAGS)                                            \
	$(GSL_CFLAGS)                                                 \
	-I$(top_srcdir)/src/sampling/common/includes                  \
	-I$(top_srcdir)/src/sampling/mfrw/includes                    \
	-I./includes


mfrw_LDADD =                                                          \
	$(BOOST_PROGRAM_OPTIONS_LIB)                                  \
	$(top_srcdir)/src/sampling/mfrw/libsamplingmfrw.la            \
	$(top_srcdir)/src/sampling/common/libsamplingcommon.la        \
	$(GSL_LIBS)

mfrw_LDFLAGS =                                                        \
	$(BOOST_LDFLAGS)


include_HEADERS = includes
MAINTAINERCLEANFILES = Makefile.in

include $(top_srcdir)/mk/mpi.am
