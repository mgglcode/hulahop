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

#include <nsCOMPtr.h>
#include <nsILocalFile.h>
#include <nsXULAppAPI.h>
#include <nsIWindowWatcher.h>
#include <nsIWebBrowser.h>
#include <nsIWebBrowserChrome.h>
#include <nsIDOMWindow.h>
#include <nsIBaseWindow.h>
#include <pyxpcom/PyXPCOM.h>

#include "HulahopDirectoryProvider.h"
#include "hulahop.h"

static HulahopDirectoryProvider kDirectoryProvider;

static void
setup_plugin_path()
{
    const char *user_path;
    char *new_path;

    user_path = g_getenv ("MOZ_PLUGIN_PATH");
    new_path = g_strconcat(user_path ? user_path : "",
                           user_path ? ":" : "",
                           PLUGIN_PATH,
                           (char *) NULL);
    g_setenv ("MOZ_PLUGIN_PATH", new_path, TRUE);
    g_free (new_path);
}

gboolean
hulahop_startup()
{
    nsresult rv;

    setup_plugin_path();

    hulahop_add_components_path(LIB_DIR"/components");

    nsCOMPtr<nsILocalFile> greDir;
    rv = NS_NewNativeLocalFile(nsCString(LIBXUL_DIR), PR_TRUE,
                               getter_AddRefs(greDir));
    NS_ENSURE_SUCCESS(rv, FALSE);

    nsCOMPtr<nsILocalFile> binDir;
    rv = NS_NewNativeLocalFile(nsCString(LIBXUL_DIR), PR_TRUE,
                               getter_AddRefs(binDir));
    NS_ENSURE_SUCCESS(rv, FALSE);

    rv = XRE_InitEmbedding(greDir, binDir,
                           const_cast<HulahopDirectoryProvider *>
                                    (&kDirectoryProvider), nsnull, 0);
    NS_ENSURE_SUCCESS(rv, FALSE); 
   
    XRE_NotifyProfile();
    
    return TRUE;
}

void
hulahop_shutdown()
{
    XRE_TermEmbedding();
}

void
hulahop_set_profile_path(const char *path)
{
    kDirectoryProvider.SetProfilePath(path);
}

void
hulahop_add_components_path(const char *path)
{
    kDirectoryProvider.AddComponentsPath(path);
}

HulahopWebView *
hulahop_get_view_for_window(PyObject *dom_window)
{
    nsCOMPtr<nsIDOMWindow> domWindow;
    Py_nsISupports::InterfaceFromPyObject(dom_window,
                                          NS_GET_IID(nsIDOMWindow),
                                          getter_AddRefs(domWindow),
                                          PR_FALSE);
    NS_ENSURE_TRUE(domWindow, NULL);

    nsCOMPtr<nsIDOMWindow> topDomWindow;
    domWindow->GetTop(getter_AddRefs(topDomWindow));
    NS_ENSURE_TRUE(topDomWindow, NULL);

    nsCOMPtr<nsIWindowWatcher> wwatch = do_GetService
                        ("@mozilla.org/embedcomp/window-watcher;1");
    NS_ENSURE_TRUE(wwatch, NULL);

    nsCOMPtr<nsIWebBrowserChrome> chrome;
    wwatch->GetChromeForWindow(topDomWindow, getter_AddRefs(chrome));
    NS_ENSURE_TRUE(chrome, NULL);

    nsCOMPtr<nsIWebBrowser> browser;
    chrome->GetWebBrowser(getter_AddRefs(browser));
    NS_ENSURE_TRUE(browser, NULL);

    nsCOMPtr<nsIBaseWindow> baseWindow = do_QueryInterface(browser);
    NS_ENSURE_TRUE(baseWindow, NULL);

    gpointer native_parent;
    baseWindow->GetParentNativeWindow(&native_parent);

    return HULAHOP_WEB_VIEW(native_parent);
}
