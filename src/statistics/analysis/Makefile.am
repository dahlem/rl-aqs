# Copyright (C) 2007, 2008, 2009 Dominik Dahlem <Dominik.Dahlem@gmail.com>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

noinst_LTLIBRARIES = libstatisticsanalysis.la

libstatisticsanalysis_la_SOURCES =                                    \
	CI.cc                                                         \
	OnlineStats.cc                                                \
	Stats.cc

libstatisticsanalysis_la_CPPFLAGS =                                   \
	$(BOOST_CPPFLAGS)                                             \
	$(GSL_CFLAGS)                                                 \
	-I./includes

libstatisticsanalysis_la_LIBADD =                                     \
	$(BOOST_LDFLAGS)                                              \
	$(GSL_LIBS)

libstatisticsanalysis_la_LIBFLAGS =                                   \
	$(BOOST_LDFLAGS)

include_HEADERS = includes
MAINTAINERCLEANFILES = Makefile.in

include $(top_srcdir)/mk/mpi.am