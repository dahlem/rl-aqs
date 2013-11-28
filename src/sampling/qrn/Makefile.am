# Copyright (C) 2007, 2008 Dominik Dahlem <Dominik.Dahlem@gmail.com>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

noinst_LTLIBRARIES = libsamplingqrn.la

libsamplingqrn_la_SOURCES = QRNSequence.cc


libsamplingqrn_la_CPPFLAGS =                                          \
	$(GSL_CFLAGS)                                                 \
	-I$(top_srcdir)/src/sampling/qrn/includes

libsamplingqrn_la_LIBADD =                                            \
	$(GSL_LIBS)


include_HEADERS = includes

include $(top_srcdir)/mk/mpi.am