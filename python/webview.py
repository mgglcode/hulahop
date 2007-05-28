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
import logging

import gobject

from hulahop import _hulahop

import xpcom
from xpcom import components
from xpcom.components import interfaces
from xpcom.nsError import *

class _Chrome:
    _com_interfaces_ = interfaces.nsIWebBrowserChrome,      \
                       interfaces.nsIEmbeddingSiteWindow,   \
                       interfaces.nsIInterfaceRequestor

    def __init__(self, web_view):
        self.web_view = web_view
        self.title = ''
        self._visibility = False
    
    # nsIWebBrowserChrome
    def destroyBrowserWindow(self):
        logging.debug("nsIWebBrowserChrome.destroyBrowserWindow")
    
    def exitModalEventLoop(self, status):
        logging.debug("nsIWebBrowserChrome.exitModalEventLoop")
        
    def isWindowModal(self):
        logging.debug("nsIWebBrowserChrome.isWindowModal")
        return False

    def setStatus(self, statusType, status):
        #logging.debug("nsIWebBrowserChrome.setStatus")
        pass

    def showAsModal(self):
        logging.debug("nsIWebBrowserChrome.showAsModal")
        
    def sizeBrowserTo(self, cx, cy):
        logging.debug("nsIWebBrowserChrome.sizeBrowserTo: %r %r" % (cx, cy))
        self.web_view.get_toplevel().resize(cx, cy)
        self.web_view.type = WebView.TYPE_POPUP

    # nsIEmbeddingSiteWindow
    def getDimensions(self, flags):
        logging.debug("nsIEmbeddingSiteWindow.getDimensions: %r" % flags)
        base_window = self.web_view.browser.queryInterface(interfaces.nsIBaseWindow)
        if (flags & interfaces.nsIEmbeddingSiteWindow.DIM_FLAGS_POSITION) and \
           ((flags & interfaces.nsIEmbeddingSiteWindow.DIM_FLAGS_SIZE_INNER) or \
            (flags & interfaces.nsIEmbeddingSiteWindow.DIM_FLAGS_SIZE_OUTER)):
            return base_window.getPositionAndSize()
        elif flags & interfaces.nsIEmbeddingSiteWindow.DIM_FLAGS_POSITION:
            x, y = base_window.getPosition()
            return (x, y, 0, 0)
        elif (flags & interfaces.nsIEmbeddingSiteWindow.DIM_FLAGS_SIZE_INNER) or \
             (flags & interfaces.nsIEmbeddingSiteWindow.DIM_FLAGS_SIZE_OUTER):
            width, height = base_window.getSize()
            return (0, 0, width, height)
        else:
            raise xpcom.Exception('Invalid flags: %r' % flags)

    def setDimensions(self, flags, x, y, cx, cy):
        logging.debug("nsIEmbeddingSiteWindow.setDimensions: %r" % flags)

    def setFocus(self):
        logging.debug("nsIEmbeddingSiteWindow.setFocus")
        base_window = self.web_view.browser.queryInterface(interfaces.nsIBaseWindow)
        base_window.setFocus()        

    def get_title(self):
        logging.debug("nsIEmbeddingSiteWindow.get_title: %r" % self.title)
        return self.title
        
    def set_title(self, title):
        logging.debug("nsIEmbeddingSiteWindow.set_title: %r" % title)
        self.title = title
        self.web_view._notify_title_changed()
        
    def get_visibility(self):
        logging.debug("nsIEmbeddingSiteWindow.get_visibility: %r" % self._visibility)
        return self._visibility

    def set_visibility(self, visibility):
        logging.debug("nsIEmbeddingSiteWindow.set_visibility: %r" % visibility)
        if self._visibility != visibility:
            self._visibility = visibility
            if self._visibility:
                self.web_view.get_toplevel().show()
            else:
                self.web_view.get_toplevel().hide()

    def queryInterface(self, uuid):
        if not uuid in self._com_interfaces_:
            # Components.returnCode = Cr.NS_ERROR_NO_INTERFACE;
            logging.error('Interface %s not implemented by this instance: %r' % (uuid, self))
            return None
        return xpcom.server.WrapObject(self, uuid)

    def getInterface(self, uuid):
        result = self.queryInterface(uuid)
        
        if not result:
            # delegate to the nsIWebBrowser
            requestor = self.web_view.browser.queryInterface(interfaces.nsIInterfaceRequestor)
            try:
                result = requestor.getInterface(uuid)
            except xpcom.Exception:
                logging.error('Interface %s not implemented by this instance: %r' % (uuid, self.web_view.browser))
                result = None

        return result
        
class WebView(_hulahop.WebView):

    TYPE_WINDOW = 0
    TYPE_POPUP = 1

    __gproperties__ = {
        'title' : (str, None, None, None,
                   gobject.PARAM_READABLE)
    }
    def __init__(self):
        _hulahop.WebView.__init__(self)
        
        self.type = WebView.TYPE_WINDOW
        
        self._chrome = xpcom.server.WrapObject(
                    _Chrome(self), interfaces.nsIEmbeddingSiteWindow)
        weak_ref = xpcom.client.WeakReference(self._chrome)
        self.browser.containerWindow = self._chrome
        
        item = self.browser.queryInterface(interfaces.nsIDocShellTreeItem)
        item.itemType = interfaces.nsIDocShellTreeItem.typeContentWrapper

        self.create_window()

    def _notify_title_changed(self):
        self.notify('title')

    def do_get_property(self, pspec):
        if pspec.name == 'title':
            return self._chrome.title

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

