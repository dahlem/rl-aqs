# Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@gmail.com>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

AC_DEFUN([AC_GCOV],
[
  AC_ARG_ENABLE(gcov,
		AS_HELP_STRING([--enable-gcov],
			       [Enable gcov/lcov support @<:@default=no@:>@ (gcc only)]),
                [ac_gcov_enabled=$enableval],
                [ac_gcov_enabled=no])

	if test "$ac_gcov_enabled" = "yes"; then
		AC_CHECK_PROGS(gcov, gcov)
		if ! test "$gcov"; then
			AC_MSG_ERROR([Could not find gcov utility, check your gcc instalation])
		fi

		AC_CHECK_LIB(gcov, __gcov_init, [], [])
		AC_CHECK_PROGS(lcov, lcov)
		AC_CHECK_PROGS(genhtml, genhtml)
		AC_CHECK_PROGS(gendesc, gendesc)
		AC_CHECK_PROGS(geninfo, geninfo)
		if ! test "$lcov" || ! test "$genhtml" || ! test "$gendesc" ||
		   ! test "$geninfo"; then
			AC_MSG_ERROR([Could not find lcov utilities, do you have lcov installed?])
		fi

                CPPFLAGS="$CPPFLAGS -fprofile-arcs -ftest-coverage"

                # check whether GD.pm is available
                AC_PERL_MODULE_VERSION([GD 2.30], [ac_genhtml_frames=yes], [ac_genhtml_frames=yes])

                AC_SUBST([CLEANFILES],['*.da *.gcda *.lcov'])
                AC_SUBST([MOSTLYCLEANFILES],['*.bb *.bbg *.gcno'])

        fi

        if test "x$ac_genhtml_frames" = "xyes"; then
                AC_SUBST([GCOV_GENHTML_OPTIONS],['-f --legend -s'])
        else
                AC_SUBST([GCOV_GENHTML_OPTIONS],['--legend -s'])
        fi

	AM_CONDITIONAL(GCOV, [test "$ac_gcov_enabled" = "yes"])
])