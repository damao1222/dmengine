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
#include "dmfilewin32.h"
#include "dmlogger.h"
#include "../dmfilehelper.h"
#include "dmdir.h"
#include <io.h>

DM_BEGIN_NAMESPACE

class FileWin32Private
{
public:
    dbool isVaild() const;
    UrlString url;
    HANDLE fileHandle;
    dint64 pos;
    dint64 size;
};

static inline bool getFindData(UrlString path, WIN32_FIND_DATA &findData)
{
	// path should not end with a trailing slash
	while (path.endWith('\\'))
		path.remove(path.size()-1);

	// can't handle drives
	if (!path.endWith(StdChar(':'))) {
		HANDLE hFind = ::FindFirstFile((wchar_t*)path.toCharStr(), &findData);
		if (hFind != INVALID_HANDLE_VALUE) {
			::FindClose(hFind);
			return true;
		}
	}

	return false;
}

static bool isDirPath(const UrlString &dirPath, bool *existed)
{
	UrlString path = GetLocal(dirPath);
	if (path.length() == 2 && path.at(1) == StdChar(':'))
		path += StdChar('\\');

	DWORD fileAttrib = ::GetFileAttributes(path.toCharStr());
	if (fileAttrib == INVALID_FILE_ATTRIBUTES) {
		int errorCode = GetLastError();
		if (errorCode == ERROR_ACCESS_DENIED || errorCode == ERROR_SHARING_VIOLATION) {
			WIN32_FIND_DATA findData;
			if (getFindData(path, findData))
				fileAttrib = findData.dwFileAttributes;
		}
	}

	if (existed)
		*existed = fileAttrib != INVALID_FILE_ATTRIBUTES;

	if (fileAttrib == INVALID_FILE_ATTRIBUTES)
		return false;

	return fileAttrib & FILE_ATTRIBUTE_DIRECTORY;
}

dbool FileWin32Private::isVaild() const
{
    return fileHandle != INVALID_HANDLE_VALUE;
}

dbool FileWin32::exists(const UrlString& url, dbool dir /*= false*/)
{
    UrlString path = GetLocal(url);

	if (dir)
	{
		dbool bExists = false;
		if (isDirPath(path, &bExists))
			return bExists;

		return false;
	}
	
	struct __stat64 buffer;
    return (_wstat64(path.toCharStr(), &buffer)==0);
}

dbool FileWin32::remove(const UrlString& url)
{
    UrlString path = GetLocal(url);
    return ::DeleteFile(path.toCharStr()) ? true : false;
}

dbool FileWin32::rename(const UrlString& url, const UrlString& urlnew)
{
    UrlString strFile = GetLocal(url);
    UrlString strNewFile = GetLocal(urlnew);

    return ::MoveFile(strFile.toCharStr(), strNewFile.toCharStr()) ? true : false;
}

dbool FileWin32::setHidden(const UrlString &url, dbool hidden)
{
#ifdef DM_OS_WIN32
    UrlString path = GetLocal(url);
    DWORD attributes = hidden ? FILE_ATTRIBUTE_HIDDEN : FILE_ATTRIBUTE_NORMAL;
    if (SetFileAttributesW(path.toCharStr(), attributes))
        return true;
#endif
    return false;
}

static dbool CreateDir(const UrlString& url)
{
	return CreateDirectory(url.toCharStr(), NULL);
}

static dbool RemoveDir(const UrlString& url)
{
	return RemoveDirectory(url.toCharStr());
}

dbool FileWin32::mkdir(const UrlString& url, dbool parent)
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
					bool existed = false;
					if (!isDirPath(chunk, &existed)) {
						if (!existed) {
							if (!CreateDir(chunk))
								return false;
						} else {
							return false;
						}
					}
				}
			}
			return true;
		}

        return CreateDir(path);
	}
    return false;
}

dbool FileWin32::rmdir(const UrlString& url, dbool parent)
{
	UrlString path = GetLocal(url);
    if (!path.isEmpty())
	{
		if (parent)
		{
			for (int oldslash = 0, slash=path.length(); slash > 0; oldslash = slash) {
				UrlString chunk = path.left(slash);
				if (chunk.length() == 2 && chunk.at(1) == ':')
					break;
				if (!isDirPath(chunk, 0))
					return false;
				if (!RemoveDir(chunk))
					return oldslash != 0;
				slash = path.lastIndexOf(Dir::separator().toStrChar(), oldslash-1);
			}
			return true;
		}

        return RemoveDir(path);
	}
    return false;
}

