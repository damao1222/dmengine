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
#include "dmclassloader.h"
#include "dmlibrarydef.h"
#include "dmrefcount.h"
#include "dmhash.h"
#include "dmlogger.h"

DM_BEGIN_NAMESPACE

ClassLoaderData::Data ClassLoaderData::shared_null = { DM_BASIC_ATOMIC_INITIALIZER(1), true, NULL, NULL};

class ClassLoaderDataCache : public Singleton<ClassLoaderDataCache>
{
public:
    Hash<UrlString, ClassLoaderData::Data *> data;
};

DM_GLOBAL(ClassLoaderDataCache, g_classLoaderDataCache);

ClassLoaderData::ClassLoaderData() :
    d(&shared_null)
{
    d->ref.ref();
}

ClassLoaderData::ClassLoaderData(const UrlString &libraryFile)
{
    init(libraryFile);
}

ClassLoaderData::ClassLoaderData(const ClassLoaderData &other):
    d(other.d)
{
    d->ref.ref(); 

    if (!d->sharable) 
        detach_helper();
}

void ClassLoaderData::init(const UrlString &libraryFile)
{
    DM_ASSERT(!libraryFile.isEmpty());

    ClassLoaderData::Data *tmp = g_classLoaderDataCache.data.value(libraryFile, NULL);
    if (tmp)
    {
        d = tmp;
        d->ref.ref();
        if (!d->sharable) 
            detach_helper();
        DM_LOGI("cached class loader");
    }
    else
    {
        d = new Data;
        d->ref = 1;
        d->sharable = true;
        d->loader = LibraryLoaderFactory::create(libraryFile);
        d->classinfo = NULL;
        
        g_classLoaderDataCache.data.insert(libraryFile, d);
    }
}

void ClassLoaderData::final()
{
    if (!d->ref.deref())
        clean(d);
}

dbool ClassLoaderData::load()
{
    if (d->classinfo)
        return true;

    if (d->loader->load())
    {
        void* ptr = NULL;
        dint ret = d->loader->resolveExport(DM_LIBRARY_CLAZZ_INFO, &ptr);
        if (ret == eLibResolveSuccess)
        {
            dm_classinfo_clazz_t* classFunc = (dm_classinfo_clazz_t*)ptr;
            if (classFunc != NULL)
            {
                d->classinfo = classFunc();
#ifdef DM_DEBUG
                if (name() != NULL)
                    DM_LOGI("Load class: %s", name());
#endif
                return d->classinfo != NULL;
            }
        }
    }
    return false;
}

void ClassLoaderData::unload()
{
    exchange(&shared_null);
}

dint ClassLoaderData::version() const
{
    return d->classinfo && d->classinfo->versionFunc ? (d->classinfo->versionFunc)() : 0;
}

const char* ClassLoaderData::name() const
{
    return d->classinfo && d->classinfo->nameFunc ? (d->classinfo->nameFunc)() : NULL;
}

void* ClassLoaderData::create()
{
    return d->classinfo && d->classinfo->createFunc ? (d->classinfo->createFunc)() : NULL;
}

dbool ClassLoaderData::destroy(void *p)
{
    if (p && d->classinfo && d->classinfo->destroyFunc)
    {
        (d->classinfo->destroyFunc)(p);
        return true;
    }

    return false;
}

ClassLoaderData::Data* ClassLoaderData::detach()
{
    Data *x = d;
    Data* t = NULL;
    DM_TRY
    {
        t = new Data;

        t->ref = 1;
        t->sharable = true;
        t->loader = NULL;
        t->classinfo = NULL;

        d = t;
    }
    DM_CATCH (const std::bad_alloc& ba)
    {
        DM_FATAL("%s", ba.what());
        delete t;
    }
    return x;
}

void ClassLoaderData::detach_helper()
{
    ClassLoaderData::Data *x = detach();
    if (!x->ref.deref())
        clean(x);
}

void ClassLoaderData::exchange(Data *data)
{
    if (d != data) {
        ClassLoaderData::Data *o = data;
        o->ref.ref();
        if (!d->ref.deref())
            clean(d);
        d = o;
        if (!d->sharable)
            detach_helper();
    }
}

void ClassLoaderData::clean(Data *data)
{
    g_classLoaderDataCache.data.remove(data->loader->getFullPath());
    data->loader->unload();
    data->classinfo = NULL;
    DM_SAFE_DELETE(data->loader);
    DM_SAFE_DELETE(data);
}

ClassLoaderData& ClassLoaderData::operator=(const ClassLoaderData &other)
{
    if (this == &other)
        return *this;

    if (d != other.d) {
        ClassLoaderData::Data *o = other.d;
        o->ref.ref();
        if (!d->ref.deref())
            clean(d);
        d = o;
        if (!d->sharable)
            detach_helper();
    }
    return (*this);
}

void ClassLoaderData::cleanCache()
{
    g_classLoaderDataCache.data.clear();
}
DM_END_NAMESPACE