# Copyright (C) 2007, 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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
	Bottom.cc

#	Fifo.cc                                                       \
#	LadderQueue.cc

libcommonqueue_la_CPPFLAGS =                                          \
	$(BOOST_CPPFLAGS)                                             \
	-I./includes                                                  \
	-I../list/includes

#	-I../sort/includes                                            

libcommonqueue_la_LIBADD =                                            \
	$(BOOST_LDFLAGS)                                              \
	-lm                                                           \
	../list/libcommonlist.la

#	../sort/libcommonsort.la                                      

include_HEADERS = includes
