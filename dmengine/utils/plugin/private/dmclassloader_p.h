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
#ifndef DMCLASSLOADER_P_H
#define DMCLASSLOADER_P_H

#include "dmatomicint.h"
#include "../dmlibraryloader.h"
#include "../dmlibrarydef.h"

DM_BEGIN_NAMESPACE
struct DM_DLL_EXPORT ClassLoaderData
{
    struct Data {
        BasicAtomicInt ref;
        duint32 sharable : 1;
        LibraryLoader*      loader;
        DmLibraryClass* classinfo;
        UrlString path;
    };

    ClassLoaderData();
    ClassLoaderData(const UrlString &libraryFile);
    ClassLoaderData(const ClassLoaderData &other);

    void init(const UrlString &libraryFile);
    void final();

    dbool load();
    void unload();
    dint version() const;
    const char* name() const;

    void* create();
    dbool destroy(void *p);

    Data* detach();
    void detach_helper();
    void exchange(Data *data);
    void clean(Data *data);

    ClassLoaderData& operator=(const ClassLoaderData &d);

    Data *d;
    static Data shared_null;
    static void cleanCache();
};
DM_END_NAMESPACE

#endif // DMCLASSLOADER_P_H
