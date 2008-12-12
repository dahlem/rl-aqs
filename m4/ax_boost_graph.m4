# ===========================================================================
#           http://autoconf-archive.cryp.to/ax_boost_graph.html
# ===========================================================================
#
# SYNOPSIS
#
#   AX_BOOST_GRAPH
#
# DESCRIPTION
#
#   Test for Graph library from the Boost C++ libraries. The macro
#   requires a preceding call to AX_BOOST_BASE. Further documentation is
#   available at <http://randspringer.de/boost/index.html>.
#
#   This macro calls:
#
#     AC_SUBST(BOOST_GRAPH_LIB)
#
#   And sets:
#
#     HAVE_BOOST_GRAPH
#
# LAST MODIFICATION
#
#   2008-04-12
#
# COPYLEFT
#
#   Copyright (c) 2008 Thomas Porschberg <thomas@randspringer.de>
#   Copyright (c) 2008 Michael Tindal
#
#   Copying and distribution of this file, with or without modification, are
#   permitted in any medium without royalty provided the copyright notice
#   and this notice are preserved.

AC_DEFUN([AX_BOOST_GRAPH],
[
	AC_ARG_WITH([boost-graph],
	AS_HELP_STRING([--with-boost-graph@<:@=special-lib@:>@],
                   [use the Graph library from boost - it is possible to specify a certain library for the linker
                        e.g. --with-boost-graph=boost_graph-gcc-mt-d-1_33_1 ]),
        [
        if test "$withval" = "no"; then
			want_boost="no"
        elif test "$withval" = "yes"; then
            want_boost="yes"
            ax_boost_user_graph_lib=""
        else
		    want_boost="yes"
        	ax_boost_user_graph_lib="$withval"
		fi
        ],
        [want_boost="yes"]
	)

	if test "x$want_boost" = "xyes"; then
        AC_REQUIRE([AC_PROG_CC])
		CPPFLAGS_SAVED="$CPPFLAGS"
		CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
		export CPPFLAGS

		LDFLAGS_SAVED="$LDFLAGS"
		LDFLAGS="$LDFLAGS $BOOST_LDFLAGS"
		export LDFLAGS

        AC_CACHE_CHECK(whether the Boost::Graph library is available,
					   ax_cv_boost_graph,
        [AC_LANG_PUSH([C++])
		 AC_COMPILE_IFELSE(AC_LANG_PROGRAM([[@%:@include <boost/graph/adjacency_list.hpp>]],
                                   [[typedef boost::adjacency_list<boost::listS, boost::vecS> Graph;
                                     Graph G(2);
                                     return 0;
                                   ]]),
         ax_cv_boost_graph=yes, ax_cv_boost_graph=no)
         AC_LANG_POP([C++])
		])
		if test "x$ax_cv_boost_graph" = "xyes"; then
			AC_DEFINE(HAVE_BOOST_GRAPH,,[define if the Boost::Graph library is available])
            BOOSTLIBDIR=`echo $BOOST_LDFLAGS | sed -e 's/@<:@^\/@:>@*//'`
            if test "x$ax_boost_user_graph_lib" = "x"; then
                for libextension in `ls $BOOSTLIBDIR/libboost_graph*.{so,a}* 2>/dev/null | sed 's,.*/,,' | sed -e 's;^lib\(boost_graph.*\)\.so.*$;\1;' -e 's;^lib\(boost_graph.*\)\.a*$;\1;'` ; do
                     ax_lib=${libextension}
				    AC_CHECK_LIB($ax_lib, exit,
                                 [BOOST_GRAPH_LIB="-l$ax_lib"; AC_SUBST(BOOST_GRAPH_LIB) link_graph="yes"; break],
                                 [link_graph="no"])
  				done
                if test "x$link_graph" != "xyes"; then
                for libextension in `ls $BOOSTLIBDIR/boost_graph*.{dll,a}* 2>/dev/null | sed 's,.*/,,' | sed -e 's;^\(boost_graph.*\)\.dll.*$;\1;' -e 's;^\(boost_graph.*\)\.a*$;\1;'` ; do
                     ax_lib=${libextension}
				    AC_CHECK_LIB($ax_lib, exit,
                                 [BOOST_GRAPH_LIB="-l$ax_lib"; AC_SUBST(BOOST_GRAPH_LIB) link_graph="yes"; break],
                                 [link_graph="no"])
  				done
                fi

            else
               for ax_lib in $ax_boost_user_graph_lib boost_graph-$ax_boost_user_graph_lib; do
				      AC_CHECK_LIB($ax_lib, main,
                                   [BOOST_GRAPH_LIB="-l$ax_lib"; AC_SUBST(BOOST_GRAPH_LIB) link_graph="yes"; break],
                                   [link_graph="no"])
                  done

            fi
			if test "x$link_graph" != "xyes"; then
				AC_MSG_ERROR(Could not link against $ax_lib !)
			fi
		fi

		CPPFLAGS="$CPPFLAGS_SAVED"
    	LDFLAGS="$LDFLAGS_SAVED"
	fi
])
