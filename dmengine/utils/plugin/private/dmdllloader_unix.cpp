/*
   Copyright (C) 2012-2014 Xiongfa Li
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
#include "dmdllloader_unix.h"
#include "dmlogger.h"
#include <dlfcn.h>

DM_BEGIN_NAMESPACE

class DllLoaderUnixPrivate
{
public:
    void* dllHandle;
};

DllLoaderUnix::DllLoaderUnix(const UrlString &libraryFile) :
    LibraryLoader(libraryFile),
    C_D(DllLoaderUnix)
{
    pdm->dllHandle = NULL;
}

DllLoaderUnix::~DllLoaderUnix()
{
    if (pdm->dllHandle)
        unload();
    D_D(DllLoaderUnix);
}

dbool DllLoaderUnix::load()
{
    if (pdm->dllHandle != NULL)
        return true;

    UtilString strFileName= getFullPath();
    int flags = RTLD_LAZY;

    DM_LOGI("Loading: %s\n", strFileName.toCharStr());
#if defined(TARGET_ANDROID)
    CAndroidDyload temp;
    pdm->dllHandle = temp.Open(strFileName.c_str());
#else
    pdm->dllHandle = dlopen(strFileName.toCharStr(), flags);
#endif
    if (!pdm->dllHandle)
    {
        DM_LOGW("Unable to load %s, reason: %s", strFileName.toCharStr(), dlerror());
        return false;
    }
    return true;
}

void DllLoaderUnix::unload()
{
    DM_LOGI("Unloading: %s\n", getFullPath().toCharStr());

    if (pdm->dllHandle)
    {
#if defined(TARGET_ANDROID)
        CAndroidDyload temp;
        if (temp.Close(pdm->dllHandle) != 0)
#else
        if (dlclose(pdm->dllHandle) != 0)
#endif
            DM_LOGW("Unable to unload %s, reason: %s", getFullPath().toCharStr(), dlerror());
    }

    pdm->dllHandle = NULL;
}

dint DllLoaderUnix::resolveExport(const char* symbol, void** f, dbool logging /*= true*/)
{
    if (!pdm->dllHandle && !load())
    {
        if (logging)
            DM_LOGW("Unable to resolve: %s %s, reason: so not loaded", getFullPath().toCharStr(), symbol);
        return eLibResolveFailed;
    }

    void* s = dlsym(pdm->dllHandle, symbol);
    if (!s)
    {
        if (logging)
            DM_LOGW("Unable to resolve: %s %s, reason: %s", getFullPath().toCharStr(), symbol, dlerror());
        return eLibResolveFailed;
    }

    *f = s;
    return eLibResolveSuccess;
}

dbool DllLoaderUnix::hasSymbols()
{
    return false;
}

DM_END_NAMESPACE
