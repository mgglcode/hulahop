import os

import gtk

import hulahop
hulahop.startup(os.path.expanduser('~/.test-hulahop'))
from hulahop.webview import WebView

from xpcom import components
    
def _quit(window):
    hulahop.shutdown()
    gtk.main_quit()

cls = components.classes["@mozilla.org/preferences-service;1"]
prefService = cls.getService(components.interfaces.nsIPrefService)
branch = prefService.getBranch('')
branch.setBoolPref('security.warn_submit_insecure', False)
branch.setBoolPref('security.warn_submit_insecure.show_once', False)

window = gtk.Window()
window.connect("destroy", _quit)

web_view = WebView()
web_view.load_uri('http://www.google.com')
window.add(web_view)
web_view.show()

window.show()

gtk.main()
