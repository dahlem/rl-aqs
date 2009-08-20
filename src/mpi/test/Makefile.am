# Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

bin_PROGRAMS = crn_test seeds_read struct_test groups_test \
	read_graphml write_seeds

crn_test_SOURCES =                                                    \
	crn_test.cc

crn_test_CPPFLAGS =                                                   \
	$(BOOST_CPPFLAGS)                                             \
	$(GSL_CFLAGS)                                                 \
	-I$(top_srcdir)/src/sampling/crn/includes                     \
	-I$(top_srcdir)/src/design/pattern/includes                   \
	-I$(top_srcdir)/src/sampling/common/includes

crn_test_LDADD =                                                      \
	$(BOOST_PROGRAM_OPTIONS_LIB)                                  \
	$(top_srcdir)/src/sampling/crn/libsamplingcrn.la              \
	$(top_srcdir)/src/sampling/common/libsamplingcommon.la        \
	$(GSL_LIBS)

crn_test_LDFLAGS =                                                    \
	$(BOOST_LDFLAGS)

seeds_read_SOURCES =                                                  \
	seeds_read.cc

seeds_read_CPPFLAGS =                                                 \
	$(BOOST_CPPFLAGS)

seeds_read_LDFLAGS =                                                  \
	$(BOOST_LDFLAGS)

struct_test_SOURCES =                                                 \
	struct_test.cc

struct_test_CPPFLAGS =                                                \
	$(BOOST_CPPFLAGS)

struct_test_LDFLAGS =                                                 \
	$(BOOST_LDFLAGS)

groups_test_SOURCES =                                                 \
	groups.cc

groups_test_CPPFLAGS =                                                \
	$(BOOST_CPPFLAGS)

groups_test_LDFLAGS =                                                 \
	$(BOOST_LDFLAGS)

read_graphml_SOURCES =                                                \
	read_graphml.cc

read_graphml_CPPFLAGS =                                               \
	-I$(top_srcdir)/src/network/evolution/includes                \
	$(BOOST_CPPFLAGS)

read_graphml_LDADD =                                                  \
	$(top_srcdir)/src/network/evolution/libnetevo.la

read_graphml_LDFLAGS =                                                \
	$(BOOST_LDFLAGS)


write_seeds_SOURCES =                                                 \
	write_seeds.cc

write_seeds_CPPFLAGS =                                                \
	$(BOOST_CPPFLAGS)                                             \
	$(GSL_CFLAGS)

write_seeds_LDADD =                                                   \
	$(BOOST_FILESYSTEM_LIB)                                       \
	$(BOOST_SYSTEM_LIB)                                           \
	$(GSL_LIBS)

write_seeds_LDFLAGS =                                                 \
	$(BOOST_LDFLAGS)


MAINTAINERCLEANFILES = Makefile.in

include $(top_srcdir)/mk/mpi.am