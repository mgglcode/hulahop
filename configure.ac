AC_INIT([hulahop],[0.1],[],[hulahop])

AC_PREREQ([2.59])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([configure.ac])

AM_INIT_AUTOMAKE([1.9 foreign dist-bzip2 no-dist-gzip])

AC_PROG_LIBTOOL

PKG_CHECK_MODULES(DEPS, gtk+-2.0)

AM_PATH_PYTHON
AM_CHECK_PYTHON_HEADERS(,[AC_MSG_ERROR(could not find Python headers)])

AC_PATH_PROG(PYGTK_CODEGEN, pygtk-codegen-2.0, no)

PKG_CHECK_MODULES(PYGTK, pygtk-2.0)

PYGTK_DEFSDIR=`$PKG_CONFIG --variable=defsdir pygtk-2.0`
AC_SUBST(PYGTK_DEFSDIR)

GECKO_INIT([MOZILLA])
AC_SUBST(MOZILLA_INCLUDE_ROOT)
AC_SUBST([MOZILLA_HOME])
AC_SUBST([MOZILLA_EXTRA_LIBS])
AC_SUBST([MOZILLA_GLUE_LIBS])

GECKO_XPIDL()
AC_SUBST([XPIDL])
AC_SUBST([XPIDL_IDLDIR])

PKG_CHECK_MODULES([GECKO],[${gecko_cv_gecko}-xpcom >= 1.9 ${gecko_cv_gecko}-js $gecko_cv_extra_pkg_dependencies])
AC_SUBST([GECKO_CFLAGS])
AC_SUBST([GECKO_LIBS])

AC_OUTPUT([
Makefile
components/Makefile
components/browserhelper/Makefile
src/Makefile
])
