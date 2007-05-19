from hulahop import _hulahop

_hulahop.startup()

from xpcom import components
from xpcom.components import interfaces

class Browser(_hulahop.Browser):
    def __init__(self):
        _hulahop.Browser.__init__(self)

    def get_document(self):
        browser = self.get_browser()
        if not browser:
            return None
        else:
            web_browser = browser.queryInterface(interfaces.nsIWebBrowser)
            return web_browser.contentDOMWindow

    document = property(get_document)
