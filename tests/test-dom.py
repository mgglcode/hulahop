import os

import gtk

import hulahop
hulahop.startup(os.path.expanduser('~/.test-hulahop'))
from hulahop.webview import WebView

from xpcom import components

class EventListener:
    _com_interfaces_ = components.interfaces.nsIDOMEventListener

    def handleEvent(self, event):
        print 'Mouse down'
    
def _quit(window):
    hulahop.shutdown()
    gtk.main_quit()

window = gtk.Window()
window.connect("destroy", _quit)

web_view = WebView()
web_view.window_root.addEventListener('mousedown', EventListener(), False)
web_view.load_uri('http://www.google.com')
window.add(web_view)
web_view.show()

window.show()

gtk.main()
