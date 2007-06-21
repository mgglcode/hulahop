INCLUDES =					                        \
	$(DEPS_CFLAGS)				                    \
	$(PYTHON_INCLUDES)			                    \
	$(MOZILLA_CFLAGS)			                    \
	-I$(MOZILLA_INCLUDE_ROOT)/docshell	            \
	-I$(MOZILLA_INCLUDE_ROOT)/dom		            \
	-I$(MOZILLA_INCLUDE_ROOT)/js		            \
	-I$(MOZILLA_INCLUDE_ROOT)/xulapp	            \
	-I$(MOZILLA_INCLUDE_ROOT)/nspr		            \
	-I$(MOZILLA_INCLUDE_ROOT)/pyxpcom	            \
	-I$(MOZILLA_INCLUDE_ROOT)/string	            \
	-I$(MOZILLA_INCLUDE_ROOT)/webbrwsr	            \
	-I$(MOZILLA_INCLUDE_ROOT)/widget	            \
	-I$(MOZILLA_INCLUDE_ROOT)/xpcom		            \
	-I$(MOZILLA_INCLUDE_ROOT)/xpconnect	            \
	-DMOZILLA_HOME=\"$(MOZILLA_HOME)\"              \
    -DPLUGIN_PATH=\"$(libdir)/mozilla/plugins\"     \
	-DDATA_DIR=\"$(pkgdatadir)\"

noinst_LTLIBRARIES = libhulahop.la

libhulahop_la_SOURCES = 		    \
	HulahopDirectoryProvider.cpp	\
	HulahopDirectoryProvider.h	    \
	hulahop.cpp			            \
	hulahop.h			            \
	hulahop-web-view.cpp		    \
	hulahop-web-view.h
