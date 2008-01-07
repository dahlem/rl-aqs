# Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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
	-I$(top_srcdir)/src/sampling/qrn/includes

LDADD =                                                               \
	-lgsl                                                         \
	-lm


include_HEADERS = includes
