# Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@gmail.com>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

bin_PROGRAMS = nnet_gaussian

nnet_gaussian_SOURCES =                                               \
	CL.cc                                                         \
	nnet_gaussian.cc

nnet_gaussian_CPPFLAGS =                                              \
	$(BOOST_CPPFLAGS)                                             \
	$(OPENMP_CXXFLAGS)                                            \
	$(GSL_CFLAGS)                                                 \
	-I$(top_srcdir)/src/utils/vector/includes                     \
	-I$(top_srcdir)/src/sampling/crn/includes                     \
	-I$(top_srcdir)/src/design/pattern/includes                   \
	-I$(top_srcdir)/src/sampling/common/includes                  \
	-I./includes


nnet_gaussian_LDADD =                                                 \
	$(BOOST_PROGRAM_OPTIONS_LIB)                                  \
	$(top_srcdir)/src/sampling/crn/libsamplingcrn.la              \
	$(top_srcdir)/src/utils/vector/libutils_vector.la             \
	$(top_srcdir)/src/sampling/common/libsamplingcommon.la        \
	$(GSL_LIBS)

nnet_gaussian_LDFLAGS =                                               \
	$(BOOST_LDFLAGS)


include_HEADERS = includes
MAINTAINERCLEANFILES = Makefile.in

include $(top_srcdir)/mk/mpi.am