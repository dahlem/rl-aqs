# Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

bin_PROGRAMS = des_main

des_main_SOURCES =                                                    \
	CL.cc                                                         \
	des_main.cc

des_main_CPPFLAGS =                                                   \
	$(BOOST_CPPFLAGS)                                             \
	-I./includes                                                  \
	-I$(top_srcdir)/src/core/event/includes                       \
	-I$(top_srcdir)/src/network/evolution/includes                \
	-I$(top_srcdir)/src/sampling/crn/includes                     \
	-I$(top_srcdir)/src/design/pattern/includes                   \
	-I$(top_srcdir)/src/sampling/common/includes                  \
	-I$(top_srcdir)/src/common/queue/includes                     \
	-I$(top_srcdir)/src/io/includes                               \
	-I$(top_srcdir)/src/common/list/includes


des_main_LDADD =                                                      \
	$(BOOST_LDFLAGS)                                              \
	$(BOOST_PROGRAM_OPTIONS_LIB)                                  \
	$(BOOST_FILESYSTEM_LIB)                                       \
	$(BOOST_SYSTEM_LIB)                                           \
	-lboost_graph                                                 \
	-lgsl                                                         \
	-lm                                                           \
	$(top_srcdir)/src/sampling/crn/libsamplingcrn.la              \
	$(top_srcdir)/src/sampling/common/libsamplingcommon.la        \
	$(top_srcdir)/src/common/queue/libcommonqueue.la              \
	$(top_srcdir)/src/common/list/libcommonlist.la                \
	$(top_srcdir)/src/core/event/libcoreevent.la


include_HEADERS = includes
MAINTAINERCLEANFILES = Makefile.in
