sugardir = $(pythondir)/hulahop
sugar_PYTHON =		\
	__init__.py	\
	webview.py

INCLUDES =					\
	$(DEPS_CFLAGS)				\
	$(PYTHON_INCLUDES)			\
	$(PYGTK_CFLAGS)				\
	$(MOZILLA_CFLAGS)			\
	-I$(MOZILLA_INCLUDE_ROOT)/docshell	\
	-I$(MOZILLA_INCLUDE_ROOT)/dom		\
	-I$(MOZILLA_INCLUDE_ROOT)/xulapp	\
	-I$(MOZILLA_INCLUDE_ROOT)/nspr		\
	-I$(MOZILLA_INCLUDE_ROOT)/pyxpcom	\
	-I$(MOZILLA_INCLUDE_ROOT)/string	\
	-I$(MOZILLA_INCLUDE_ROOT)/webbrwsr	\
	-I$(MOZILLA_INCLUDE_ROOT)/widget	\
	-I$(MOZILLA_INCLUDE_ROOT)/xpcom		\
	-DMOZILLA_HOME=\"$(MOZILLA_HOME)\"

pkgpyexec_LTLIBRARIES = _hulahop.la

_hulahop_la_LDFLAGS = -module -avoid-version -R$(MOZILLA_HOME) -dlopen self
_hulahop_la_LIBADD = 		\
	$(GECKO_LIBS)		\
	$(DEPS_LIBS)		\
	$(MOZILLA_EXTRA_LIBS)	\
	$(MOZILLA_GLUE_LIBS)

_hulahop_la_SOURCES = 			\
	HulahopDirectoryProvider.cpp	\
	HulahopDirectoryProvider.h	\
	hulahop-web-view.cpp		\
	hulahop-web-view.h		\
	hulahop.c			\
	hulahopmodule.c

hulahop.c: hulahop.defs hulahop.override

CLEANFILES = hulahop.c
EXTRA_DIST = hulahop.override hulahop.defs

.defs.c:
	(cd $(srcdir)\
	 && $(PYGTK_CODEGEN) \
	    --register $(PYGTK_DEFSDIR)/gdk-types.defs \
	    --register $(PYGTK_DEFSDIR)/gtk-types.defs \
	    --override $*.override \
	    --prefix py$* $*.defs) > gen-$*.c \
	&& cp gen-$*.c $*.c \
	&& rm -f gen-$*.c
