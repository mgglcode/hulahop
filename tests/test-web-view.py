import os

import gtk

import hulahop
hulahop.startup(os.path.expanduser('~/.test-hulahop'))
from hulahop.webview import WebView

def quit(window):
    hulahop.shutdown()
    gtk.main_quit()

def entry_activate_cb(entry):
    from xpcom import components
    from xpcom.components import interfaces

    cls = components.classes['@sugarlabs.org/add-cert-exception;1']
    cert_exception = cls.createInstance(interfaces.hulahopAddCertException)
    cert_exception.showDialog(web_view.dom_window)

window = gtk.Window()
window.set_default_size(600, 400)
window.connect("destroy", quit)

vbox = gtk.VBox()

entry = gtk.Entry()
entry.connect('activate', entry_activate_cb)
vbox.pack_start(entry, False)
entry.show()

web_view = WebView()
web_view.load_uri('https://mozilla.org')
vbox.pack_start(web_view)
web_view.show()

window.add(vbox)
vbox.show()

window.show()

entry.grab_focus()

gtk.main()
