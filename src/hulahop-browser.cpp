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
#include <nsILocalFile.h>
#include <nsIBaseWindow.h>
#include <nsXULAppAPI.h>
#include <PyXPCOM.h>

#include "hulahop-browser.h"
#include "HulahopDirectoryProvider.h"

struct _HulahopBrowser {
	GtkBin base_instance;
	nsCOMPtr<nsIWebBrowser>  browser;
	nsCOMPtr<nsIBaseWindow>  base_window;
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
    
    return TRUE;
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

    nsresult rv;
    
    browser->browser = do_CreateInstance
            ("@mozilla.org/embedding/browser/nsWebBrowser;1");
    g_assert(browser->browser);

    nsCOMPtr<nsIDocShellTreeItem> item = do_QueryInterface(browser->browser);
    item->SetItemType(nsIDocShellTreeItem::typeContentWrapper);

    browser->base_window = do_QueryInterface(browser->browser);
        
    rv = browser->base_window->InitWindow(widget, nsnull, 0, 0,
                                          widget->allocation.width,
                                          widget->allocation.height);
    g_assert(NS_SUCCEEDED(rv));

    rv = browser->base_window->Create();
    g_assert(NS_SUCCEEDED(rv));
}

static void
hulahop_browser_map(GtkWidget *widget)
{
    HulahopBrowser *browser = HULAHOP_BROWSER(widget);

    GTK_WIDGET_SET_FLAGS(widget, GTK_MAPPED);

    nsCOMPtr<nsIBaseWindow> baseWindow = do_QueryInterface(browser->browser);

    // XXX hack around problem. probably widget/gtk2 window initialization.
    baseWindow->SetVisibility(PR_FALSE);
    baseWindow->SetVisibility(PR_TRUE);

    gdk_window_show(widget->window);
}

static void
hulahop_browser_class_init(HulahopBrowserClass *browser_class)
{
    GtkWidgetClass  *widget_class = GTK_WIDGET_CLASS(browser_class);

    parent_class = (GObjectClass *) g_type_class_peek_parent(browser_class);

    widget_class->realize = hulahop_browser_realize;
    widget_class->map = hulahop_browser_map;
}

static void
hulahop_browser_init(HulahopBrowser *browser)
{
    GTK_WIDGET_UNSET_FLAGS(GTK_WIDGET(browser), GTK_NO_WINDOW);
}

PyObject *
hulahop_browser_get_browser (HulahopBrowser *browser)
{
    return PyObject_FromNSInterface(browser->browser,
                                    NS_GET_IID(nsIWebBrowser), PR_FALSE);
}
