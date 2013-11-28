# Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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
TESTS = StatsCorrelationSuite

check_PROGRAMS = $(TESTS)

StatsCorrelationSuite_SOURCES =                                            \
	ScfTest.cc                                                         \
	StatsCorrelationSuite.cc

StatsCorrelationSuite_LDFLAGS = $(CPPUNIT_LIBS) -ldl

StatsCorrelationSuite_CXXFLAGS =                                           \
	$(CPPUNIT_CFLAGS)                                                  \
	-Iincludes                                                         \
	-I$(top_srcdir)/src/statistics/common/includes                     \
	-I$(top_srcdir)/src/statistics/correlation/includes 

LDADD =                                                                    \
	$(top_srcdir)/src/statistics/common/libstatisticscommon.la         \
	$(top_srcdir)/src/statistics/correlation/libstatisticscorr.la


include_HEADERS = includes
CLEANFILES += $(TESTS) *.log
