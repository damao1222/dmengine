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

#include "dmutilstring.h"

DM_BEGIN_NAMESPACE
class LibraryLoader;
DM_PRIVATE_CLASS(LibraryLoaderCache);
class DM_DLL_EXPORT LibraryLoaderCache
{
    DM_DECLARE_PRIVATE(LibraryLoaderCache)
public:
    /** 
     * 装载一个动态库（线程安全）.
     * @param libraryFile  动态库路径.
     * @return 装载成功返回装载后的loader，否则返回空.
     */
    LibraryLoader* loadLibrary(const UrlString &libraryFile);

    /** 
     *  卸载一个动态库（线程安全）.
     * @param libraryFile  动态库路径.
     * @return 成功返回true，失败返回false
     */
    dbool unloadLibrary(const UrlString &libraryFile);

    /** 
     *  卸载一个动态库（线程安全）.
     * @param loader  装载动态库的loader.
     * @return 成功返回true，失败返回false
     */
    dbool unloadLibrary(LibraryLoader *loader);

    /** 
     * 获得所有缓存的动态库路径（线程安全）.
     * @return 路径列表
     */
    UrlStringList cachedLibs() const;

    /** 
     * 获得LibraryLoaderCache的单例，如果单例不存在则new一个LibraryLoaderCache对象（线程安全）.
     * @return 返回LibraryLoaderCache单例.
     */
    static LibraryLoaderCache* getInstance();

    /** 
     * 获得LibraryLoaderCache的单例，如果单例存在返回单例指针，否则返回NULL.
     * @return 返回LibraryLoaderCache单例.
     */
    static LibraryLoaderCache* instance();

    /** 
     * 释放LibraryLoaderCache的单例（线程安全）.
     */
    static void releaseInstance();
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
    friend class LibraryLoaderCache;
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

#define dmLibLoaderCache (*DM_NS::LibraryLoaderCache::getInstance())

#endif // DMLIBRARYLOADER_H
