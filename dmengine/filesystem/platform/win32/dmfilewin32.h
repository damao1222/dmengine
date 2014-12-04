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
#ifndef DMFILEWIN32_H
#define DMFILEWIN32_H
#include "dmifile.h"
#include "dmfileinfo.h"

DM_BEGIN_NAMESPACE
DM_PRIVATE_CLASS(FileWin32);
class DM_DLL_EXPORT FileWin32 : public IFile
{
    DM_DECLARE_PRIVATE(FileWin32)
public:
    static dbool exists(const UrlString& url, dbool dir = false);
    static dbool remove(const UrlString& url);
    static dbool rename(const UrlString& url, const UrlString& urlnew);
    static dbool setHidden(const UrlString& url, dbool hidden);
    static dbool mkdir(const UrlString& url, dbool parent);
    static dbool rmdir(const UrlString& url, dbool parent);
    static dbool getFileInfos(const UrlString& url, FileInfoList &list, dint fileFilter);

    FileWin32();
    explicit FileWin32(const UrlString& url);
    virtual ~FileWin32();

    virtual dbool open(EOpenMode mode);
    virtual dbool open(const UrlString& url, EOpenMode mode);
    virtual dbool exists() const;
    virtual dint stat(const UrlString& url, struct __stat64* buffer);
    virtual dint stat(struct __stat64* buffer);
    virtual duint read(void* lpBuf, dint64 uiBufSize);
    virtual dint write(const void* lpBuf, dint64 uiBufSize);
    virtual dint64 seek(dint64 iFilePosition, dint iWhence = SEEK_SET);
    virtual void close();
    virtual dint64 getPosition() const;
    virtual dint64 getLength() const;
    virtual void flush();
    virtual dint truncate(dint64 size) const { return -1;};

    virtual dbool skipNext(){return false;}

    virtual dbool remove();
    virtual dbool rename(const UrlString& urlnew);
    virtual dbool setHidden(dbool hidden);

    virtual dint ioctl(EIoControl request, void* param);

    virtual UrlString getContent() const { return ""; }
    virtual const UrlString& path() const;
};
DM_END_NAMESPACE
#endif // DMFILEWIN32_H
