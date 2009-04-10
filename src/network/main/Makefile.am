# Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

bin_PROGRAMS = network_add_vertices wevonet_main

network_add_vertices_SOURCES =                                        \
	network_add_vertices.cc

network_add_vertices_CPPFLAGS =                                       \
	-I./includes                                                  \
	$(BOOST_CPPFLAGS)


network_add_vertices_LDADD =                                          \
	$(BOOST_LDFLAGS)


wevonet_main_SOURCES =                                                \
	wevonet_main.cc

wevonet_main_CPPFLAGS =                                               \
	-I$(top_srcdir)/src/network/evolution/includes                \
	-I$(top_srcdir)/src/sampling/crn/includes                     \
	-I$(top_srcdir)/src/design/pattern/includes                   \
	-I$(top_srcdir)/src/sampling/common/includes                  \
	$(BOOST_CPPFLAGS)                                             \
	$(OPENMP_CXXFLAGS)                                            \
	$(EXPAT_CFLAGS)


wevonet_main_LDADD =                                                  \
	$(top_srcdir)/src/network/evolution/libnetevo.la              \
	$(top_srcdir)/src/sampling/crn/libsamplingcrn.la              \
	$(top_srcdir)/src/sampling/common/libsamplingcommon.la        \
	$(BOOST_GRAPH_LIB)                                            \
	$(BOOST_PROGRAM_OPTIONS_LIB)                                  \
	$(BOOST_FILESYSTEM_LIB)                                       \
	$(BOOST_SYSTEM_LIB)                                           \
	$(BOOST_LDFLAGS)                                              \
	$(OPENMP_CXXFLAGS)                                            \
	$(EXPAT_LDFLAGS)


MAINTAINERCLEANFILES = Makefile.in
