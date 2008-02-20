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
TESTS = SortSuite

check_PROGRAMS = $(TESTS)

SortSuite_SOURCES = \
	MergesortTest.cc \
	SortSuite.cc

SortSuite_LDFLAGS = $(CPPUNIT_LIBS) -ldl

SortSuite_CXXFLAGS = $(CPPUNIT_CFLAGS) \
	-Iincludes \
	-I$(top_srcdir)/src/test-utils/list/includes \
	-I$(top_srcdir)/src/common/sort/includes \
	-I$(top_srcdir)/src/common/list/includes 

LDADD = \
	$(top_srcdir)/src/common/sort/libcommonsort.la \
	$(top_srcdir)/src/common/list/libcommonlist.la \
	$(top_srcdir)/src/test-utils/list/libtestutilslist.la


include_HEADERS = includes
CLEANFILES += $(TESTS) *.log
