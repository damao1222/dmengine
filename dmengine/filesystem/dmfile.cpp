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

#include "dmfile.h"
#include "platform/dmfilesystem_arch.h"

DM_BEGIN_NAMESPACE

class FilePrivate
{
public:
    IFile *engine;
};

#define FILE_WRAP(Func) (pdm->engine->Func)

File::File() :
    C_D(File)
{
    pdm->engine = new FileEngine;
}

File::File(const UrlString& url) :
    C_D(File)
{
    pdm->engine = new FileEngine(url);
}

File::~File()
{
    DM_SAFE_DELETE(pdm->engine);
    D_D(File);
}

dbool File::open(EOpenMode mode /*= eO_ReadOnly*/)
{
    return FILE_WRAP(open)(mode);
}

dbool File::open(const UrlString& url, EOpenMode mode /*= eO_ReadOnly*/)
{
    return FILE_WRAP(open)(url, mode);
}

dbool File::exists() const
{
    return FILE_WRAP(exists)();
}

dint File::stat(const UrlString& url, struct __stat64* buffer)
{
    return FILE_WRAP(stat)(url, buffer);
}

dint File::stat(struct __stat64* buffer)
{
    return FILE_WRAP(stat)(buffer);
}

duint File::read(void* lpBuf, dint64 uiBufSize)
{
    return FILE_WRAP(read)(lpBuf, uiBufSize);
}

dint File::write(const void* lpBuf, dint64 uiBufSize)
{
    return FILE_WRAP(write)(lpBuf, uiBufSize);
}

dbool File::readString(char *szLine, dint iLineLength)
{
    return FILE_WRAP(readString)(szLine, iLineLength);
}

dint64 File::seek(dint64 iFilePosition, dint iWhence /*= SEEK_SET*/)
{
    return FILE_WRAP(seek)(iFilePosition, iWhence);
}

void File::close()
{
    FILE_WRAP(close)();
}

dint64 File::getPosition() const
{
    return FILE_WRAP(getPosition)();
}

dint64 File::getLength() const
{
    return FILE_WRAP(getLength)();
}

dint File::getChunkSize() const
{
    return FILE_WRAP(getChunkSize)();
}

void File::flush()
{
    FILE_WRAP(flush)();
}

dbool File::remove()
{
    return FILE_WRAP(remove)();
}

dbool File::rename(const UrlString& urlnew)
{
    return FILE_WRAP(rename)(urlnew);
}

dbool File::setHidden(dbool hidden)
{
    return FILE_WRAP(setHidden)(hidden);
}

dbool File::copyTo(const UrlString& destUrl)
{
    return FILE_WRAP(copyTo)(destUrl);
}

dint File::ioctl(EIoControl request, void* param)
{
    return FILE_WRAP(ioctl)(request, param);
}

UtilString File::getContent() const
{
    return "";
}

const UrlString& File::path() const
{
    return FILE_WRAP(path)();
}

dbool File::getAllDatas(dbyte *&pBuf, dint64 &uSize)
{
    pBuf = NULL; uSize = 0;
    if(seek(0, SEEK_CUR) < 0) 
    {
        return false;
    }

    dint64 iFileLen = getLength();
    pBuf = new duchar[iFileLen];
    uSize = 0;
    while (uSize != iFileLen)
    {
        dint64 readed = read(pBuf, iFileLen);

        if (readed <= 0)
            return false;

        uSize += readed;
    }
    
    return uSize == iFileLen;
}

dbool File::exists(const UrlString& url)
{
    return File(url).exists();
}

dbool File::remove(const UrlString& url)
{
    return File(url).remove();
}

dbool File::rename(const UrlString& url, const UrlString& urlnew)
{
    return File(url).rename(urlnew);
}

dbool File::setHidden(const UrlString& url, dbool hidden)
{
    return File(url).setHidden(hidden);
}

dbool File::copy(const UrlString& srcUrl, const UrlString& destUrl)
{
    if (srcUrl.isEmpty() || destUrl.isEmpty())
        return false;

    File file(srcUrl);
    if (file.copyTo(destUrl))
        return true;

    File destFile(destUrl);
    if (!destFile.open(eO_OverWrite))
    {
        return false;
    }

    dbool bRet = false;
    if (file.open())
    {
        dbyte *pBuf = NULL;
        dint64 uSize = 0;
        if (file.getAllDatas(pBuf, uSize))
        {
            bRet = destFile.write(pBuf, uSize) > 0;
            DM_SAFE_DELETE_ARRAY(pBuf);
        }
        file.close();
    }

    destFile.close();
    return bRet;
}

DM_END_NAMESPACE

