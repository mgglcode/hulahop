import os

import gtk

import hulahop
hulahop.startup(os.path.expanduser('~/.test-hulahop'))
from hulahop.webview import WebView

def _quit(window):
    hulahop.shutdown()
    gtk.main_quit()

window = gtk.Window()
window.connect("destroy", _quit)

web_view = WebView()
web_view.load_uri('http://www.google.com')
window.add(web_view)
web_view.show()

window.show()

gtk.main()