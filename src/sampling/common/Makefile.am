# Copyright (C) 2007, 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

noinst_LTLIBRARIES = libsamplingcommon.la

libsamplingcommon_la_SOURCES =                                        \
	Rng.cc                                                        \
	SamplingException.cc                                          \
	Seeds.cc

libsamplingcommon_la_CPPFLAGS =                                       \
	$(BOOST_CPPFLAGS)                                             \
	-I./includes                                                  \
	-I$(top_srcdir)/src/design/pattern/includes

libsamplingcommon_la_LIBADD =                                         \
	$(BOOST_LDFLAGS)                                              \
	-lgsl                                                         \
	-lm


include_HEADERS = includes
