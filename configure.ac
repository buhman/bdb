# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])

AC_INIT([bdb],[m4_esyscmd(./git-version)],[zack@buhman.org],[https://buhman.org/cgit/bdb.git])
AC_CONFIG_SRCDIR([src/bdb.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign 1.11 -Wall -Wno-portability subdir-objects silent-rules color-tests dist-xz tar-ustar])

AS_IF([! test -n "$VERSION"], [
         AC_MSG_ERROR([git-version-gen failed])
])

m4_define(bdb_version, `echo $VERSION | cut -d- -f1`)

AC_SUBST([BDB_VERSION], bdb_version)

AC_SUBST(BDB_FORMAT_VERSION, 1)

AC_SUBST(LIBBDB_VERSION_INFO, [0:0:0])

AC_CANONICAL_HOST
AC_DEFINE_UNQUOTED([CANONICAL_HOST], "$host", [Canonical host string.])

AM_SILENT_RULES([yes])

### checks ###

# CC

AC_PROG_CC
AC_PROG_CC_C99
AC_GNU_SOURCE

# pkg-config

PKG_PROG_PKG_CONFIG

#### compiler flags ####

AX_APPEND_COMPILE_FLAGS(
        [-Wall -W -Wextra -pipe -Wno-long-long -Wno-overlength-strings -Wunsafe-loop-optimizations -Wundef -Wformat=2 -Wlogical-op -Wsign-compare -Wformat-security -Wmissing-include-dirs -Wformat-nonliteral -Wold-style-definition -Wpointer-arith -Winit-self -Wdeclaration-after-statement -Wfloat-equal -Wmissing-prototypes -Wstrict-prototypes -Wredundant-decls -Wmissing-declarations -Wmissing-noreturn -Wshadow -Wendif-labels -Wcast-align -Wstrict-aliasing -Wwrite-strings -Wno-unused-parameter -Wno-error=unused-function -ffast-math -fno-common -fdiagnostics-show-option],
        [], [-pedantic -pedantic-errors -Werror])

#### libtool ####

LT_PREREQ(2.4)
LT_INIT([dlopen disable-static])

#### environment ####

AC_HEADER_STDC
AX_PTHREAD

# unit tests

PKG_CHECK_MODULES(LIBCHECK, [check])

### output ###

AC_CONFIG_FILES([
Makefile
src/Makefile
tests/Makefile
tools/Makefile
src/version.h
])

AC_OUTPUT
