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
#include "dmlibraryloader.h"

DM_BEGIN_NAMESPACE

class LibraryLoaderPrivate
{
public:
    UrlString fileName;
    dint namePos;
};

LibraryLoader::LibraryLoader(const UrlString &libraryFile) :
    C_D(LibraryLoader)
{
    pdm->fileName = libraryFile;
    pdm->namePos = pdm->fileName.lastIndexOf('/');

    DM_ASSERT(!pdm->fileName.isEmpty());
}

LibraryLoader::~LibraryLoader()
{
    D_D(LibraryLoader);
}

UrlString LibraryLoader::getName() const
{
    return pdm->fileName.subStr(pdm->namePos + 1);
}

const UrlString & LibraryLoader::getFullPath() const
{
    return pdm->fileName;
}

UrlString LibraryLoader::getDir() const
{
    return pdm->fileName.subStr(0, pdm->namePos);
}
DM_END_NAMESPACE