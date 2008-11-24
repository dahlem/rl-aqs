# Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

noinst_LTLIBRARIES = libcoreevent.la

libcoreevent_la_SOURCES =                                                  \
	AckEvent.cc                                                        \
	AckHandler.cc                                                      \
	AdminEvent.cc                                                      \
	EventGenerator.cc                                                  \
	EventProcessor.cc                                                  \
	Event.cc                                                           \
	PreAnyEvent.cc                                                     \
	PostAnyEvent.cc                                                    \
	PostEvent.cc                                                       \
	LastArrivalEvent.cc                                                \
	ProcessedEventsHandler.cc                                          \
	UnprocessedEventsHandler.cc                                        \
	GenerateEventHandler.cc                                            \
	ArrivalEvent.cc                                                    \
	ArrivalHandler.cc                                                  \
	DepartureEvent.cc                                                  \
	NumEventsHandler.cc                                                \
	LastEventHandler.cc                                                \
	UtilisationHandler.cc                                              \
	ExpectedAverageEventInQueueHandler.cc                              \
	LogGraphHandler.cc                                                 \
	DepartureHandler.cc

libcoreevent_la_CPPFLAGS =                                                 \
	$(BOOST_CPPFLAGS)                                                  \
	-I./includes                                                       \
	-I$(top_srcdir)/src/date/includes                                  \
	-I$(top_srcdir)/src/design/pattern/includes                        \
	-I$(top_srcdir)/src/statistics/analysis/includes                   \
	-I$(top_srcdir)/src/sampling/crn/includes                          \
	-I$(top_srcdir)/src/sampling/common/includes                       \
	-I$(top_srcdir)/src/common/queue/includes                          \
	-I$(top_srcdir)/src/network/evolution/includes                     \
	-I$(top_srcdir)/src/io/includes                                    \
	-I$(top_srcdir)/src/common/list/includes

libcoreevent_la_LIBADD =                                                   \
	$(BOOST_LDFLAGS)                                                   \
	$(BOOST_FILESYSTEM_LIB)                                            \
	$(BOOST_SYSTEM_LIB)                                                \
	-lgsl                                                              \
	-lm                                                                \
	$(top_srcdir)/src/date/libdesdate.la                               \
	$(top_srcdir)/src/io/libdesio.la                                   \
	$(top_srcdir)/src/sampling/common/libsamplingcommon.la             \
	$(top_srcdir)/src/statistics/analysis/libstatisticsanalysis.la     \
	$(top_srcdir)/src/network/evolution/libnetevo.la                   \
	$(top_srcdir)/src/common/queue/libcommonqueue.la

include_HEADERS = includes
MAINTAINERCLEANFILES = Makefile.in
