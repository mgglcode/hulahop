import gtk
import hulahop

def _quit(window):
    hulahop.shutdown()
    gtk.main_quit()

window = gtk.Window()
window.connect("destroy", _quit)

browser = hulahop.Browser()
window.add(browser)
browser.show()

window.show()

browser.load_uri('http://www.google.com')

gtk.main()
