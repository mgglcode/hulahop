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

    if (!strcmp(aKey, NS_APP_USER_PROFILE_50_DIR) && mProfilePath) {
        NS_ADDREF(*aResult = mProfilePath);
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
        
    return NS_ERROR_FAILURE;
}

NS_IMETHODIMP
HulahopDirectoryProvider::GetFiles(const char *aKey,
                                   nsISimpleEnumerator **aResult)
{
    nsresult rv = NS_ERROR_FAILURE;

    if (!strcmp(aKey, NS_XPCOM_COMPONENT_DIR_LIST)) {  
        rv = NS_NewArrayEnumerator(aResult, mComponentsDirs);
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

