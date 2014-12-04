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
#include "dmdllloader_win.h"
#include "dmlogger.h"

DM_BEGIN_NAMESPACE

class DllLoaderWinPrivate
{
public:
    HMODULE dllHandle;
};

DllLoaderWin::DllLoaderWin(const UrlString &libraryFile) :
    LibraryLoader(libraryFile),
    C_D(DllLoaderWin)
{
    pdm->dllHandle = NULL;
}

DllLoaderWin::~DllLoaderWin()
{
    if (pdm->dllHandle)
        unload();
    D_D(DllLoaderWin);
}

dbool DllLoaderWin::load()
{
    if (pdm->dllHandle != NULL)
        return true;

    UrlString strFileName = getFullPath();
    pdm->dllHandle = LoadLibraryExW(strFileName.toCharStr(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
    if (!pdm->dllHandle)
    {
        LPVOID lpMsgBuf;
        DWORD dw = GetLastError(); 

        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dw, 0, (LPTSTR) &lpMsgBuf, 0, NULL );
        DM_LOGW("%s: Failed to load %s with error %d:%s", strFileName.toCharStr(), dw, lpMsgBuf);
        LocalFree(lpMsgBuf);
        return false;
    }

    return true;
}

void DllLoaderWin::unload()
{
    if (pdm->dllHandle)
    {
        if (!FreeLibrary(pdm->dllHandle))
            DM_LOGW("Unable to unload %s", getFullPath().toCharStr());
    }

    pdm->dllHandle = NULL;
}

dint DllLoaderWin::resolveExport(const char* symbol, void** f, dbool logging /*= true*/)
{
    if (!pdm->dllHandle && !load())
    {
        if (logging)
            DM_LOGW("Unable to resolve: %s %s, reason: DLL not loaded", getFullPath().toCharStr(), symbol);
        return eLibResolveFailed;
    }

    void *s = GetProcAddress(pdm->dllHandle, symbol);

    if (!s)
    {
        if (logging)
            DM_LOGW("Unable to resolve: %s %s", getFullPath().toCharStr(), symbol);
        return eLibResolveFailed;
    }

    *f = s;
    return eLibResolveSuccess;
}

dbool DllLoaderWin::hasSymbols()
{
    return false;
}

DM_END_NAMESPACE