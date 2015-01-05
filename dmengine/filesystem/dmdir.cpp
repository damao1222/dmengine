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

#include "dmdir.h"
#include "platform/dmfilesystem_arch.h"
#include "dmfile.h"

DM_BEGIN_NAMESPACE
class DirPrivate
{
public:
	UrlString path;
};

Dir::Dir(const UrlString &path) :
	C_D(Dir)
{
	pdm->path = path;
}

Dir::~Dir()
{
	D_D(Dir);
}

const UrlString& Dir::path() const
{
	return pdm->path;
}

FileInfoList Dir::fileInfoList(dint fileFilter /*= FileInfo::eUnknown*/) const
{
    FileInfoList list;
    FileEngine::getFileInfos(pdm->path, list, fileFilter);
    return list;
}

dbool Dir::removeAll()
{
    if (!exists())
        return false;

    FileInfoList list = fileInfoList();
    for (dint i=0; i<list.size(); ++i)
    {
        const FileInfo &info = list.at(i);
        if (info.isDir())
        {
            Dir dir(info.getPath());
            if (!dir.removeAll())
                return false;
        }
        else
        {
            File file(info.getPath());
            if (!file.remove())
                return false;
        }
    }
    return remove();
}

dbool Dir::exists() const
{
	return FileEngine::exists(pdm->path, true);
}

dbool Dir::remove()
{
	return rmdir(pdm->path);
}

dbool Dir::rename(const UrlString &newUrl)
{
	if (pdm->path.isEmpty() || newUrl.isEmpty())
		return false;

	return File::rename(pdm->path, newUrl);
}

dbool Dir::create()
{
	return mkdir(pdm->path);
}

Char Dir::separator()
{
#if defined (DM_FS_FAT) || defined(DM_OS_WIN) || defined(DM_OS_SYMBIAN)
	return Char('\\');
#elif defined(DM_OS_UNIX)
	return Char('/');
#elif defined (DM_OS_MAC)
	return Char(':');
#else
	return Char('/');
#endif
}

UrlString& Dir::addSlashAtEnd(UrlString &url)
{
    if (!url.endWith('\\') && !url.endWith('/'))
    {
        url += separator().toStrChar();
    }

    return url;
}

dbool Dir::exists(const UrlString &path)
{
	Dir dir(path);
	return dir.exists();
}

dbool Dir::rmdir(const UrlString &path)
{
	return FileEngine::rmdir(path, false);
}

dbool Dir::mkdir(const UrlString &path)
{
	return FileEngine::mkdir(path, false);
}

dbool Dir::mkpath(const UrlString &path)
{
	return FileEngine::mkdir(path, true);
}

dbool Dir::rmpath(const UrlString &path)
{
	return FileEngine::rmdir(path, true);
}

dbool Dir::removeAll(const UrlString &path)
{
    Dir dir(path);
    return dir.removeAll();
}

DM_END_NAMESPACE