AC_INIT([hulahop],[0.4.0],[],[hulahop])

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

PKG_CHECK_MODULES([LIBXUL],[libxul])

# FIXME To be able to use pyxpcom we currently need
# to link to xpcom at build time. Which involves various hacks.
LIBXUL_SDK_DIR=`$PKG_CONFIG --variable=sdkdir libxul`
LIBXUL_DIR=`echo $LIBXUL_SDK_DIR | sed -e 's/-devel//'`
LIBXUL_LIBS="$LIBXUL_LIBS -L$LIBXUL_DIR -lpyxpcom"

LIBXUL_CFLAGS=`$PKG_CONFIG --cflags --define-variable=includetype=unstable libxul`

AC_SUBST([LIBXUL_CFLAGS])
AC_SUBST([LIBXUL_LIBS])
AC_SUBST([LIBXUL_DIR])
AC_SUBST([LIBXUL_SDK_DIR])

AC_OUTPUT([
Makefile
components/Makefile
data/Makefile
data/chrome/Makefile
src/Makefile
python/Makefile
python/config.py
])
