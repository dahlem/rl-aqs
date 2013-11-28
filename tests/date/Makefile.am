# Copyright (C) 2007, 2008 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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
TESTS = CurDateSuite

check_PROGRAMS = $(TESTS)

CurDateSuite_SOURCES =                                                \
	CurDateTest.cc                                                \
	CurDateSuite.cc

CurDateSuite_LDFLAGS = $(CPPUNIT_LIBS) -ldl

CurDateSuite_CXXFLAGS = $(CPPUNIT_CFLAGS)                             \
	-Iincludes                                                    \
	-I$(top_srcdir)/src/date/includes                             \
	-I$(top_srcdir)/src/design/pattern/includes

CurDateSuite_LDADD =                                                  \
	$(top_srcdir)/src/date/libdesdate.la


include_HEADERS = includes
CLEANFILES += $(TESTS) *.log
