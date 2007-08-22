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

    nsCOMPtr<nsILocalFile> greDir;
    rv = NS_NewNativeLocalFile(nsCString(LIBXUL_DIR), PR_TRUE,
                               getter_AddRefs(greDir));
    NS_ENSURE_SUCCESS(rv, FALSE);

    nsCOMPtr<nsILocalFile> binDir;
    rv = NS_NewNativeLocalFile(nsCString(LIBXUL_DIR"/components"), PR_TRUE,
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