dbool FileWin32::getFileInfos(const UrlString& url, FileInfoList &list, dint fileFilter)
{
    list.clear();
    UrlString path = GetLocal(url);
    if (!path.isEmpty())
    {
        Dir::addSlashAtEnd(path);
        WIN32_FIND_DATA findData;
        /*
        dbool haveData = false;
        int infoLevel = 0 ;         // FindExInfoStandard;
        DWORD dwAdditionalFlags  = 0;
        dint searchOps =  0;         // FindExSearchNameMatch
        
        //if (onlyDirs) //dir only
        //    searchOps = 1 ;             // FindExSearchLimitToDirectories
        HANDLE fileHandle = FindFirstFileEx((const wchar_t *)path.toCharStr(), FINDEX_INFO_LEVELS(infoLevel), &findData,
            FINDEX_SEARCH_OPS(searchOps), 0, dwAdditionalFlags);
            */
        //UrlString searchMask = path + UrlString("*.*");
        UrlString searchMask = path + UrlString("*");
        HANDLE fileHandle = FindFirstFile(searchMask.toCharStr(), &findData);
        if (fileHandle == INVALID_HANDLE_VALUE)
            return exists(url, true);

        do {
            UrlString filename = findData.cFileName;
            if (filename.isEmpty())
                continue;

            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (fileFilter & FileInfo::eDir)
                    continue;

                if (filename != "." && filename != "..")
                {
                    list.append(FileInfo(path + filename, FileInfo::eDir));
                }
            }
            else
            {
                if (fileFilter & FileInfo::eFile)
                    continue;
                list.append(FileInfo(path + filename, FileInfo::eFile));
            }
        } 
        while (FindNextFile(fileHandle, &findData));
        FindClose(fileHandle);
        return true;
    }
    return false;
}

dbool FileWin32::copyTo(const UrlString& destUrl)
{
    if (destUrl.isEmpty())
        return false;

    UrlString destFile = GetLocal(destUrl);

    return ::CopyFile(pdm->url.toCharStr(), destFile.toCharStr(), true)  != 0;
}

FileWin32::FileWin32():
    C_D(FileWin32)
{
    pdm->fileHandle = INVALID_HANDLE_VALUE;
    pdm->pos = 0;
    pdm->size = 0;
}

FileWin32::FileWin32(const UrlString& url):
    C_D(FileWin32)
{
    pdm->url = GetLocal(url);
    pdm->fileHandle = INVALID_HANDLE_VALUE;
    pdm->pos = 0;
    pdm->size = 0;
}

FileWin32::~FileWin32()
{
    close();

    D_D(FileWin32);
}

dbool FileWin32::open(EOpenMode mode)
{
    if (pdm->url.isEmpty())
        return false;

    DWORD createMode = 0, access = 0, shareMode = 0, flagsAndAttributes = 0;

    if (mode == eO_ReadOnly)
    {
        access = GENERIC_READ;
        shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
        createMode = OPEN_EXISTING;
    }
    else if (mode & eO_WriteOnly)
    {
        access = GENERIC_READ | GENERIC_WRITE;
        shareMode = FILE_SHARE_READ;
        createMode = (mode & eO_OverWrite) ? CREATE_ALWAYS : OPEN_ALWAYS;
        flagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
    }
    

    SECURITY_ATTRIBUTES securityAtts = { sizeof(SECURITY_ATTRIBUTES), NULL, FALSE };

    DWORD creationDisp = OPEN_EXISTING;
    // Create the file handle.
    pdm->fileHandle = CreateFile(pdm->url.toCharStr(), access, shareMode, &securityAtts, createMode, flagsAndAttributes, NULL);

    // Bail out on error.
    if (pdm->fileHandle == INVALID_HANDLE_VALUE) {
        return false;
    }
    return true;
}

dbool FileWin32::open(const UrlString& url, EOpenMode mode)
{
    pdm->url = GetLocal(url);
    return open(mode);
}

