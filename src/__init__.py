from hulahop import _hulahop

_hulahop.startup()

from xpcom import components
from xpcom.components import interfaces

class Browser(_hulahop.Browser):
    def __init__(self):
        _hulahop.Browser.__init__(self)

    def get_browser(self):
        return _hulahop.Browser.get_browser(self)

    def get_document(self):
        if self.browser:
            return self.browser.contentDOMWindow
        return None

    def load_uri(self, uri):
        web_nav = self.browser.queryInterface(interfaces.nsIWebNavigation)
        web_nav.loadURI(uri, interfaces.nsIWebNavigation.LOAD_FLAGS_NONE,
                        None, None, None)

    document = property(get_document)
    browser = property(get_browser)
