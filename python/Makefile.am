pkgpyexec_PYTHON =	\
	__init__.py	\
	webview.py	\
	config.py

INCLUDES =					\
	$(DEPS_CFLAGS)				\
	$(PYTHON_INCLUDES)			\
	$(PYGTK_CFLAGS)				\
	-I$(top_srcdir)/src

pkgpyexec_LTLIBRARIES = _hulahop.la

_hulahop_la_LDFLAGS = -module -avoid-version -R$(LIBXUL_DIR)
_hulahop_la_LIBADD = 				\
	$(LIBXUL_LIBS)				\
	$(DEPS_LIBS)				\
	$(top_builddir)/src/libhulahop.la

_hulahop_la_SOURCES = 			\
	hulahop.c			\
	hulahopmodule.c

hulahop.c: hulahop.defs hulahop.override

CLEANFILES = hulahop.c config.py
EXTRA_DIST = hulahop.override hulahop.defs config.py.in

.defs.c:
	(cd $(srcdir)\
	 && $(PYGTK_CODEGEN) \
	    --register $(PYGTK_DEFSDIR)/gdk-types.defs \
	    --register $(PYGTK_DEFSDIR)/gtk-types.defs \
	    --override $*.override \
	    --prefix py$* $*.defs) > gen-$*.c \
	&& cp gen-$*.c $*.c \
	&& rm -f gen-$*.c
