xptdir = $(pkglibdir)/components
xpt_in_files = hulahopAddCertException.idl
xpt_DATA = $(xpt_in_files:.idl=.xpt)

%.xpt: %.idl
	$(LIBXUL_SDK_DIR)/bin/xpidl -m typelib -w -v -I $(LIBXUL_SDK_DIR)/idl -e $@ $< 

componentsdir = $(pkglibdir)/components
components_DATA = hulahopAddCertException.js	\
		  xulappinfo.js

EXTRA_DIST = $(xpt_in_files) $(components_DATA)

CLEANFILES = $(xpt_DATA)

DISTCLEANFILES = $(xpt_DATA)
