# Copyright (C) 2008-2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
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
	LeaveEvent.cc                                                      \
	LeaveHandler.cc                                                    \
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
	NullUnprocessedEventsHandler.cc                                    \
	GenerateArrivalsHandler.cc                                         \
	GenerateArrivalsAdminHandler.cc                                    \
	ArrivalEvent.cc                                                    \
	ArrivalHandler.cc                                                  \
	DepartureEvent.cc                                                  \
	NumEventsHandler.cc                                                \
	LastEventHandler.cc                                                \
	UtilisationHandler.cc                                              \
	ExpectedAverageEventInQueueHandler.cc                              \
	LogGraphHandler.cc                                                 \
	DepartureHandler.cc                                                \
	ResponseStatsHandler.cc                                            \
	DefaultResponseHandler.cc                                          \
	ExpertNormalHandler.cc                                             \
	ExpertAbsoluteHandler.cc                                           \
	ExpertPositiveHandler.cc                                           \
	ExpertNegativeHandler.cc                                           \
	FullRLResponseHandler.cc                                           \
	HybridFullRLResponseHandler.cc                                     \
	SerialiseArrivalsHandler.cc                                        \
	SystemStatisticsHandler.cc                                         \
	RollingSystemStatsAdminHandler.cc                                  \
	CognitiveValueHandler.cc                                           \
	CoefficientVariationHandler.cc                                     \
	RLResponseHandler.cc

libcoreevent_la_CPPFLAGS =                                                 \
	$(BOOST_CPPFLAGS)                                                  \
	$(OPENMP_CXXFLAGS)                                                 \
	$(GSL_CFLAGS)                                                      \
	-I./includes                                                       \
	-I$(top_srcdir)/src/core/rl/includes                               \
	-I$(top_srcdir)/src/design/pattern/includes                        \
	-I$(top_srcdir)/src/statistics/analysis/includes                   \
	-I$(top_srcdir)/src/sampling/crn/includes                          \
	-I$(top_srcdir)/src/sampling/common/includes                       \
	-I$(top_srcdir)/src/sampling/mfrw/includes                         \
	-I$(top_srcdir)/src/common/queue/includes                          \
	-I$(top_srcdir)/src/network/evolution/includes                     \
	-I$(top_srcdir)/src/io/includes                                    \
	-I$(top_srcdir)/src/nnet/includes                                  \
	-I$(top_srcdir)/src/utils/vector/includes                          \
	-I$(top_srcdir)/src/core/mgmt/includes                             \
	-I$(top_srcdir)/src/common/list/includes

libcoreevent_la_LIBADD =                                                   \
	$(BOOST_FILESYSTEM_LIB)                                            \
	$(BOOST_SYSTEM_LIB)                                                \
	$(GSL_LIBS)                                                        \
	$(top_srcdir)/src/io/libdesio.la                                   \
	$(top_srcdir)/src/common/list/libcommonlist.la                     \
	$(top_srcdir)/src/sampling/common/libsamplingcommon.la             \
	$(top_srcdir)/src/statistics/analysis/libstatisticsanalysis.la     \
	$(top_srcdir)/src/network/evolution/libnetevo.la                   \
	$(top_srcdir)/src/core/rl/librl.la                                 \
	$(top_srcdir)/src/utils/vector/libutils_vector.la                  \
	$(top_srcdir)/src/core/mgmt/libcoremgmt.la                         \
	$(top_srcdir)/src/common/queue/libcommonqueue.la                   \
	$(top_srcdir)/src/sampling/mfrw/libsamplingmfrw.la

libcoreevent_la_LDFLAGS =                                                  \
	$(BOOST_LDFLAGS)

include_HEADERS = includes
MAINTAINERCLEANFILES = Makefile.in

include $(top_srcdir)/mk/mpi.am
