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

import os

import gtk

from hulahop._hulahop import shutdown
from hulahop import _hulahop

_XO_DPI = 200.0

def startup(profile_path, components_dirs=[]):
    _hulahop.set_profile_path(profile_path)
    if not os.path.isdir(profile_path):
        try:
            os.makedirs(profile_path)
        except OSError, exc:
            raise RuntimeError('Could not create user directory.')

    for path in components_dirs:
        _hulahop.add_components_path(path)
        
    _hulahop.startup()

    from xpcom import components

    cls = components.classes["@mozilla.org/preferences-service;1"]
    prefService = cls.getService(components.interfaces.nsIPrefService)
    branch = prefService.getBranch('')
    branch.setIntPref('layout.css.dpi', _get_layout_dpi())

def _get_layout_dpi():
    _screen_dpi = gtk.settings_get_default().get_property('gtk-xft-dpi')
    screen_width = gtk.gdk.screen_width()

    if _screen_dpi != _XO_DPI or screen_width != 1200:
        return -1

    # Layout:
    # 1024x768 -> (96 * 6) / 1024 * 201 = 113 dpi
    # 800x600  -> (96 * 6) /  800 * 201 = 144 dpi
    #
    # Fonts:
    # 7  pt ->  7 / 12 * 201 = 117 dpi
    # 8  pt ->  8 / 12 * 201 = 134 dpi
    # 9  pt ->  9 / 12 * 201 = 150 dpi

    return 134
