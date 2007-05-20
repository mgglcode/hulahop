import os

import gtk

import hulahop
hulahop.startup(os.path.expanduser('~/.test-hulahop'))
from hulahop.webview import WebView

from xpcom import components
from xpcom.server.factory import Factory

class AppLauncher:
    _com_interfaces_ = components.interfaces.nsIHelperAppLauncherDialog

    def promptForSaveToFile(self, launcher, windowContext,
                            defaultFile, suggestedFileExtension):
        print 'promptForSaveToFile'
                            
    def show(self, launcher, context, reason):
        print 'show'

class EventListener:
    _com_interfaces_ = components.interfaces.nsIDOMEventListener

    def handleEvent(self, event):
        print 'Mouse down'
    
def _quit(window):
    hulahop.shutdown()
    gtk.main_quit()

registrar = components.registrar
registrar.registerFactory('{64355793-988d-40a5-ba8e-fcde78cac631}"',
                          'Test Application Launcher',
                          '@mozilla.org/helperapplauncherdialog;1',
                          Factory(AppLauncher))

cls = components.classes["@mozilla.org/preferences-service;1"]
prefService = cls.getService(components.interfaces.nsIPrefService)
branch = prefService.getBranch('')
branch.setBoolPref('security.warn_submit_insecure', False)
branch.setBoolPref('security.warn_submit_insecure.show_once', False)

window = gtk.Window()
window.connect("destroy", _quit)

web_view = WebView()
web_view.window_root.addEventListener('mousedown', EventListener(), False)
web_view.load_uri('http://www.google.com')
window.add(web_view)
web_view.show()

window.show()

gtk.main()
