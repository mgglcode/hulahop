import os

import gtk

import hulahop
hulahop.startup(os.path.expanduser('~/.test-hulahop'))
from hulahop.webview import WebView

def quit(window):
    hulahop.shutdown()
    gtk.main_quit()

def entry_activate_cb(entry):
    web_view.load_uri(entry.get_text())
    web_view.grab_focus()

window = gtk.Window()
window.set_default_size(600, 400)
window.connect("destroy", quit)

vbox = gtk.VBox()

entry = gtk.Entry()
entry.connect('activate', entry_activate_cb)
vbox.pack_start(entry, False)
entry.show()

web_view = WebView()
web_view.load_uri('http://www.gnome.org')
vbox.pack_start(web_view)
web_view.show()

window.add(vbox)
vbox.show()

window.show()

entry.grab_focus()

gtk.main()
