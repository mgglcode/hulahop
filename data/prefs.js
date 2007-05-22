// Disable onload popups
pref("dom.disable_open_during_load", true);

// Disable usless security warnings
pref("security.warn_entering_secure", false);
pref("security.warn_entering_secure.show_once", true);
pref("security.warn_leaving_secure", false);
pref("security.warn_leaving_secure.show_once", false);
pref("security.warn_submit_insecure", false);
pref("security.warn_submit_insecure.show_once", false);
pref("security.warn_viewing_mixed", true);
pref("security.warn_viewing_mixed.show_once", false);
pref("security.warn_entering_weak", true);
pref("security.warn_entering_weak.show_once", false);

// Set some style properties to not follow our dark gtk theme
pref("ui.-moz-field", "#FFFFFF");
pref("ui.-moz-fieldtext", "#000000");
pref("ui.buttonface", "#D3D3DD");
pref("ui.buttontext", "#000000");

// Fonts
pref("font.size.unit", "pt");

pref("font.default.ar", "sans-serif");
pref("font.size.variable.ar", 12);
pref("font.size.fixed.ar", 9);

pref("font.default.el", "serif");
pref("font.size.variable.el", 12);
pref("font.size.fixed.el", 9);

pref("font.default.he", "sans-serif");
pref("font.size.variable.he", 12);
pref("font.size.fixed.he", 9);

pref("font.default.ja", "sans-serif");
pref("font.size.variable.ja", 12);
pref("font.size.fixed.ja", 12);

pref("font.default.ko", "sans-serif");
pref("font.size.variable.ko", 12);
pref("font.size.fixed.ko", 12);

pref("font.default.th", "serif");
pref("font.size.variable.th", 12);
pref("font.size.fixed.th", 9);

pref("font.default.tr", "serif");
pref("font.size.variable.tr", 12);
pref("font.size.fixed.tr", 9);

pref("font.default.x-baltic", "serif");
pref("font.size.variable.x-baltic", 12);
pref("font.size.fixed.x-baltic", 9);

pref("font.default.x-central-euro", "serif");
pref("font.size.variable.x-central-euro", 12);
pref("font.size.fixed.x-central-euro", 9);

pref("font.default.x-cyrillic", "serif");
pref("font.size.variable.x-cyrillic", 12);
pref("font.size.fixed.x-cyrillic", 9);

pref("font.default.x-unicode", "serif");
pref("font.size.variable.x-unicode", 12);
pref("font.size.fixed.x-unicode", 9);

pref("font.default.x-western", "serif");
pref("font.size.variable.x-western", 12);
pref("font.size.fixed.x-western", 9);

pref("font.default.zh-CN", "sans-serif");
pref("font.size.variable.zh-CN", 12);
pref("font.size.fixed.zh-CN", 12);

pref("font.default.zh-TW", "sans-serif");
pref("font.size.variable.zh-TW", 12);
pref("font.size.fixed.zh-TW", 12);

pref("font.default.zh-HK", "sans-serif");
pref("font.size.variable.zh-HK", 12);
pref("font.size.fixed.zh-HK", 12);

// Enable error pages (xulrunner is missing this pref)
pref("browser.xul.error_pages.enabled", true);
