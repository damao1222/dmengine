/*
   Copyright (C) 2012-2014 Xiongfa Li, <damao1222@live.com>
   All rights reserved.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "dmlibrary.h"
#include "private/dmlibrary_p.h"
#include "dmlibraryloader.h"
#include "dmlibraryloadercache.h"
#include "dmlibrarydef.h"

DM_BEGIN_NAMESPACE
class LibraryDataPrivate
{
public:
    UrlString                        libPath;
    LibraryLoader*              loader;
    void*                               clazz;
    dm_create_clazz_t*       createFunc;
    dm_destroy_clazz_t*     destroyFunc;
    dm_version_clazz_t*     versionFunc;
};

LibraryData::LibraryData(const char *path):
    C_D(LibraryData)
{
    pdm->libPath = path;
    pdm->loader = NULL;
    pdm->clazz = NULL;
    pdm->createFunc = NULL;
    pdm->destroyFunc = NULL;
    pdm->versionFunc = NULL;
}

LibraryData::~LibraryData()
{
    D_D(LibraryData);
}

dbool LibraryData::load()
{
    if (pdm->clazz)
        return true;

    if (!pdm->loader)
        pdm->loader = dmLibLoaderCache.loadLibrary(pdm->libPath);

    if (pdm->loader)
    {
        dbool loadRet = false;
        void* ptr = NULL;
        dint ret = pdm->loader->resolveExport(DM_LIBRARY_CLAZZ_CREATE, &ptr);
        if (ret == eLibResolveSuccess)
        {
            pdm->createFunc = (dm_create_clazz_t*)ptr;
            if (pdm->createFunc != NULL)
            {
                pdm->clazz = (pdm->createFunc)();
                loadRet = true;
            }
        }

        ptr = NULL;
        ret = pdm->loader->resolveExport(DM_LIBRARY_CLAZZ_DESTROY, &ptr);
        if (ret == eLibResolveSuccess)
        {
            pdm->destroyFunc = (dm_destroy_clazz_t*)ptr;
        }

        ptr = NULL;
        ret = pdm->loader->resolveExport(DM_LIBRARY_CLAZZ_VERSION, &ptr);
        if (ret == eLibResolveSuccess)
        {
            pdm->versionFunc = (dm_version_clazz_t*)ptr;
        }

        return loadRet;
    }
    return false;
}

void LibraryData::unload()
{
    if (pdm->clazz != NULL)
    {
        if (pdm->destroyFunc != NULL)
        {
            (pdm->destroyFunc)(pdm->clazz);
            //d->clazz = NULL;
        }
        //FIXME:在此处设空？
        pdm->clazz = NULL;
    }

    dmLibLoaderCache.unloadLibrary(pdm->loader);
}

dint LibraryData::version() const
{
    return pdm->versionFunc ? (pdm->versionFunc)() : 0;
}

DM_END_NAMESPACE