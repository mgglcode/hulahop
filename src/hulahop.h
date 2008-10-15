/*
 * Copyright (C) 2006, Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __HULAHOP_H__
#define __HULAHOP_H__

#include "hulahop-web-view.h"

G_BEGIN_DECLS

gboolean        hulahop_startup             (void);
void            hulahop_shutdown            (void);
void            hulahop_set_profile_path    (const char *path);
void            hulahop_add_components_path (const char *path);
HulahopWebView *hulahop_get_view_for_window (PyObject   *dom_window);

G_END_DECLS

#endif
