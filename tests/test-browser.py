import gtk
import hulahop

window = gtk.Window()

browser = hulahop.Browser()
window.add(browser)
browser.show()

window.show()

gtk.main()
