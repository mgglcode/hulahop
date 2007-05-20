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
#include <nsILocalFile.h>
#include <nsIBaseWindow.h>
#include <nsXULAppAPI.h>
#include <PyXPCOM.h>

#include <gtk/gtkfixed.h>
#include <gtk/gtkwindow.h>

#include "hulahop-browser.h"
#include "HulahopDirectoryProvider.h"

struct _HulahopBrowser {
	GtkBin base_instance;

	nsCOMPtr<nsIWebBrowser>  browser;
	nsCOMPtr<nsIBaseWindow>  base_window;

    GtkWidget *offscreen_window;
    GtkWidget *mozilla_widget;
};

struct _HulahopBrowserClass {
	GtkBinClass base_class;
};

G_DEFINE_TYPE(HulahopBrowser, hulahop_browser, GTK_TYPE_BIN)

static GObjectClass *parent_class = NULL;

static const HulahopDirectoryProvider kDirectoryProvider;

gboolean
hulahop_startup()
{
    nsresult rv;

    nsCOMPtr<nsILocalFile> greDir;
    rv = NS_NewNativeLocalFile(nsCString(MOZILLA_HOME), PR_TRUE,
                               getter_AddRefs(greDir));
    NS_ENSURE_SUCCESS(rv, FALSE);

    nsCOMPtr<nsILocalFile> binDir;
    rv = NS_NewNativeLocalFile(nsCString(MOZILLA_HOME"/components"), PR_TRUE,
                               getter_AddRefs(binDir));
    NS_ENSURE_SUCCESS(rv, FALSE);

    rv = XRE_InitEmbedding(greDir, binDir,
                           NS_CONST_CAST(HulahopDirectoryProvider *,
                                    &kDirectoryProvider), nsnull, 0);
    NS_ENSURE_SUCCESS(rv, FALSE);
    
    XRE_NotifyProfile();
    
    return TRUE;
}

void
hulahop_shutdown()
{
    XRE_TermEmbedding();
}

static gboolean
child_focus_in_cb(GtkWidget      *widget,
                  GdkEventFocus  *event,
                  HulahopBrowser *browser)
{
    nsCOMPtr<nsIWebBrowserFocus> webBrowserFocus;
    webBrowserFocus = do_QueryInterface(browser->browser);
    NS_ENSURE_TRUE(webBrowserFocus, FALSE);

    webBrowserFocus->Activate();

    return FALSE;
}

static gboolean
child_focus_out_cb(GtkWidget      *widget,
                   GdkEventFocus  *event,
                   HulahopBrowser *browser)
{
    nsCOMPtr<nsIWebBrowserFocus> webBrowserFocus;
    webBrowserFocus = do_QueryInterface(browser->browser);
    NS_ENSURE_TRUE(webBrowserFocus, FALSE);

    webBrowserFocus->Deactivate();

    return FALSE;
}

static void
hulahop_browser_unrealize(GtkWidget *widget)
{
    HulahopBrowser *browser = HULAHOP_BROWSER(widget);

    gtk_widget_reparent(browser->mozilla_widget, browser->offscreen_window);
    
    GTK_WIDGET_CLASS(parent_class)->unrealize(widget);
}

static void
hulahop_browser_realize(GtkWidget *widget)
{
    HulahopBrowser *browser = HULAHOP_BROWSER(widget);
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

    g_assert(GTK_IS_WIDGET(browser->mozilla_widget));

    gtk_widget_reparent(browser->mozilla_widget, widget);

    g_signal_connect_object(browser->mozilla_widget,
                            "focus-in-event",
                            G_CALLBACK(child_focus_in_cb),
                            browser, (GConnectFlags)0);
    g_signal_connect_object(browser->mozilla_widget,
                            "focus-out-event",
                            G_CALLBACK(child_focus_out_cb),
                            browser, (GConnectFlags)0);
}

static void
hulahop_browser_map(GtkWidget *widget)
{
    HulahopBrowser *browser = HULAHOP_BROWSER(widget);

    GTK_WIDGET_SET_FLAGS(widget, GTK_MAPPED);

    browser->base_window->SetVisibility(PR_TRUE);

    gdk_window_show(widget->window);
}

static void
hulahop_browser_unmap(GtkWidget *widget)
{
    HulahopBrowser *browser = HULAHOP_BROWSER(widget);

    GTK_WIDGET_UNSET_FLAGS(widget, GTK_MAPPED);

    browser->base_window->SetVisibility(PR_FALSE);

    gdk_window_hide(widget->window);
}

static void
hulahop_browser_size_allocate(GtkWidget *widget, GtkAllocation *allocation)
{
    HulahopBrowser *browser = HULAHOP_BROWSER(widget);

    if (GTK_WIDGET_REALIZED(widget)) {
        gdk_window_move_resize(widget->window,
                               allocation->x, allocation->y,
                               allocation->width, allocation->height);

        browser->base_window->SetSize(allocation->width,
                                      allocation->height, PR_TRUE);
    }
}

static void
hulahop_browser_dispose(GObject *object)
{
    HulahopBrowser *browser = HULAHOP_BROWSER(object);
    
    if (browser->offscreen_window) {
        gtk_widget_destroy(browser->offscreen_window);
        browser->offscreen_window = NULL;
    }
}

static void
hulahop_browser_class_init(HulahopBrowserClass *browser_class)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(browser_class);
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(browser_class);

    parent_class = (GObjectClass *) g_type_class_peek_parent(browser_class);

    widget_class->realize = hulahop_browser_realize;
    widget_class->unrealize = hulahop_browser_unrealize;    
    widget_class->map = hulahop_browser_map;
    widget_class->unmap = hulahop_browser_unmap;
    widget_class->size_allocate = hulahop_browser_size_allocate;

    gobject_class->dispose = hulahop_browser_dispose;    
}

static void
hulahop_browser_init(HulahopBrowser *browser)
{
    browser->offscreen_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_realize(browser->offscreen_window);

    nsresult rv;
    
    browser->browser = do_CreateInstance
            ("@mozilla.org/embedding/browser/nsWebBrowser;1");
    g_assert(browser->browser);

    nsCOMPtr<nsIDocShellTreeItem> item = do_QueryInterface(browser->browser);
    item->SetItemType(nsIDocShellTreeItem::typeContentWrapper);

    browser->base_window = do_QueryInterface(browser->browser);

    rv = browser->base_window->InitWindow(browser->offscreen_window,
                                          nsnull, 0, 0, 100, 100);
    g_assert(NS_SUCCEEDED(rv));

    rv = browser->base_window->Create();
    g_assert(NS_SUCCEEDED(rv));

    browser->mozilla_widget = GTK_BIN(browser->offscreen_window)->child;

    GTK_WIDGET_UNSET_FLAGS(GTK_WIDGET(browser), GTK_NO_WINDOW);
}

PyObject *
hulahop_browser_get_browser (HulahopBrowser *browser)
{
    return PyObject_FromNSInterface(browser->browser,
                                    NS_GET_IID(nsIWebBrowser));
}
