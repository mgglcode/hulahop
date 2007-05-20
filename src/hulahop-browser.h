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

#ifndef __HULAHOP_BROWSER_H__
#define __HULAHOP_BROWSER_H__

#include <Python.h>
#include <gtk/gtkbin.h>

G_BEGIN_DECLS

typedef struct _HulahopBrowser            HulahopBrowser;
typedef struct _HulahopBrowserClass       HulahopBrowserClass;

#define HULAHOP_TYPE_BROWSER		      (hulahop_browser_get_type())
#define HULAHOP_BROWSER(object)	    	  (G_TYPE_CHECK_INSTANCE_CAST((object), HULAHOP_TYPE_BROWSER, HulahopBrowser))
#define HULAHOP_BROWSER_CLASS(klass)	  (G_TYPE_CHECK_CLASS_CAST((klass), HULAHOP_TYPE_BROWSER, HulahopBrowserClass))
#define HULAHOP_IS_BROWSER(object)		  (G_TYPE_CHECK_INSTANCE_TYPE((object), HULAHOP_TYPE_BROWSER))
#define HULAHOP_IS_BROWSER_CLASS(klass)	  (G_TYPE_CHECK_CLASS_TYPE((klass), HULAHOP_TYPE_BROWSER))
#define HULAHOP_BROWSER_GET_CLASS(object) (G_TYPE_INSTANCE_GET_CLASS((object), HULAHOP_TYPE_BROWSER, HulahopBrowserClass))

gboolean  hulahop_startup             (void);
void      hulahop_shutdown            (void);

GType     hulahop_browser_get_type    (void);
PyObject *hulahop_browser_get_browser (HulahopBrowser *browser);

G_END_DECLS

#endif
