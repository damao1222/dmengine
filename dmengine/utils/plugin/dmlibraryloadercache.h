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
     * װ��һ����̬��.
     * @param libraryFile  ��̬��·��.
     * @return װ�سɹ�����װ�غ��loader�����򷵻ؿ�.
     */
    LibraryLoader* loadLibrary(const UrlString &libraryFile);

    /** 
     *  ж��һ����̬��.
     * @param libraryFile  ��̬��·��.
     * @return �ɹ�����true��ʧ�ܷ���false
     */
    dbool unloadLibrary(const UrlString &libraryFile);

    /** 
     *  ж��һ����̬��.
     * @param loader  װ�ض�̬���loader.
     * @return �ɹ�����true��ʧ�ܷ���false
     */
    dbool unloadLibrary(LibraryLoader *loader);

    /** 
     * ������л���Ķ�̬��·��
     * @return ·���б�
     */
    UrlStringList cachedLibs() const;

private:
    /** 
     * ���캯��
     */
    LibraryLoaderCache();

    /** 
     * ��������
     */
    ~LibraryLoaderCache();

    //internal
    friend class Singleton<LibraryLoaderCache>;
 };

/** 
 * @brief ���һ��LibraryLoaderʵ��.
 */
class DM_DLL_EXPORT LibraryLoaderFactory
{
public:
    /** 
     * ����LibraryLoader
     * @param libraryFile  ��·��
     * @return LibraryLoaderʵ��
     */
    static LibraryLoader* create(const UrlString &libraryFile);
};
DM_END_NAMESPACE

#define dmLibLoaderCache (*DM::LibraryLoaderCache::getInstance())

#endif // DMLIBRARYLOADER_H
