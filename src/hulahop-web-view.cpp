/*
 * Copyright (C) 2007, Red Hat, Inc.
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

#include <nsIDocShellTreeItem.h>
#include <nsComponentManagerUtils.h>
#include <nsCOMPtr.h>
#include <nsIWebBrowser.h>
#include <nsIWebBrowserFocus.h>
#include <nsIWebBrowserChrome.h>
#include <nsIEmbeddingSiteWindow.h>
#include <nsIDOMWindow2.h>
#include <nsIDOMEventTarget.h>
#include <nsIBaseWindow.h>
#include <nsIDocShell.h>
#include <nsIInterfaceRequestorUtils.h>
#include <PyXPCOM.h>

#include <gtk/gtkfixed.h>
#include <gtk/gtkwindow.h>

#include "hulahop-web-view.h"

struct _HulahopWebView {
	GtkBin base_instance;

	nsCOMPtr<nsIWebBrowser>  browser;
	nsCOMPtr<nsIBaseWindow>  base_window;

    GtkWidget *offscreen_window;
    GtkWidget *mozilla_widget;
};

struct _HulahopWebViewClass {
	GtkBinClass base_class;
};

G_DEFINE_TYPE(HulahopWebView, hulahop_web_view, GTK_TYPE_BIN)

static GObjectClass *parent_class = NULL;

static gboolean
child_focus_in_cb(GtkWidget      *widget,
                  GdkEventFocus  *event,
                  HulahopWebView *web_view)
{
    nsCOMPtr<nsIWebBrowserFocus> webBrowserFocus;
    webBrowserFocus = do_QueryInterface(web_view->browser);
    NS_ENSURE_TRUE(webBrowserFocus, FALSE);

    webBrowserFocus->Activate();

    return FALSE;
}

static gboolean
child_focus_out_cb(GtkWidget      *widget,
                   GdkEventFocus  *event,
                   HulahopWebView *web_view)
{
    nsCOMPtr<nsIWebBrowserFocus> webBrowserFocus;
    webBrowserFocus = do_QueryInterface(web_view->browser);
    NS_ENSURE_TRUE(webBrowserFocus, FALSE);

    webBrowserFocus->Deactivate();

    return FALSE;
}

static void
hulahop_web_view_unrealize(GtkWidget *widget)
{
    HulahopWebView *web_view = HULAHOP_WEB_VIEW(widget);

    gtk_widget_reparent(web_view->mozilla_widget, web_view->offscreen_window);
    
    GTK_WIDGET_CLASS(parent_class)->unrealize(widget);
}

static void
hulahop_web_view_realize(GtkWidget *widget)
{
    HulahopWebView *web_view = HULAHOP_WEB_VIEW(widget);
    GdkWindowAttr attributes;
    gint attributes_mask;

    GTK_WIDGET_SET_FLAGS(widget, GTK_REALIZED);

    attributes.window_type = GDK_WINDOW_CHILD;
    attributes.x = widget->allocation.x;
    attributes.y = widget->allocation.y;
    attributes.width = widget->allocation.width;
    attributes.height = widget->allocation.height;
    attributes.wclass = GDK_INPUT_OUTPUT;
    attributes.visual = gtk_widget_get_visual(widget);
    attributes.colormap = gtk_widget_get_colormap(widget);
    attributes.event_mask = gtk_widget_get_events(widget) | GDK_EXPOSURE_MASK;

    attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;

    widget->window = gdk_window_new(gtk_widget_get_parent_window(widget),
                                    &attributes, attributes_mask);
    gdk_window_set_user_data(widget->window, widget);

    widget->style = gtk_style_attach (widget->style, widget->window);
    gtk_style_set_background(widget->style, widget->window, GTK_STATE_NORMAL);

    g_assert(GTK_IS_WIDGET(web_view->mozilla_widget));

    gtk_widget_reparent(web_view->mozilla_widget, widget);

    g_signal_connect_object(web_view->mozilla_widget,
                            "focus-in-event",
                            G_CALLBACK(child_focus_in_cb),
                            web_view, (GConnectFlags)0);
    g_signal_connect_object(web_view->mozilla_widget,
                            "focus-out-event",
                            G_CALLBACK(child_focus_out_cb),
                            web_view, (GConnectFlags)0);
}

static void
hulahop_web_view_map(GtkWidget *widget)
{
    HulahopWebView *web_view = HULAHOP_WEB_VIEW(widget);

    GTK_WIDGET_SET_FLAGS(widget, GTK_MAPPED);

    nsCOMPtr<nsIWebBrowserChrome> webBrowserChrome;
    web_view->browser->GetContainerWindow(getter_AddRefs(webBrowserChrome));
    NS_ENSURE_TRUE(webBrowserChrome, );

    nsCOMPtr<nsIEmbeddingSiteWindow> embeddingSiteWindow;
    embeddingSiteWindow = do_QueryInterface(webBrowserChrome);
    NS_ENSURE_TRUE(embeddingSiteWindow, );

    embeddingSiteWindow->SetVisibility(PR_TRUE);

    web_view->base_window->SetVisibility(PR_TRUE);

    gdk_window_show(widget->window);
}

static void
hulahop_web_view_unmap(GtkWidget *widget)
{
    HulahopWebView *web_view = HULAHOP_WEB_VIEW(widget);

    GTK_WIDGET_UNSET_FLAGS(widget, GTK_MAPPED);

    nsCOMPtr<nsIWebBrowserChrome> webBrowserChrome;
    web_view->browser->GetContainerWindow(getter_AddRefs(webBrowserChrome));
    NS_ENSURE_TRUE(webBrowserChrome, );

    nsCOMPtr<nsIEmbeddingSiteWindow> embeddingSiteWindow;
    embeddingSiteWindow = do_QueryInterface(webBrowserChrome);
    NS_ENSURE_TRUE(embeddingSiteWindow, );

    embeddingSiteWindow->SetVisibility(PR_FALSE);

    web_view->base_window->SetVisibility(PR_FALSE);

    gdk_window_hide(widget->window);
}

static void
hulahop_web_view_size_allocate(GtkWidget *widget, GtkAllocation *allocation)
{
    HulahopWebView *web_view = HULAHOP_WEB_VIEW(widget);

    if (GTK_WIDGET_REALIZED(widget)) {
        gdk_window_move_resize(widget->window,
                               allocation->x, allocation->y,
                               allocation->width, allocation->height);

        web_view->base_window->SetSize(allocation->width,
                                       allocation->height, PR_TRUE);
    }
}

static void
hulahop_web_view_dispose(GObject *object)
{
    HulahopWebView *web_view = HULAHOP_WEB_VIEW(object);
    
    if (web_view->offscreen_window) {
        gtk_widget_destroy(web_view->offscreen_window);
        web_view->offscreen_window = NULL;
    }
}

static void
hulahop_web_view_class_init(HulahopWebViewClass *web_view_class)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(web_view_class);
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(web_view_class);

    parent_class = (GObjectClass *) g_type_class_peek_parent(web_view_class);

    widget_class->realize = hulahop_web_view_realize;
    widget_class->unrealize = hulahop_web_view_unrealize;    
    widget_class->map = hulahop_web_view_map;
    widget_class->unmap = hulahop_web_view_unmap;
    widget_class->size_allocate = hulahop_web_view_size_allocate;

    gobject_class->dispose = hulahop_web_view_dispose;    
}

static void
hulahop_web_view_init(HulahopWebView *web_view)
{
    web_view->offscreen_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_realize(web_view->offscreen_window);

    nsresult rv;
    
    web_view->browser = do_CreateInstance
            ("@mozilla.org/embedding/browser/nsWebBrowser;1");
    g_assert(web_view->browser);

    nsCOMPtr<nsIDocShellTreeItem> item = do_QueryInterface(web_view->browser);
    item->SetItemType(nsIDocShellTreeItem::typeContentWrapper);

    web_view->base_window = do_QueryInterface(web_view->browser);

    rv = web_view->base_window->InitWindow(web_view->offscreen_window,
                                          nsnull, 0, 0, 100, 100);
    g_assert(NS_SUCCEEDED(rv));
}

PyObject *
hulahop_web_view_get_browser(HulahopWebView *web_view)
{
    return PyObject_FromNSInterface(web_view->browser,
                                    NS_GET_IID(nsIWebBrowser));
}

PyObject *
hulahop_web_view_get_window_root(HulahopWebView *web_view)
{
    nsresult rv;

    nsCOMPtr<nsIDOMWindow> contentWindow;
    rv = web_view->browser->GetContentDOMWindow(getter_AddRefs(contentWindow));
    NS_ENSURE_SUCCESS(rv, NULL);

    nsCOMPtr<nsIDOMWindow2> domWindow(do_QueryInterface(contentWindow));
    NS_ENSURE_TRUE (domWindow, NULL);

    nsCOMPtr<nsIDOMEventTarget> eventTarget;    
    domWindow->GetWindowRoot(getter_AddRefs(eventTarget));
    NS_ENSURE_TRUE (eventTarget, NULL);

    return PyObject_FromNSInterface(eventTarget,
                                    NS_GET_IID(nsIDOMEventTarget));
}

PyObject *
hulahop_web_view_get_doc_shell(HulahopWebView *web_view)
{
    nsresult rv;

    nsCOMPtr<nsIDocShell> docShell(do_GetInterface(web_view->browser, &rv));
    NS_ENSURE_SUCCESS (rv, NULL);

    return PyObject_FromNSInterface(docShell,
                                    NS_GET_IID(nsIDocShell));
}

void
hulahop_web_view_create_window(HulahopWebView *web_view)
{
    nsresult rv;

    rv = web_view->base_window->Create();
    g_assert(NS_SUCCEEDED(rv));

    web_view->mozilla_widget = GTK_BIN(web_view->offscreen_window)->child;

    GTK_WIDGET_UNSET_FLAGS(GTK_WIDGET(web_view), GTK_NO_WINDOW);
}
