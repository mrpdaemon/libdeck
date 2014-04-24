AC_INIT([libdeck], [0.1])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/libdeck.c])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE
AM_PROG_LIBTOOL
AC_PROG_CC
AC_PROG_INSTALL

dnl Check for random device
AC_MSG_CHECKING(for randomness device)
if test -r "/dev/random";then
   AC_DEFINE(DEVRANDOM, ["/dev/random"], [random device])
   AC_MSG_RESULT(/dev/random)
elif test -r "/dev/urandom";then
   AC_DEFINE(DEVRANDOM, ["/dev/urandom"], [random device])
   AC_MSG_RESULT(/dev/urandom)
else
   AC_MSG_RESULT(not found)
fi

dnl Check for pthread libraries
AX_PTHREAD

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
