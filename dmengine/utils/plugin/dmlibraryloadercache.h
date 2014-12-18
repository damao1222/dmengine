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
#ifndef DMLIBRARYLOADERCACHE_H
#define DMLIBRARYLOADERCACHE_H

#include "dmsingleton.h"
#include "dmutilstring.h"

DM_BEGIN_NAMESPACE
class LibraryLoader;
DM_PRIVATE_CLASS(LibraryLoaderCache);
class DM_DLL_EXPORT LibraryLoaderCache : public Singleton<LibraryLoaderCache>
{
    DM_DECLARE_PRIVATE(LibraryLoaderCache)
public:
    /** 
     * 装载一个动态库.
     * @param libraryFile  动态库路径.
     * @return 装载成功返回装载后的loader，否则返回空.
     */
    LibraryLoader* loadLibrary(const UrlString &libraryFile);

    /** 
     *  卸载一个动态库.
     * @param libraryFile  动态库路径.
     * @return 成功返回true，失败返回false
     */
    dbool unloadLibrary(const UrlString &libraryFile);

    /** 
     *  卸载一个动态库.
     * @param loader  装载动态库的loader.
     * @return 成功返回true，失败返回false
     */
    dbool unloadLibrary(LibraryLoader *loader);

    /** 
     * 获得所有缓存的动态库路径
     * @return 路径列表
     */
    UrlStringList cachedLibs() const;

private:
    /** 
     * 构造函数
     */
    LibraryLoaderCache();

    /** 
     * 析构函数
     */
    ~LibraryLoaderCache();

    //internal
    friend class Singleton<LibraryLoaderCache>;
 };

/** 
 * @brief 获得一个LibraryLoader实例.
 */
class DM_DLL_EXPORT LibraryLoaderFactory
{
public:
    /** 
     * 创建LibraryLoader
     * @param libraryFile  库路径
     * @return LibraryLoader实例
     */
    static LibraryLoader* create(const UrlString &libraryFile);
};
DM_END_NAMESPACE

#define dmLibLoaderCache (*DM::LibraryLoaderCache::getInstance())

#endif // DMLIBRARYLOADER_H
