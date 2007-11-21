# Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

noinst_LTLIBRARIES = libcommonqueue.la

libcommonqueue_la_SOURCES = \
	Queue.cc \
	QueueException.cc \
	Fifo.cc \
	Top.cc \
	Bottom.cc \
	Ladder.cc \
	LadderQueue.cc

libcommonqueue_la_LDFLAGS = -lm

AM_CPPFLAGS = -I./includes -I../sort/includes -I../list/includes
LIBADD = ../sort/libcommonsort.la ../list/libcommonlist.la

include_HEADERS = includes
