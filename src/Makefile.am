INCLUDES =					\
	$(DEPS_CFLAGS)				\
	$(PYTHON_INCLUDES)			\
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

noinst_LTLIBRARIES = libhulahop.la

libhulahop_la_SOURCES = 		\
	HulahopDirectoryProvider.cpp	\
	HulahopDirectoryProvider.h	\
	hulahop-web-view.cpp		\
	hulahop-web-view.h
