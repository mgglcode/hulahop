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

#include <nsILocalFile.h>
#include <nsAppDirectoryServiceDefs.h>
#include <nsDirectoryServiceDefs.h>
#include <nsArrayEnumerator.h>
#include <nsIToolkitChromeRegistry.h>
#include <nsNetUtil.h>
#include <nsISupportsArray.h>
#include <nsIMutableArray.h>
#include <nsXULAppAPI.h>

#include "HulahopDirectoryProvider.h"

NS_IMPL_QUERY_INTERFACE2(HulahopDirectoryProvider,
                         nsIDirectoryServiceProvider,
                         nsIDirectoryServiceProvider2)

NS_IMETHODIMP_(nsrefcnt)
HulahopDirectoryProvider::AddRef()
{ 
    return 1;
}

NS_IMETHODIMP_(nsrefcnt)
HulahopDirectoryProvider::Release()
{ 
    return 1;
}

NS_IMETHODIMP
HulahopDirectoryProvider::GetFile(const char *aKey,
                                  PRBool *aPersist,
                                  nsIFile **aResult)
{
    nsresult rv;

    // All our paths are currently persistent
    *aPersist = PR_TRUE;

    if ((!strcmp(aKey, NS_APP_USER_PROFILE_50_DIR) ||
         !strcmp(aKey, NS_APP_USER_PROFILE_LOCAL_50_DIR) ||
         !strcmp(aKey, NS_APP_PROFILE_DIR_STARTUP)) && mProfilePath) {
        nsCOMPtr<nsIFile> file;
        rv = mProfilePath->Clone(getter_AddRefs(file));
        NS_ENSURE_SUCCESS(rv, NS_ERROR_FAILURE);

        NS_ADDREF(*aResult = file);
        return NS_OK;
    }

    if (!strcmp(aKey, NS_XPCOM_COMPONENT_REGISTRY_FILE) && mProfilePath) {
        nsCOMPtr<nsIFile> file;
        rv = mProfilePath->Clone(getter_AddRefs(file));
        NS_ENSURE_SUCCESS(rv, NS_ERROR_FAILURE);
        
        rv = file->AppendNative(nsCString("compreg.dat"));
        NS_ENSURE_SUCCESS(rv, NS_ERROR_FAILURE);

        NS_ADDREF(*aResult = file);
        return NS_OK;
    }

    if (!strcmp(aKey, NS_XPCOM_XPTI_REGISTRY_FILE) && mProfilePath) {
        nsCOMPtr<nsIFile> file;
        rv = mProfilePath->Clone(getter_AddRefs(file));
        NS_ENSURE_SUCCESS(rv, NS_ERROR_FAILURE);
        
        rv = file->AppendNative(nsCString("xpti.dat"));
        NS_ENSURE_SUCCESS(rv, NS_ERROR_FAILURE);

        NS_ADDREF(*aResult = file);
        return NS_OK;
    }

    if (!strcmp(aKey, NS_APP_PREF_DEFAULTS_50_DIR)) {
        nsCOMPtr<nsILocalFile> dataDir;
        NS_NewNativeLocalFile(nsCString(DATA_DIR),
                              PR_TRUE, getter_AddRefs(dataDir));

        NS_ADDREF(*aResult = dataDir);
        return NS_OK;
    }

    if (!strcmp(aKey, NS_APP_PREFS_50_FILE)) {
        nsCOMPtr<nsIFile> file;
        rv = mProfilePath->Clone(getter_AddRefs(file));
        NS_ENSURE_SUCCESS(rv, NS_ERROR_FAILURE);

        rv = file->AppendNative(nsCString("prefs.js"));
        NS_ENSURE_SUCCESS(rv, NS_ERROR_FAILURE);

        nsCAutoString inDir;
        file->GetNativePath(inDir);
        printf("returning %s for key NS_APP_PREFS_50_FILE\n", inDir.get());fflush(stdout);
        NS_ADDREF(*aResult = file);
        return NS_OK;
    }

    if (!strcmp(aKey, XRE_USER_SYS_EXTENSION_DIR) && mProfilePath) {
        nsCOMPtr<nsIFile> file;
        rv = mProfilePath->Clone(getter_AddRefs(file));
        NS_ENSURE_SUCCESS(rv, NS_ERROR_FAILURE);

        rv = file->AppendNative(nsCString("extensions"));
        NS_ENSURE_SUCCESS(rv, NS_ERROR_FAILURE);

        NS_ADDREF(*aResult = file);
        return NS_OK;
    }
    
    return NS_ERROR_FAILURE;
}

NS_IMETHODIMP
HulahopDirectoryProvider::GetFiles(const char *aKey,
                                   nsISimpleEnumerator **aResult)
{
    nsresult rv = NS_ERROR_FAILURE;

    if (!strcmp(aKey, NS_XPCOM_COMPONENT_DIR_LIST)) {  
        rv = NS_NewArrayEnumerator(aResult, mComponentsDirs);
    } else if (!strcmp(aKey, NS_CHROME_MANIFESTS_FILE_LIST)) {
        nsCOMPtr<nsILocalFile> manifestDir;
        rv = NS_NewNativeLocalFile (nsDependentCString(DATA_DIR "/chrome"), PR_TRUE,
                                    getter_AddRefs (manifestDir));
        NS_ENSURE_SUCCESS (rv, rv);

        nsCOMPtr<nsISupports> element (do_QueryInterface (manifestDir, &rv));
        NS_ENSURE_SUCCESS (rv, rv);

        nsCOMPtr<nsIMutableArray> array (do_CreateInstance(NS_ARRAY_CONTRACTID));
        if(!array)
                return NS_ERROR_OUT_OF_MEMORY;
        rv = array->AppendElement (manifestDir, PR_FALSE);
        NS_ENSURE_SUCCESS (rv, rv);

        rv = array->Enumerate(aResult);

        NS_ENSURE_SUCCESS (rv, rv);

        rv = NS_SUCCESS_AGGREGATE_RESULT;
    }

    return rv;
}

void
HulahopDirectoryProvider::SetProfilePath(const char *path)
{
    NS_NewNativeLocalFile(nsCString(path),
                          PR_TRUE, getter_AddRefs(mProfilePath));
}

void
HulahopDirectoryProvider::AddComponentsPath(const char *path)
{
    nsresult rv;

    nsCOMPtr<nsILocalFile> localFile;
    rv = NS_NewNativeLocalFile(nsCString(path),
                               PR_TRUE, getter_AddRefs(localFile));
    if (localFile) {
        mComponentsDirs.AppendObject(localFile);
    }
}

