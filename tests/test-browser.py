import gtk
import hulahop

window = gtk.Window()

browser = hulahop.Browser()
window.add(browser)
browser.show()

window.show()

browser.load_uri('http://www.google.com')

gtk.main()
