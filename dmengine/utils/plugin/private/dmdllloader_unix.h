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
#ifndef DMDLLLOADER_UNIX_H
#define DMDLLLOADER_UNIX_H

#include "../dmlibraryloader.h"

DM_BEGIN_NAMESPACE
DM_PRIVATE_CLASS(DllLoaderUnix);
class DM_DLL_EXPORT DllLoaderUnix : public LibraryLoader
{
    DM_DECLARE_PRIVATE(DllLoaderUnix)
public:
    DllLoaderUnix(const UrlString &libraryFile);
    virtual ~DllLoaderUnix();

    virtual dbool load();
    virtual void unload();

    virtual dint resolveExport(const char* symbol, void** ptr, dbool logging = true);
    virtual dbool hasSymbols();
};

DM_END_NAMESPACE

#endif // DMDLLLOADER_UNIX_H
