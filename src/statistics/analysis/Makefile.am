# Copyright (C) 2007, 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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
	Stats.cc

libstatisticsanalysis_la_CPPFLAGS = -I./includes

libstatisticsanalysis_la_LIBADD =                                     \
	-lgsl                                                         \
	-lgslcblas                                                    \
	-lm

include_HEADERS = includes
MAINTAINERCLEANFILES = Makefile.in
