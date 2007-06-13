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
                       interfaces.nsIWebBrowserChrome2,     \
                       interfaces.nsIEmbeddingSiteWindow,   \
                       interfaces.nsIInterfaceRequestor

    def __init__(self, web_view):
        self.web_view = web_view
        self.title = ''

    # nsIWebBrowserChrome
    def destroyBrowserWindow(self):
        logging.debug("nsIWebBrowserChrome.destroyBrowserWindow")
        self.web_view.get_toplevel().destroy()
    
    def exitModalEventLoop(self, status):
        logging.debug("UNIMPLEMENTED: nsIWebBrowserChrome.exitModalEventLoop")
        """
        if self._modal:
            self.web_view.get_toplevel().grab_remove()
            self._modal = False
            gtk.main_quit()

            #self.web_view.pop_js_context()
        """

    def isWindowModal(self):
        logging.debug("UNIMPLEMENTED: nsIWebBrowserChrome.isWindowModal")
        return False

    def setStatus(self, statusType, status):
        #logging.debug("nsIWebBrowserChrome.setStatus")
        self.web_view._set_status(status.encode('utf-8'))

    def showAsModal(self):
        logging.debug("UNIMPLEMENTED: nsIWebBrowserChrome.showAsModal")
        """
        self._modal = True
        self.web_view.get_toplevel().grab_add()
        
        #self.web_view.push_js_context()
        import time; time.sleep(5)
        gtk.main()
        """
        
    def sizeBrowserTo(self, cx, cy):
        logging.debug("nsIWebBrowserChrome.sizeBrowserTo: %r %r" % (cx, cy))
        self.web_view.get_toplevel().resize(cx, cy)
        self.web_view.type = WebView.TYPE_POPUP

    # nsIWebBrowserChrome2
    def setStatusWithContext(self, statusType, statusText, statusContext):
        self.web_view._set_status(statusText.encode('utf-8'))

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
        #logging.debug("nsIEmbeddingSiteWindow.get_visibility: %r" % self.web_view.get_toplevel().props.visible)
        return self.web_view.get_toplevel().props.visible

    def set_visibility(self, visibility):
        logging.debug("nsIEmbeddingSiteWindow.set_visibility: %r" % visibility)
        if visibility:
            self.web_view.show()
            self.web_view.get_toplevel().show()
        else:
            self.web_view.get_toplevel().hide()

    # nsIInterfaceRequestor
    def queryInterface(self, uuid):
        if not uuid in self._com_interfaces_:
            # Components.returnCode = Cr.NS_ERROR_NO_INTERFACE;
            logging.warning('Interface %s not implemented by this instance: %r' % (uuid, self))
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
                logging.warning('Interface %s not implemented by this instance: %r' % (uuid, self.web_view.browser))
                result = None

        return result
        
class WebView(_hulahop.WebView):

    TYPE_WINDOW = 0
    TYPE_POPUP = 1

    __gproperties__ = {
        'title' : (str, None, None, None,
                   gobject.PARAM_READABLE),
        'status' : (str, None, None, None,
                   gobject.PARAM_READABLE)
    }
    def __init__(self):
        _hulahop.WebView.__init__(self)
        
        self.type = WebView.TYPE_WINDOW
        
        self._chrome = xpcom.server.WrapObject(
                    _Chrome(self), interfaces.nsIEmbeddingSiteWindow)
        weak_ref = xpcom.client.WeakReference(self._chrome)
        self.browser.containerWindow = self._chrome

        self._status = ''

        self.create_window()

    def _notify_title_changed(self):
        self.notify('title')

    def _set_status(self, status):
        self._status = status
        self.notify('status')

    def do_get_property(self, pspec):
        if pspec.name == 'title':
            return self._chrome.title
        elif pspec.name == 'status':
            return self._status

    def get_window_root(self):
        return _hulahop.WebView.get_window_root(self)

    def get_browser(self):
        return _hulahop.WebView.get_browser(self)

    def get_doc_shell(self):
        requestor = self.browser.queryInterface(interfaces.nsIInterfaceRequestor)
        return requestor.getInterface(interfaces.nsIDocShell)

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

