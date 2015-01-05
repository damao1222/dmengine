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
#ifndef DMDLLLOADER_WIN_H
#define DMDLLLOADER_WIN_H

#include "../dmlibraryloader.h"

DM_BEGIN_NAMESPACE
DM_PRIVATE_CLASS(DllLoaderWin);
class DM_DLL_EXPORT DllLoaderWin : public LibraryLoader
{
    DM_DECLARE_PRIVATE(DllLoaderWin)
public:
    DllLoaderWin(const UrlString &libraryFile);
    virtual ~DllLoaderWin();

    virtual dbool load();
    virtual void unload();

    virtual dint resolveExport(const char* symbol, void** ptr, dbool logging = true);
    virtual dbool hasSymbols();
};

DM_END_NAMESPACE

#endif // DMDLLLOADER_WIN_H
