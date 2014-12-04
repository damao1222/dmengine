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
#ifndef DMLIBRARY_H
#define DMLIBRARY_H

#include "dmclassloader.h"

DM_BEGIN_NAMESPACE

template <typename T>
class Library
{
public:
    /** 
     * 构造函数
     */
    Library();

    /** 
     * 构造函数
     * @param libraryFile  动态库路径
     */
    explicit Library(const UrlString &libraryFile);

    /** 
     * 拷贝构造函数，注意：拷贝过后clazz函数将new一个新的对象，与other的clazz不同
     * @param libraryFile  动态库路径
     */
    Library(const Library &other);

    /** 
     * 析构函数，将卸载动态库
     */
    virtual ~Library();

    /** 
     * 加载动态库
     * @return 加载成功返回true，否则返回false
     */
    virtual dbool load();

    /** 
     * 卸载动态库
     */
    virtual void unload();

    /** 
     * 获得库的版本，如果没有版本号返回0
     * @return 版本号
     */
    virtual dint version() const;

    /** 
     * 获得类的名称
     * @return 类库中包含类名则返回类名，否则返回NULL
     */
    const char* className() const;

    /** 
     * 返回插件抽象类的指针，如果插件加载不成功返回NULL
     * @return 插件抽象类指针
     */
    T* clazz();

    /** 
     * 指针运算符重载，返回插件抽象类的指针，如果插件加载不成功返回NULL
     * @return 插件抽象类指针
     */
    inline T* operator->();

    /** 
     * 指针运算符重载，返回插件抽象类的指针，如果插件加载不成功返回NULL
     * @return 插件抽象类指针
     */
    inline const T* operator->() const;
    
    /** 
     * 赋值运算符，注意：拷贝过后clazz函数将new一个新的对象，与other的clazz不同
     * @param other  拷贝的对象
     * @return 当前实例引用
     */
    Library& operator=(const Library &other);
/*
 ===========================================================================================
 = internal
 ===========================================================================================
 */
private:
    ClassLoader<T> loader;
    T *p;
};

template <typename T>
inline Library<T>::Library():
    p(NULL)
{
}

template <typename T>
inline Library<T>::Library(const UrlString &libraryFile):
    loader(libraryFile),
    p(NULL)
{
}


template <typename T>
inline Library<T>::Library(const Library &other):
    loader(other.loader),
    p(NULL)
{
}

template <typename T>
Library<T>::~Library()
{
    unload();
}

template <typename T>
dbool Library<T>::load()
{
    if (loader.load())
    {
        p = loader.newInstance();
    }

    return p != NULL;
}

template <typename T>
void Library<T>::unload()
{
    loader.deleteInstance(p);
    p = NULL;

    loader.unload();
}

template <typename T>
inline dint Library<T>::version() const
{
    return loader.version();
}

template <typename T>
inline const char* Library<T>::className() const
{
    return loader.className();
}

template <typename T>
inline T* Library<T>::clazz()
{
    if (!p && !load())
    {
        return NULL;
    }
    return p;
}

template <typename T>
inline T* Library<T>::operator->()
{
    return clazz();
}

template <typename T>
inline const T* Library<T>::operator->() const
{
    return clazz();
}

template <typename T>
inline Library<T>& Library<T>::operator=(const Library &other)
{
    loader = other.loader;
    p = NULL;
    return *this;
}
DM_END_NAMESPACE

#endif // DMLIBRARY_H
