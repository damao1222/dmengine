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
#include "dmfilehelper.h"
DM_BEGIN_NAMESPACE

UrlString GetLocal(const UrlString &url)
{
    UrlString path(url);
#if 0
    dint index = url.indexOf("file");
    if(index > -1)
    {
        // file://drive[:]/path
        // file:///drive:/path
        CStdString host( url.GetHostName() );

        if(host.size() > 0)
        {
            if(host.Right(1) == ":")
            path = host + "/" + path;
            else
            path = host + ":/" + path;
        }
    }
#endif
#ifdef DM_OS_WIN
    path.replace('/', '\\');
#endif

    if (IsAliasShortcut(path))
        TranslateAliasShortcut(path);

    return path;
}

dbool IsAliasShortcut(UrlString &path)
{
  bool  rtn = false;

#if defined(DM_OS_MAC)
  // Note: regular files that have an .alias extension can be
  //   reported as an alias when clearly, they are not. Trap them out.
  if (URIUtils::GetExtension(path) != ".alias")
  {
    FSRef fileRef;
    Boolean targetIsFolder, wasAliased;

    // It is better to call FSPathMakeRefWithOptions and pass kFSPathMakeRefDefaultOptions
    //   since it will succeed for paths such as "/Volumes" unlike FSPathMakeRef.
    if (noErr == FSPathMakeRefWithOptions((UInt8*)path.c_str(), kFSPathMakeRefDefaultOptions, &fileRef, NULL))
    {
      if (noErr == FSIsAliasFile(&fileRef, &wasAliased, &targetIsFolder))
      {
        if (wasAliased)
        {
          rtn = true;
        }
      }
    }
  }
#elif defined(DM_OS_LINUX)
  // Linux does not use alias or shortcut methods
#elif defined(DM_OS_WIN)
/* Needs testing under Windows platform so ignore shortcuts for now
    if (CUtil::GetExtension(path) == ".lnk")
    {
      rtn = true;
    }
*/
#endif
  return(rtn);
}

void TranslateAliasShortcut(UrlString &path)
{
#if defined(DM_OS_MAC)
  FSRef fileRef;
  Boolean targetIsFolder, wasAliased;

  if (noErr == FSPathMakeRefWithOptions((UInt8*)path.c_str(), kFSPathMakeRefDefaultOptions, &fileRef, NULL))
  {
    if (noErr == FSResolveAliasFileWithMountFlags(&fileRef, TRUE, &targetIsFolder, &wasAliased, kResolveAliasFileNoUI))
    {
      if (wasAliased)
      {
        char real_path[PATH_MAX];
        if (noErr == FSRefMakePath(&fileRef, (UInt8*)real_path, PATH_MAX))
        {
          path = real_path;
        }
      }
    }
  }
#elif defined(DM_OS_LINUX)
  // Linux does not use alias or shortcut methods

#elif defined(DM_OS_WIN)
/* Needs testing under Windows platform so ignore shortcuts for now
  CComPtr<IShellLink> ipShellLink;

  // Get a pointer to the IShellLink interface
  if (NOERROR == CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&ipShellLink))
    WCHAR wszTemp[MAX_PATH];

    // Get a pointer to the IPersistFile interface
    CComQIPtr<IPersistFile> ipPersistFile(ipShellLink);

    // IPersistFile is using LPCOLESTR so make sure that the string is Unicode
#if !defined _UNICODE
    MultiByteToWideChar(CP_ACP, 0, lpszShortcutPath, -1, wszTemp, MAX_PATH);
#else
    wcsncpy(wszTemp, lpszShortcutPath, MAX_PATH);
#endif

    // Open the shortcut file and initialize it from its contents
    if (NOERROR == ipPersistFile->Load(wszTemp, STGM_READ))
    {
      // Try to find the target of a shortcut even if it has been moved or renamed
      if (NOERROR == ipShellLink->Resolve(NULL, SLR_UPDATE))
      {
        WIN32_FIND_DATA wfd;
        TCHAR real_path[PATH_MAX];
        // Get the path to the shortcut target
        if (NOERROR == ipShellLink->GetPath(real_path, MAX_PATH, &wfd, SLGP_RAWPATH))
        {
          // Get the description of the target
          TCHAR szDesc[MAX_PATH];
          if (NOERROR == ipShellLink->GetDescription(szDesc, MAX_PATH))
          {
            path = real_path;
          }
        }
      }
    }
  }
*/
#endif
}
DM_END_NAMESPACE
