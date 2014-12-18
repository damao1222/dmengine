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
#ifndef DMFILEUNIX_H
#define DMFILEUNIX_H
#include "dmifile.h"
#include "dmfileinfo.h"
#include <fcntl.h>

DM_BEGIN_NAMESPACE
#define EINTR_LOOP(var, cmd)                    \
    do {                                        \
        var = cmd;                              \
    } while (var == -1 && errno == EINTR)

static inline dint dm_safe_open(const char *path, int flags, mode_t mode = 0777)
{
#ifdef O_CLOEXEC
    flags |= O_CLOEXEC;
#endif
    register int fd;
    EINTR_LOOP(fd, ::open(path, flags, mode));

    // unknown flags are ignored, so we have no way of verifying if
    // O_CLOEXEC was accepted
    if (fd != -1)
        ::fcntl(fd, F_SETFD, FD_CLOEXEC);
    return fd;
}

#define DM_OPEN dm_safe_open

struct SNativeIoControl
{
    unsigned long int   request;
    void*               param;
};

DM_PRIVATE_CLASS(FileUnix);
class DM_DLL_EXPORT FileUnix : public IFile
{
    DM_DECLARE_PRIVATE(FileUnix)
public:
    static dbool exists(const UrlString& url, dbool dir = false);
    static dbool remove(const UrlString& url);
    static dbool rename(const UrlString& url, const UrlString& urlnew);
    static dbool setHidden(const UrlString& url, dbool hidden);
    static dbool mkdir(const UrlString& url, dbool parent);
    static dbool rmdir(const UrlString& url, dbool parent);
    static dbool getFileInfos(const UrlString& url, FileInfoList &list, dint fileFilter);

    FileUnix();
    explicit FileUnix(const UrlString& url);
    virtual ~FileUnix();

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
#endif // DMFILEUNIX_H
