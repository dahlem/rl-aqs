# Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@gmail.com>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

AC_DEFUN([AC_VALGRIND],
[
  AC_ARG_ENABLE(valgrind,
                AC_HELP_STRING([--enable-valgrind],	
                               [enable run-time valgrind detection.  @<:@default=no@:>@]),
                [ac_valgrind_enabled=$enableval],
		[ac_valgrind_enabled=no])


  VALGRIND_VERSION="3.2"

  if test "$ac_valgrind_enabled" = "yes"; then
          PKG_CHECK_MODULES(VALGRIND, 
                            valgrind > $VALGRIND_VERSION,
                            USE_VALGRIND="yes",
                            [
                                    USE_VALGRIND="no"
                                    AC_MSG_RESULT([no])
                            ])
           AC_SUBST(VALGRIND_CFLAGS)
           AC_SUBST(VALGRIND_LIBS)
  
           AC_PATH_PROG(VALGRIND_PATH, valgrind, no)

           AC_SUBST([VALGRIND_SUPPRESSIONS],[''])
           AC_SUBST([VALGRIND_SUPPS_DIR],['./valgrind/suppressions/common'])
           AC_SUBST([VALGRIND_OPTIONS], ['--tool=memcheck --leak-check=full'])
  fi

  AM_CONDITIONAL(VALGRIND, [test  "x$ac_valgrind_enabled" = "xyes"] && [test ! "x$VALGRIND_PATH" = "xno"])
])
