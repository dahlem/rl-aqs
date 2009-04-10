# Copyright (C) 2007-2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

noinst_LTLIBRARIES = libcommonqueue.la

libcommonqueue_la_SOURCES =                                           \
	Queue.cc                                                      \
	QueueException.cc                                             \
	Top.cc                                                        \
	Ladder.cc                                                     \
	Bottom.cc                                                     \
	LadderQueue.cc

libcommonqueue_la_CPPFLAGS =                                          \
	$(BOOST_CPPFLAGS)                                             \
	$(GSL_CFLAGS)                                                 \
	-I./includes                                                  \
	-I../list/includes

libcommonqueue_la_LIBADD =                                            \
	$(BOOST_LDFLAGS)                                              \
	$(GSL_LIBS)                                                   \
	../list/libcommonlist.la

include_HEADERS = includes

include $(top_srcdir)/mk/mpi.am