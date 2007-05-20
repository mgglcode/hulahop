import gtk
import hulahop

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

browser = hulahop.Browser()
browser.window_root.addEventListener('mousedown', EventListener(), False)
browser.load_uri('http://www.google.com')
window.add(browser)
browser.show()

window.show()

gtk.main()
