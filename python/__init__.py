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
import logging
import sys
import ConfigParser

import gtk

from hulahop import config

sys.path.insert(0, os.path.join(config.libxul_dir, 'python'))

from hulahop._hulahop import shutdown
from hulahop._hulahop import get_view_for_window
from hulahop import _hulahop

_XO_DPI = 200

_app_version = ''

def set_app_version(version):
    global _app_version
    _app_version = version

def startup(profile_path, components_dirs=[]):
    _hulahop.set_profile_path(profile_path)
    if not os.path.isdir(profile_path):
        try:
            os.makedirs(profile_path)
        except OSError, exc:
            raise RuntimeError('Could not create user directory.')

    _check_compreg(profile_path)

    for path in components_dirs:
        _hulahop.add_components_path(path)
        
    _hulahop.startup()

    from xpcom import components

    cls = components.classes["@mozilla.org/preferences-service;1"]
    prefService = cls.getService(components.interfaces.nsIPrefService)
    branch = prefService.getBranch('')
    branch.setIntPref('layout.css.dpi', _get_layout_dpi())

def _check_compreg(profile_path):
    comp_path = os.path.join(profile_path, 'compatibility.ini')

    try:
        cp = ConfigParser.ConfigParser(defaults={'app_version' : ''})
        if not cp.read(comp_path):
            valid = False
        else:
            valid = cp.get('main', 'app_version') == _app_version and \
                    cp.get('main', 'libxul_dir') == config.libxul_dir
    except ConfigParser.Error:
        valid = False
        
    if not valid:
        compreg = os.path.join(profile_path, 'compreg.dat')        
        if os.path.exists(compreg):
            try:
                os.unlink(compreg)
            except OSError, e:
                logging.error('Unlink error: %s' % e)
            
        cp = ConfigParser.ConfigParser()
        cp.add_section('main')
        cp.set('main', 'app_version', _app_version)
        cp.set('main', 'libxul_dir', config.libxul_dir)
    
        f = open(comp_path, 'w')
        try:
            cp.write(f)
        except ConfigParser.Error, e:
            logging.error('Can not write %s error: %s' % (comp_path, e))
        f.close()

def _get_layout_dpi():
    gtk_xft_dpi = gtk.settings_get_default().get_property('gtk-xft-dpi')
    screen_dpi = gtk_xft_dpi / 1024
    screen_width = gtk.gdk.screen_width()

    if screen_dpi != _XO_DPI or screen_width != 1200:
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
