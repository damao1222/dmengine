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
#include "dmfileunix.h"
#include "dmlogger.h"
#include "../dmfilehelper.h"
#include "dmdir.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

DM_BEGIN_NAMESPACE
#define INVALID_HANDLE_VALUE -1

class FileUnixPrivate
{
public:
    dbool isVaild() const;
    UrlString url;
    dint fileHandle;
    dint64 pos;
    dint64 size;
};

dbool FileUnixPrivate::isVaild() const
{
    return fileHandle != INVALID_HANDLE_VALUE;
}

static dint CreateDir(const UrlString& url, mode_t mode)
{
    return ::mkdir(url.toCharStr(), mode);
}

static dint RemoveDir(const UrlString& url)
{
    return ::rmdir(url.toCharStr());
}

static dint GetStat(const UrlString& url, struct __stat64 *buf)
{
    return ::stat(url.toCharStr(), buf);
}

dbool FileUnix::mkdir(const UrlString& url, dbool parent)
{
    UrlString path = GetLocal(url);
    if (!path.isEmpty())
    {
        if (parent)
        {
            int oldslash = -1;
            for (int slash=0; slash != -1; oldslash = slash) {
                slash = path.indexOf(Dir::separator().toStrChar(), oldslash+1);
                if (slash == -1) {
                    if (oldslash == path.length())
                        break;
                    slash = path.length();
                }
                if (slash) {
                    UrlString chunk = path.left(slash);
                    struct __stat64 st;
                    if (GetStat(chunk, &st) != -1) {
                        if ((st.st_mode & S_IFMT) != S_IFDIR)
                            return false;
                    } else if (CreateDir(chunk, 0777) != 0) {
                        return false;
                    }
                }
            }
            return true;
        }

        return (CreateDir(path, 0777) == 0);
    }
    return false;
}

dbool FileUnix::rmdir(const UrlString& url, dbool parent)
{
    UrlString path = GetLocal(url);
    if (!path.isEmpty())
    {
        if (parent)
        {
            for (int oldslash = 0, slash=path.length(); slash > 0; oldslash = slash) {
                UrlString chunk = path.left(slash);
                struct __stat64 st;
                if (GetStat(chunk, &st) != -1) {
                    if ((st.st_mode & S_IFMT) != S_IFDIR)
                        return false;
                    if (RemoveDir(chunk) != 0)
                        return oldslash != 0;
                } else {
                    return false;
                }
                slash = path.lastIndexOf(Dir::separator().toStrChar(), oldslash-1);
            }
            return true;
        }

        return RemoveDir(path) == 0;
    }
    return false;
}

dbool FileUnix::getFileInfos(const UrlString& url, FileInfoList &list, dint fileFilter)
{
    UrlString path = GetLocal(url);
    if (!path.isEmpty())
    {
        DIR *dir;  
        struct dirent *dmsg;  
        int i=0;  
        UrlString subPath;
        if ((dir = ::opendir(path.toCharStr())) != NULL)  
        {
            Dir::addSlashAtEnd(path);
            while ((dmsg = ::readdir(dir)) != NULL)  
            {  
                if (!strcmp(dmsg->d_name, ".") || !strcmp(dmsg->d_name, ".."))  
                    continue;  

                if (dmsg->d_type == DT_DIR )  
                {  
                    if (fileFilter & FileInfo::eDir)
                        continue;

                    list.append(FileInfo(path + dmsg->d_name, FileInfo::eDir));
                }  
                else
                {
                    if (fileFilter & FileInfo::eFile)
                        continue;

                    list.append(FileInfo(path + dmsg->d_name, FileInfo::eFile));
                }
            }  
            ::closedir(dir);     
        }
    }
    return false;
}

FileUnix::FileUnix():
    C_D(FileUnix)
{
    pdm->fileHandle = INVALID_HANDLE_VALUE;
    pdm->pos = 0;
    pdm->size = 0;
}

FileUnix::FileUnix(const UrlString& url):
    C_D(FileUnix)
{
    pdm->url = GetLocal(url);
    pdm->fileHandle = INVALID_HANDLE_VALUE;
    pdm->pos = 0;
    pdm->size = 0;
}

FileUnix::~FileUnix()
{
    close();

    D_D(FileUnix);
}

dbool FileUnix::open(EOpenMode eMode)
{
    if (pdm->url.isEmpty())
        return false;

    int flags = 0, mode = S_IRUSR | S_IRGRP | S_IROTH;

    switch (eMode)
    {
    case eO_ReadOnly:
        flags = O_RDONLY;
        break;
    case eO_WriteOnly:
        flags = O_WRONLY;
        mode  |= S_IWUSR;
        break;
    case eO_ReadWrite:
        flags = O_RDWR;
        mode  |= S_IWUSR;
        break;
    case eO_OverWrite:
        flags = O_RDWR | O_TRUNC;
        mode  |= S_IWUSR;
        break;
    case eO_ForceWrite:
        flags = O_RDWR | O_CREAT | O_TRUNC;
        mode  |= S_IWUSR;
        break;
    default:
        break;
    }

    int fd = DM_OPEN(pdm->url.toCharStr(), flags, mode);
    if (fd == -1)
    {
        if (errno == 20)
            DM_LOGW("error %d opening file <%s>, flags:%x, mode:%x. ", errno, pdm->url.toCharStr(), flags, mode);
        return INVALID_HANDLE_VALUE;
    }

    pdm->fileHandle = fd;
    return true;
}

dbool FileUnix::open(const UrlString& url, EOpenMode mode)
{
    pdm->url = GetLocal(url);
    return open(mode);
}