dint FileWin32::stat(const UrlString& url, struct __stat64* buffer)
{
    UrlString strFile = GetLocal(url);
    return _wstat64(strFile.toCharStr(), buffer);
}

dint FileWin32::stat(struct __stat64* buffer)
{
    HANDLE hFileDup;
    if (0 == DuplicateHandle(GetCurrentProcess(), (HANDLE)pdm->fileHandle, GetCurrentProcess(), &hFileDup, 0, FALSE, DUPLICATE_SAME_ACCESS))
    {
        DM_LOGW(" - DuplicateHandle()");
        return -1;
    }

    int fd;
    fd = _open_osfhandle((intptr_t)((HANDLE)hFileDup), 0);
    if (fd == -1)
    {
        DM_LOGW("Stat: fd == -1");
        return -1;
    }
    int result = _fstat64(fd, buffer);
    _close(fd);
    return result;
}

duint FileWin32::read(void* lpBuf, dint64 uiBufSize)
{
    if (!pdm->isVaild()) return 0;
    DWORD nBytesRead;
    if ( ReadFile((HANDLE)pdm->fileHandle, lpBuf, (DWORD)uiBufSize, &nBytesRead, NULL) )
    {
        pdm->pos += nBytesRead;
        return nBytesRead;
    }
    return 0;
}

dint FileWin32::write(const void* lpBuf, dint64 uiBufSize) 
{ 
    if (!pdm->isVaild())
        return 0;

    DWORD nBytesWriten;
    if ( WriteFile((HANDLE)pdm->fileHandle, (void*) lpBuf, (DWORD)uiBufSize, &nBytesWriten, NULL) )
        return nBytesWriten;

    return 0;
}

dint64 FileWin32::seek(dint64 iFilePosition, int iWhence /*= SEEK_SET*/)
{
    if (!pdm->isVaild())
        return -1;

    LARGE_INTEGER lPos, lNewPos;
    lPos.QuadPart = iFilePosition;
    int bSuccess;

    switch (iWhence)
    {
    case SEEK_SET:
        bSuccess = SetFilePointerEx((HANDLE)pdm->fileHandle, lPos, &lNewPos, FILE_BEGIN);
        break;

    case SEEK_CUR:
        bSuccess = SetFilePointerEx((HANDLE)pdm->fileHandle, lPos, &lNewPos, FILE_CURRENT);
        break;

    case SEEK_END:
        bSuccess = SetFilePointerEx((HANDLE)pdm->fileHandle, lPos, &lNewPos, FILE_END);
        break;

    default:
        return -1;
    }
    if (bSuccess)
    {
        pdm->pos = lNewPos.QuadPart;
        return pdm->pos;
    }
    else
        return -1;
}

void FileWin32::close()
{
    if (pdm->isVaild()) {
        ::CloseHandle(pdm->fileHandle);
        pdm->fileHandle = INVALID_HANDLE_VALUE;
        //pdm->url.clear();
        pdm->pos = 0;
        pdm->size = 0;
    }
}

dint64 FileWin32::getPosition() const
{
    return pdm->pos;
}

dint64 FileWin32::getLength() const
{
    if(pdm->isVaild() && (pdm->size <= pdm->pos || pdm->size == 0))
    {
        LARGE_INTEGER i64Size;
        if(GetFileSizeEx((HANDLE)pdm->fileHandle, &i64Size))
            pdm->size = i64Size.QuadPart;
        else
            DM_LOGW("CHDFile::GetLength - GetFileSizeEx failed with error %d", GetLastError());
    }
    return pdm->size;
}

void FileWin32::flush()
{
    if (pdm->isVaild())
        ::FlushFileBuffers(pdm->fileHandle);
}

int FileWin32::ioctl(EIoControl request, void* param)
{
    return -1;
}

const UrlString& FileWin32::path() const
{
    return pdm->url;
}

dbool FileWin32::exists() const
{
    return exists(pdm->url);
}

dbool FileWin32::remove()
{
    return remove(pdm->url);
}

dbool FileWin32::rename(const UrlString& urlnew)
{
    return rename(pdm->url, urlnew);
}

dbool FileWin32::setHidden(dbool hidden)
{
    return setHidden(pdm->url, hidden);
}

DM_END_NAMESPACE
