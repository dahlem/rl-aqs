# Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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
TESTS = CommonSuite

check_PROGRAMS = $(TESTS)

CommonSuite_SOURCES =                                            \
	RngTest.cc                                               \
	SeedsTest.cc                                             \
	CommonSuite.cc

CommonSuite_LDFLAGS = $(CPPUNIT_LIBS) -ldl

CommonSuite_CXXFLAGS = $(CPPUNIT_CFLAGS)                         \
	-Iincludes                                               \
	-I$(top_srcdir)/src/sampling/common/includes             \
	-I$(top_srcdir)/src/design/pattern/includes

LDADD =                                                          \
	$(top_srcdir)/src/sampling/common/libsamplingcommon.la


include_HEADERS = includes
CLEANFILES += $(TESTS) *.log
