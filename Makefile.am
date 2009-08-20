# Copyright (C) 2007-2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
#  
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without 
# modifications, as long as this notice is preserved.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
ACLOCAL_AMFLAGS = -I m4 -I /usr/share/aclocal


SUBDIRS = m4 R src doc

if CPPUNIT_SUPPORT
  SUBDIRS += tests
endif

EXTRA_DIST = \
	README \
	seeds.dat \
	seeds-mpi.dat \
	iitac.sh \
	lonsdale.sh \
	$(srcdir)/pbs \
	$(srcdir)/bin \
	$(srcdir)/graphs_dense.sh \
	$(srcdir)/graphs.sh \
	$(srcdir)/m4/ac_gcov.m4 \
	$(srcdir)/m4/ac_valgrind.m4 \
	$(srcdir)/m4/ac_perl_module_version.m4 \
	$(srcdir)/m4/ax_boost_base.m4 \
	$(srcdir)/m4/ax_prog_doxygen.m4 \
	$(srcdir)/m4/acx_mpi.m4 \
	$(srcdir)/mk/gcov.am \
	$(srcdir)/mk/valgrind.am \
	$(srcdir)/mk/gcov-recursive.am \
	$(srcdir)/mk/valgrind-recursive.am \
	$(srcdir)/mk/doxygen.am


include mk/doxygen.am


DISTCHECK_CONFIGURE_FLAGS = --enable-gcov --enable-valgrind

DISTCLEANFILES = configure
MAINTAINERCLEANFILES = "Makefile.in semantic.cache"
MOSTLYCLEANFILES = ${DX_CLEANFILES}


clean-local:
	rm -rf ./coverage/*

# add valgrind targets
if VALGRIND
valgrind: all
	cd tests && $(MAKE) $(AM_MAKEFLAGS) valgrind
valgrind.gen-suppressions: all
	cd tests && $(MAKE) $(AM_MAKEFLAGS) valgrind.gen-suppressions
else
valgrind: ;
valgrind.gen-suppressions: ;
endif


if GCOV

COVOUT=./coverage
LCOVOUT=$(COVOUT)/lcov-reports
GENHTMLOUT=$(LCOVOUT)/html

lcov-prepare:
	rm -rf $(COVOUT)
	mkdir -p $(GENHTMLOUT)
	lcov --zerocounters --directory . --quiet

lcov: clean lcov-prepare all
	cd tests && $(MAKE) $(AM_MAKEFLAGS) lcov

else
lcov: ;

endif



.PHONY: valgrind valgrind.gen-suppressions lcov lcov-prepare \
	doxygen-run doxygen-doc $(DX_PS_GOAL) $(DX_PDF_GOAL)
