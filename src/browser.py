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

from xpcom import components
from xpcom.components import interfaces

class Browser(_hulahop.Browser):
    def __init__(self):
        _hulahop.Browser.__init__(self)

    def get_window_root(self):
        return _hulahop.Browser.get_window_root(self)

    def get_browser(self):
        return _hulahop.Browser.get_browser(self)

    def get_window(self):
        return self.browser.contentDOMWindow

    def load_uri(self, uri):
        web_nav = self.browser.queryInterface(interfaces.nsIWebNavigation)
        web_nav.loadURI(uri, interfaces.nsIWebNavigation.LOAD_FLAGS_NONE,
                        None, None, None)

    window = property(get_window)
    browser = property(get_browser)
    window_root = property(get_window_root)