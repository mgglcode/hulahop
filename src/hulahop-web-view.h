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

#ifndef __HULAHOP_WEB_VIEW_H__
#define __HULAHOP_WEB_VIEW_H__

#include <Python.h>
#include <gtk/gtkbin.h>

G_BEGIN_DECLS

typedef struct _HulahopWebView            HulahopWebView;
typedef struct _HulahopWebViewClass       HulahopWebViewClass;

#define HULAHOP_TYPE_WEB_VIEW              (hulahop_web_view_get_type())
#define HULAHOP_WEB_VIEW(object)           (G_TYPE_CHECK_INSTANCE_CAST((object), HULAHOP_TYPE_WEB_VIEW, HulahopWebView))
#define HULAHOP_WEB_VIEW_CLASS(klass)	   (G_TYPE_CHECK_CLASS_CAST((klass), HULAHOP_TYPE_WEB_VIEW, HulahopWebViewClass))
#define HULAHOP_IS_WEB_VIEW(object)        (G_TYPE_CHECK_INSTANCE_TYPE((object), HULAHOP_TYPE_WEB_VIEW))
#define HULAHOP_IS_WEB_VIEW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), HULAHOP_TYPE_WEB_VIEW))
#define HULAHOP_WEB_VIEW_GET_CLASS(object) (G_TYPE_INSTANCE_GET_CLASS((object), HULAHOP_TYPE_WEB_VIEW, HulahopWebViewClass))

GType     hulahop_web_view_get_type        (void);
void      hulahop_web_view_create_window   (HulahopWebView *web_view);
PyObject *hulahop_web_view_get_browser     (HulahopWebView *web_view);
PyObject *hulahop_web_view_get_window_root (HulahopWebView *web_view);
PyObject *hulahop_web_view_get_doc_shell   (HulahopWebView *web_view);

G_END_DECLS

#endif
