# Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@gmail.com>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

noinst_LTLIBRARIES = libstatisticscorr.la

libstatisticscorr_la_SOURCES = SCF.cc

libstatisticscorr_la_CPPFLAGS =                                       \
	-I./includes                                                  \
	$(BOOST_CPPFLAGS)                                             \
	-I../common/includes

libstatisticscorr_la_LIBADD =                                                               \
	../common/libstatisticscommon.la                              \
	$(GSL_LIBS)


include_HEADERS = includes
MAINTAINERCLEANFILES = Makefile.in

include $(top_srcdir)/mk/mpi.am