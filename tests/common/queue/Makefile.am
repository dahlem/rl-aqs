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
TESTS = QueueSuite

check_PROGRAMS = $(TESTS)

QueueSuite_SOURCES =                                        \
	FifoTest.cc                                         \
	TopTest.cc                                          \
	BottomTest.cc                                       \
	LadderTest.cc                                       \
	LadderQueueTest.cc                                  \
	QueueSuite.cc

QueueSuite_CXXFLAGS = $(CPPUNIT_CFLAGS)
QueueSuite_LDFLAGS = $(CPPUNIT_LIBS) -ldl

INCLUDES =                                                  \
	-Iincludes                                          \
	-I$(top_srcdir)/src/common/queue/includes           \
	-I$(top_srcdir)/src/utils/numbers/includes          \
	-I$(top_srcdir)/src/common/sort/includes            \
	-I$(top_srcdir)/src/common/list/includes

LDADD =                                                     \
	$(top_srcdir)/src/common/queue/libcommonqueue.la    \
	$(top_srcdir)/src/common/list/libcommonlist.la      \
	$(top_srcdir)/src/utils/numbers/libutils_numbers.la \
	$(top_srcdir)/src/common/sort/libcommonsort.la

CLEANFILES += *.log
