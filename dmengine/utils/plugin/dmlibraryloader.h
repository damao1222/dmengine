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
#ifndef DMLIBRARYLOADER_H
#define DMLIBRARYLOADER_H

#include "dmutilstring.h"

enum{
    eLibResolveFailed = 0,
    eLibResolveSuccess = 1,
};

DM_BEGIN_NAMESPACE
DM_PRIVATE_CLASS(LibraryLoader);
class DM_DLL_EXPORT LibraryLoader
{
    DM_DECLARE_PRIVATE(LibraryLoader)
public:
    /** 
     * 构造函数
     */
    LibraryLoader(const UrlString &libraryFile);

    /** 
     * 析构函数
     */
    virtual ~LibraryLoader();

    /** 
     * 装载库
     * @return 成功返回true，否则返回false
     */
    virtual dbool load() = 0;

    /** 
     * 卸载库
     */
    virtual void unload() = 0;

    /** 
     * 获得symbol的函数指针
     * @param symbol  符号名
     * @param ptr  匹配地址
     * @param logging  是否打印debug信息
     * @return 成功返回1，失败返回0
     */
    virtual dint resolveExport(const char* symbol, void** ptr, dbool logging = true) = 0;

    /** 
     * 是否有符号
     */
    virtual dbool hasSymbols() = 0;

    /** 
     * 获得库名，eg "libplugin.dll"
     * @return 库名
     */
    UrlString getName() const;

    /** 
     * 获得库完整路径，"c://app/libplugin.dll"
     * @return 库路径
     */
    const UrlString& getFullPath() const; 

    /** 
     * 获得库存放目录，"c://app/"
     * @return 目录
     */
    UrlString getDir() const; // 
};
DM_END_NAMESPACE

#endif // DMLIBRARYLOADER_H
