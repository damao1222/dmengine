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

#include "dmfileinfo.h"

DM_BEGIN_NAMESPACE
class FileInfoPrivate
{
public:
	UrlString path;
    FileInfo::EFileType type;
};

FileInfo::FileInfo(const UrlString &path, EFileType type) :
	C_D(FileInfo)
{
	pdm->path = path;
    pdm->type = type;
}

FileInfo::FileInfo(const FileInfo &other) :
    C_D(FileInfo)
{
    pdm->path = other.pdm->path;
    pdm->type = other.pdm->type;
}

FileInfo::~FileInfo()
{
	D_D(FileInfo);
}

dbool FileInfo::isDir() const
{
    return pdm->type & eDir;
}

const UrlString& FileInfo::getPath() const
{
    return pdm->path;
}

FileInfo& FileInfo::operator=(const FileInfo &other)
{
    if (this == &other)
        return *this;

    pdm->path = other.pdm->path;
    pdm->type = other.pdm->type;
    return *this;
}

dbool FileInfo::operator==(const FileInfo &other) const
{
    return pdm->path == other.pdm->path && pdm->type == other.pdm->type;
}

dbool FileInfo::operator!=(const FileInfo &other) const
{
    return !((*this) == other);
}

DM_END_NAMESPACE