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
TESTS = LHSSuite

check_PROGRAMS = $(TESTS)

LHSSuite_SOURCES =                                               \
	LHSTest.cc                                               \
	LHSSuite.cc

LHSSuite_LDFLAGS = $(CPPUNIT_LIBS) -ldl $(GSL_LIBS)

LHSSuite_CXXFLAGS =                                              \
	$(CPPUNIT_CFLAGS)                                        \
	-Iincludes                                               \
	-I$(top_srcdir)/src/sampling/lhs/includes                \
	-I$(top_srcdir)/src/sampling/common/includes

LDADD =                                                          \
	$(top_srcdir)/src/sampling/lhs/libsamplinglhs.la         \
	$(top_srcdir)/src/sampling/common/libsamplingcommon.la


include_HEADERS = includes
CLEANFILES += $(TESTS) *.log