dbool FileUnix::exists(const UrlString& url, dbool dir /*= false*/)
{
    UrlString path = GetLocal(url);
    if (path.isEmpty())
        return false;

    if (dir)
    {
        DIR *tmpDir = opendir(path.toCharStr());
        if (tmpDir)
        {
            closedir(tmpDir);
            return true;
        }
        return false;
    }
    
    struct __stat64 buffer;
    return (GetStat(url.toCharStr(), &buffer)==0);
}

dbool FileUnix::exists() const
{
    return exists(pdm->url, false);
}

dint FileUnix::stat(const UrlString& url, struct __stat64* buffer)
{
    UrlString path = GetLocal(url);
    return GetStat(path.toCharStr(), buffer);
}

dint FileUnix::stat(struct __stat64* buffer)
{
    return ::fstat64(pdm->fileHandle, buffer);
}

duint FileUnix::read(void* lpBuf, dint64 uiBufSize)
{
    if (!pdm->isVaild()) 
        return 0;

    size_t bytesRead = ::read(pdm->fileHandle, lpBuf, uiBufSize);
    if (bytesRead == (size_t) -1)
        return 0;

    pdm->pos += bytesRead;

    return bytesRead;
}

dint FileUnix::write(const void* lpBuf, dint64 uiBufSize) 
{ 
    if (!pdm->isVaild())
        return 0;

    size_t bytesWritten = ::write(pdm->fileHandle, lpBuf, uiBufSize);

    if (bytesWritten == (size_t) -1)
        return 0;

    return bytesWritten;
}

dint64 FileUnix::seek(dint64 iFilePosition, int iWhence /*= SEEK_SET*/)
{
    if (!pdm->isVaild())
        return -1;

#if defined(DM_OS_DARWIN) || defined(__FreeBSD__)
    off64_t currOff = ::lseek(pdm->fileHandle, iFilePosition, iWhence);
#else
    off64_t currOff = ::lseek64(pdm->fileHandle, iFilePosition, iWhence);
#endif

    if (currOff != -1)
    {
        pdm->pos = static_cast<dint64>(currOff);
    }
    return pdm->pos;
}

void FileUnix::close()
{
    if (pdm->isVaild()) {
        ::close(pdm->fileHandle);
        pdm->fileHandle = INVALID_HANDLE_VALUE;
        pdm->pos = 0;
        pdm->size = 0;
    }
}

dint64 FileUnix::getPosition() const
{
    return pdm->pos;
}

dint64 FileUnix::getLength() const
{
    if(pdm->isVaild() && (pdm->size <= pdm->pos || pdm->size == 0))
    {
        struct stat64 fileStat;
        if (::fstat64(pdm->fileHandle, &fileStat) == 0)
            pdm->size = fileStat.st_size;
        else
            DM_LOGW("FileUnix::getLength - GetFileSizeEx failed with error");
    }
    return pdm->size;
}

void FileUnix::flush()
{
    if (pdm->isVaild())
        ::fsync(pdm->fileHandle) ;
}

int FileUnix::ioctl(EIoControl request, void* param)
{
    if(request == IOCTRL_NATIVE && param)
    {
        SNativeIoControl* s = (SNativeIoControl*)param;
        return ::ioctl(pdm->fileHandle, s->request, s->param);
    }
}

const UrlString& FileUnix::path() const
{
    return pdm->url;
}

dbool FileUnix::remove(const UrlString& url)
{
    UrlString path = GetLocal(url);
    if (::unlink(path.toCharStr()) == 0)
        return 1;

    if (errno == EACCES)
    {
        DM_LOGW("cant delete file, trying to change mode <%s>", path.toCharStr());
        if (chmod(path.toCharStr(), 0600) != 0)
        {
            DM_LOGW("failed to change mode <%s>", path.toCharStr());
            return 0;
        }

        DM_LOGI("%s - reattempt to delete file",__FUNCTION__);

        if (::unlink(path.toCharStr()) == 0)
            return 1;
    }
    else if (errno == ENOENT)
    {
        UrlString strLower = url.toLower();
        DM_LOGW("cant delete file <%s>. trying lower case <%s>", path.toCharStr(), strLower.toCharStr());
        if (::unlink(strLower.toCharStr()) == 0)
        {
            DM_LOGI("successfuly removed file <%s>", strLower.toCharStr());
            return 1;
        }
    }

    return 0;
}

bool FileUnix::rename(const UrlString& url, const UrlString& newUrl)
{
    if (::rename(url.toCharStr(), newUrl.toCharStr()) == 0)
        return true;

    if (errno == EACCES)
    {
        DM_LOGW("cant move file, trying to change mode <%s>", url.toCharStr());
        if (::chmod(url.toCharStr(), 0600) != 0)
        {
            DM_LOGW("failed to change mode <%s>", url.toCharStr());
            return false;
        }

        DM_LOGW("reattempt to move file");

        if (rename(url.toCharStr(), newUrl.toCharStr()) == 0)
            return true;
    }
    else if (errno == ENOENT)
    {
        UrlString strLower = url.toLower();
        DM_LOGW("cant move file <%s>. trying lower case <%s>", url.toCharStr(), strLower.toCharStr());
        if (rename(strLower.toCharStr(), newUrl.toCharStr()) == 0) {
            DM_LOGI("successfuly moved file <%s>", strLower.toCharStr());
            return true;
        }
    }

    return false;
}

dbool FileUnix::rename(const UrlString& urlnew)
{
    return rename(pdm->url, urlnew);
}

dbool FileUnix::setHidden(const UrlString &url, dbool hidden)
{
    return false;
}

dbool FileUnix::setHidden(dbool hidden)
{
    return false;
}

dbool FileUnix::remove()
{
    return remove(pdm->url);
}
DM_END_NAMESPACE
