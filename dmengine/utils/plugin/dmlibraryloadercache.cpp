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
#include "dmlibraryloadercache.h"
#if defined (DM_OS_WIN)
#include "private/dmdllloader_win.h"
#elif defined (DM_OS_LINUX)
#include "private/dmdllloader_unix.h"
#endif
#include "dmhash.h"
#include "dmrefptr.h"
#include "dmlogger.h"

DM_BEGIN_NAMESPACE

struct LoaderPtr : public RefPtr<LoaderPtr>
{
    LoaderPtr() : loader(NULL) {}
    LoaderPtr(LibraryLoader *l) : loader(l) {}
    ~LoaderPtr();
    LibraryLoader *loader;
};

inline LoaderPtr::~LoaderPtr()
{
    if (loader)
        loader->unload();
    DM_SAFE_DELETE(loader);
}

class LibraryLoaderCachePrivate
{
public:
    Hash<UrlString, LoaderPtr*> map;
};

LibraryLoaderCache::LibraryLoaderCache() :
    C_D(LibraryLoaderCache)
{
 
}

LibraryLoaderCache::~LibraryLoaderCache()
{
    Hash<UrlString, LoaderPtr*>::Iterator it = pdm->map.begin();
    while (it != pdm->map.constEnd())
    {
        LoaderPtr *loader = *it;
        if (loader->refCount() > 1)
        {
            DM_LOGW("Library Loader reference count is %d", loader->refCount());
        }
        loader->loader->unload();
        DM_SAFE_DELETE(loader);
    }
    pdm->map.clear();
    D_D(LibraryLoaderCache);
}

LibraryLoader* LibraryLoaderCache::loadLibrary(const UrlString &libraryFile)
{
    if (libraryFile.isEmpty())
        return NULL;

    LoaderPtr *loaderPtr = pdm->map.value(libraryFile, NULL);
    if (loaderPtr)
    {
        loaderPtr->retain();
        return loaderPtr->loader;
    }
    else
    {
        LibraryLoader *libLoader = LibraryLoaderFactory::create(libraryFile);
        if (libLoader)
        {
            if (libLoader->load())
            {
                loaderPtr = new LoaderPtr(libLoader);
                pdm->map.insertMulti(libraryFile, loaderPtr);
                //NOTICE: ÓÉÎö¹¹ÊÍ·Å
                //loaderPtr->retain();
                return libLoader;
            }
            DM_SAFE_DELETE(libLoader);
        }
    }

    return NULL;
}

dbool LibraryLoaderCache::unloadLibrary(const UrlString &libraryFile)
{
    if (libraryFile.isEmpty())
        return false;

    LoaderPtr *loaderPtr = pdm->map.value(libraryFile, NULL);
    if (loaderPtr)
    {
        if (loaderPtr->refCount() == 1)
        {
            DM_LOGW("Library Loader is deleted in unloadLibrary", loaderPtr);
            loaderPtr->loader->unload();
            pdm->map.remove(libraryFile);
        }
        loaderPtr->release();
        return true;
    }
    return false;
}

dbool LibraryLoaderCache::unloadLibrary(LibraryLoader *loader)
{
    if (!loader)
        return false;

    Hash<UrlString, LoaderPtr*>::Iterator it = pdm->map.begin();
    while (it != pdm->map.constEnd())
    {
        if ((*it)->loader == loader)
        {
            LoaderPtr *loaderPtr = *it;
            if (loaderPtr->refCount() == 1)
            {
                loaderPtr->loader->unload();
                pdm->map.erase(it);
            }
            loaderPtr->release();
            return true;
        }
        ++it;
    }
    return false;
}
/*
void LibraryLoaderCache::unloadAll()
{
    Hash<UrlString, LoaderPtr*>::Iterator it = pdm->map.begin();
    while (it != pdm->map.constEnd())
    {
        LoaderPtr *loader = *it;
        if (loader->refCount() > 1)
        {
            DM_LOGW("Library Loader reference count is %d", loader->refCount());
        }
        DM_SAFE_DELETE(loader);
    }
    pdm->map.clear();
}
*/
UrlStringList LibraryLoaderCache::cachedLibs() const
{
    UrlStringList list;
    Hash<UrlString, LoaderPtr*>::Iterator it = pdm->map.begin();
    while (it != pdm->map.constEnd())
    {
        list.append(it.key());
    }
    return list;
}

LibraryLoader* LibraryLoaderFactory::create(const UrlString &libraryFile)
{
#if defined (DM_OS_WIN)
    return new DllLoaderWin(libraryFile);
#elif defined (DM_OS_LINUX)
    return new DllLoaderUnix(libraryFile);
#endif
}
DM_END_NAMESPACE