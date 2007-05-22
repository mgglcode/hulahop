# Copyright (C) 2007, Red Hat, Inc.
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.

from hulahop import _hulahop

import xpcom
from xpcom import components
from xpcom.components import interfaces

class _Chrome:
    _com_interfaces_ = interfaces.nsIWebBrowserChrome,  \
                       interfaces.nsIEmbeddingSiteWindow

    # nsIWebBrowserChrome
    def destroyBrowserWindow(self):
        pass
    
    def exitModalEventLoop(self, status):
        pass
        
    def isWindowModal(self):
        return False

    def setStatus(self, statusType, status):
        pass

    def showAsModal(self):
        pass
        
    def sizeBrowserTo(self, cx, cy):
        pass

    # nsIEmbeddingSiteWindow
    def getDimensions(self, flags):
        pass

    def setDimensions(self, flags, x, y, cx, cy):
        pass

    def setFocus(self):
        pass

    def get_title(self):
        return ''
        
    def set_title(self, title):
        print title
        
    def get_visibility(self):
        return True

    def set_visibility(self, visibility):
        pass
        
class WebView(_hulahop.WebView):
    def __init__(self):
        _hulahop.WebView.__init__(self)
        
        self._chrome = xpcom.server.WrapObject(
                _Chrome(), interfaces.nsIEmbeddingSiteWindow)
        weak_ref = xpcom.client.WeakReference(self._chrome)
        self.browser.containerWindow = self._chrome
        
        self.create_window()

    def get_window_root(self):
        return _hulahop.WebView.get_window_root(self)

    def get_browser(self):
        return _hulahop.WebView.get_browser(self)

    def get_doc_shell(self):
        return _hulahop.WebView.get_doc_shell(self)

    def get_web_progress(self):
        return self.doc_shell.queryInterface(interfaces.nsIWebProgress)

    def get_web_navigation(self):
        return self.browser.queryInterface(interfaces.nsIWebNavigation)

    def get_window(self):
        return self.browser.contentDOMWindow

    def load_uri(self, uri):
        self.web_navigation.loadURI(
                    uri, interfaces.nsIWebNavigation.LOAD_FLAGS_NONE,
                    None, None, None)

    window = property(get_window)
    browser = property(get_browser)
    window_root = property(get_window_root)
    doc_shell = property(get_doc_shell)
    web_progress = property(get_web_progress)
    web_navigation = property(get_web_navigation)
