# Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

bin_PROGRAMS = crn_test

crn_test_SOURCES =                                                    \
	crn_test.cc

crn_test_CPPFLAGS =                                                   \
	$(BOOST_CPPFLAGS)                                             \
	-I$(top_srcdir)/src/sampling/crn/includes                     \
	-I$(top_srcdir)/src/design/pattern/includes                   \
	-I$(top_srcdir)/src/sampling/common/includes


crn_test_LDADD =                                                      \
	$(BOOST_LDFLAGS)                                              \
	$(BOOST_PROGRAM_OPTIONS_LIB)                                  \
	$(top_srcdir)/src/sampling/crn/libsamplingcrn.la              \
	$(top_srcdir)/src/sampling/common/libsamplingcommon.la        \
	-lgsl                                                         \
	-lm


MAINTAINERCLEANFILES = Makefile.in

CXX = $(MPICXX)
crn_test_LDADD += $(MPILIBS)
