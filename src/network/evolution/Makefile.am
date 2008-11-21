# Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

noinst_LTLIBRARIES = libnetevo.la

libnetevo_la_SOURCES =                                                \
	GraphException.cc                                             \
	WEvonet.cc

libnetevo_la_CPPFLAGS =                                               \
	-I./includes                                                  \
	$(BOOST_CPPFLAGS)

libnetevo_la_LIBADD =                                                 \
	-lboost_graph                                                 \
	$(BOOST_LDFLAGS)

include_HEADERS = includes
MAINTAINERCLEANFILES = Makefile.in
