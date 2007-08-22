INCLUDES =						\
	$(DEPS_CFLAGS)				  	\
	$(PYTHON_INCLUDES)			 	\
	$(LIBXUL_CFLAGS)				\
	-DPLUGIN_PATH=\"$(libdir)/mozilla/plugins\"     \
	-DLIBXUL_DIR=\"$(LIBXUL_DIR)\"			\
	-DDATA_DIR=\"$(pkgdatadir)\"

noinst_LTLIBRARIES = libhulahop.la

libhulahop_la_SOURCES = 		\
	HulahopDirectoryProvider.cpp	\
	HulahopDirectoryProvider.h	\
	hulahop.cpp			\
	hulahop.h			\
	hulahop-web-view.cpp		\
	hulahop-web-view.h
