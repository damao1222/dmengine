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
#ifndef DMIFILE_H
#define DMIFILE_H
#include "dmutilstring.h"
#include <sys/stat.h>
#include <errno.h>

#ifndef DM_OS_WIN
#if defined(DM_OS_DARWIN) || defined(DM_OS_FREEBSD)
#define stat64 stat
#define __stat64 stat
#define fstat64 fstat
    typedef int64_t off64_t;
#if defined(DM_OS_DARWIN_IOS) || defined(DM_OS_FREEBSD)
#define statfs64 statfs
#endif
#else
#define __stat64 stat64
#endif
#endif

DM_BEGIN_NAMESPACE
typedef enum {
  IOCTRL_NATIVE         = 1, /**< SNativeIoControl structure, containing what should be passed to native ioctrl */
  IOCTRL_SEEK_POSSIBLE  = 2, /**< return 0 if known not to work, 1 if it should work */
  IOCTRL_CACHE_STATUS   = 3, /**< SCacheStatus structure */
  IOCTRL_CACHE_SETRATE  = 4, /**< unsigned dint with speed limit for caching in bytes per second */
  IOCTRL_SET_CACHE      = 8, /** <FileCache */
} EIoControl;

typedef enum {
  eO_ReadOnly           = 1,    /** <read only */
  eO_WriteOnly          = 2,    /** <write only */
  eO_ReadWrite          = 3,    /** <read and write */
  eO_OverWrite          = 8,    /** <always create file */
  eO_ForceWrite         = eO_WriteOnly | eO_OverWrite   /** <over write the file */
} EOpenMode;

class DM_DLL_EXPORT IFile
{
public:
    IFile();
    explicit IFile(const UrlString& url);
    virtual ~IFile();

    virtual dbool open(EOpenMode mode) = 0;
    virtual dbool open(const UrlString& url, EOpenMode mode) = 0;
    virtual dbool exists() const = 0;
    virtual dint stat(const UrlString& url, struct __stat64* buffer) = 0;
    virtual dint stat(struct __stat64* buffer);
    virtual duint read(void* lpBuf, dint64 uiBufSize) = 0;
    virtual dint write(const void* lpBuf, dint64 uiBufSize) { return -1;};
    virtual dbool readString(char *szLine, dint iLineLength);
    virtual dint64 seek(dint64 iFilePosition, dint iWhence = SEEK_SET) = 0;
    virtual void close() = 0;
    virtual dint64 getPosition() const = 0;
    virtual dint64 getLength() const = 0;
    virtual void flush() { }
    virtual dint truncate(dint64 size) const { return -1;};

    /* Returns the minium size that can be read from input stream.   *
    * For example cdrom access where access could be sector based.  *
    * This will cause file system to buffer read requests, to       *
    * to meet the requirement of CFile.                             *
    * It can also be used to indicate a file system is non buffered *
    * but accepts any read size, have it return the value 1         */
    virtual dint  getChunkSize() const {return 0;}

    virtual dbool skipNext(){return false;}

    virtual dbool remove() { return false; }
    virtual dbool rename(const UrlString& urlnew) { return false; }
    virtual dbool setHidden(dbool hidden) { return false; }
    virtual dbool copyTo(const UrlString& destUrl) { return false; }

    virtual dint ioctl(EIoControl request, void* param) { return -1; }

    virtual UrlString getContent() const { return ""; }

    virtual const UrlString& path() const { return ""; }
};
DM_END_NAMESPACE

#endif // DMIFILE_H
