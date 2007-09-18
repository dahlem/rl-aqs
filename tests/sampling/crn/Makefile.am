# Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

# Rules for the test code (use `make check` to execute)
include $(top_srcdir)/mk/valgrind.am
include $(top_srcdir)/mk/gcov.am


# unit tests
TESTS = CRNSuite

check_PROGRAMS = $(TESTS)

CRNSuite_SOURCES =                                               \
	CRNTest.cc                                               \
	CRNSuite.cc

CRNSuite_CXXFLAGS = $(CPPUNIT_CFLAGS)
CRNSuite_LDFLAGS = $(CPPUNIT_LIBS) -ldl

INCLUDES =                                                       \
	-Iincludes                                               \
	-I$(top_srcdir)/src/sampling/crn/includes                \
	-I$(top_srcdir)/src/design/pattern/includes              \
	-I$(top_srcdir)/src/sampling/common/includes

LDADD =                                                          \
	$(top_srcdir)/src/sampling/crn/libsamplingcrn.la         \
	$(top_srcdir)/src/sampling/common/libsamplingcommon.la

CLEANFILES += *.log
