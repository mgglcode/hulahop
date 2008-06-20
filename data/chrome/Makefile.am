manifestdir = $(pkgdatadir)/chrome
manifest_in_files = chrome.manifest.in
manifest_DATA = $(manifest_in_files:.manifest.in=.manifest)

%.manifest: %.manifest.in
	@sed -e "s!@CHROMEDIR@!$(pkgdatadir)/chrome!g" $< > $@

brandingdir = $(manifestdir)/branding
branding_DATA = brand.properties

globalchromedir = $(manifestdir)/global
globalchrome_DATA = netError.dtd

EXTRA_DIST = \
	$(manifest_in_files)	\
	$(branding_in_files)	\
	$(globalchrome_DATA)	\
	$(branding_DATA)

CLEANFILES = \
	$(manifest_DATA)

DISTCLEANFILES = \
	$(manifest_DATA)	\
	$(branding_DATA)
