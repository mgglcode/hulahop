m4_define([hulahop_build_id],[2008072400]) # FIXME: when should we update this?
m4_define([hulahop_version],[0.5.1])
m4_define([sp_version],[0.86])

AC_INIT([hulahop],[0.5.2],[],[hulahop])

AC_SUBST([HULAHOP_VERSION],[hulahop_version])
AC_SUBST([HULAHOP_BUILD_ID],[hulahop_build_id])
AC_SUBST([SP_VERSION],[sp_version])

AC_PREREQ([2.59])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([configure.ac])

AC_CANONICAL_HOST
AC_SUBST([HULAHOP_HOST_CPU],[$host_cpu])
AC_SUBST([HULAHOP_HOST_OS],[$host_os])

AM_INIT_AUTOMAKE([1.9 foreign dist-bzip2 no-dist-gzip])

AM_MAINTAINER_MODE

AC_PROG_LIBTOOL

AC_PROG_CXX

PKG_CHECK_MODULES(DEPS, gtk+-2.0)

AM_PATH_PYTHON
AM_CHECK_PYTHON_HEADERS(,[AC_MSG_ERROR(could not find Python headers)])

AC_PATH_PROG(PYGTK_CODEGEN, pygtk-codegen-2.0, no)

PKG_CHECK_MODULES(PYGTK, pygtk-2.0)

PYGTK_DEFSDIR=`$PKG_CONFIG --variable=defsdir pygtk-2.0`
AC_SUBST(PYGTK_DEFSDIR)

PKG_CHECK_MODULES([LIBXUL],[libxul])

# FIXME To be able to use pyxpcom we currently need
# to link to xpcom at build time. Which involves various hacks.
LIBXUL_SDK_DIR=`$PKG_CONFIG --variable=sdkdir libxul`
LIBXUL_DIR="`dirname $LIBXUL_SDK_DIR`/xulrunner-`basename $LIBXUL_SDK_DIR|sed -e 's/^.*-\(@<:@0-9.@:>@\+\)$/\1/'`"
LIBXUL_LIBS="$LIBXUL_LIBS -L$LIBXUL_DIR -lpyxpcom"

LIBXUL_CFLAGS=`$PKG_CONFIG --cflags --define-variable=includetype=unstable libxul`

AC_SUBST([LIBXUL_CFLAGS])
AC_SUBST([LIBXUL_LIBS])
AC_SUBST([LIBXUL_DIR])
AC_SUBST([LIBXUL_SDK_DIR])

AC_CONFIG_FILES([
components/xulappinfo.js
])

AC_OUTPUT([
Makefile
components/Makefile
data/Makefile
data/prefs.js
data/chrome/Makefile
src/Makefile
python/Makefile
python/config.py
])
