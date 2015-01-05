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
#ifndef DMCLASSLOADER_H
#define DMCLASSLOADER_H

#include "private/dmclassloader_p.h"

DM_BEGIN_NAMESPACE

template <typename T>
class ClassLoader
{
    ClassLoaderData d;
public:
    /** 
     * 默认构造函数
     */
    ClassLoader();

    /** 
     * 构造函数
     * @param libraryFile  动态库路径
     */
    explicit ClassLoader(const UrlString &libraryFile);

    /** 
     * 拷贝构造函数
     * @param other  另一个ClassLoader
     */
    ClassLoader(const ClassLoader &other);

    /** 
     * 析构函数，将卸载动态库
     */
    virtual ~ClassLoader();

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
     * 获得当前对象实例
     * @return 实例指针
     */
    T* newInstance();

    /** 
     * 销毁当前对象实例
     * @param p  对象指针
     * @return 如果库中包含destroy函数，则返回true，否则返回false
     */
    dbool deleteInstance(T* p);

    /** 
     * 赋值运算符
     * @param other  拷贝的对象
     * @return 当前实例引用
     */
    ClassLoader& operator=(const ClassLoader &other);
};


/*
 ===========================================================================================
 = internal
 ===========================================================================================
 */

template <typename T>
inline ClassLoader<T>::ClassLoader()
{
}

template <typename T>
inline ClassLoader<T>::ClassLoader(const UrlString &libraryFile):
    d(libraryFile)
{
}

template <typename T>
inline ClassLoader<T>::ClassLoader(const ClassLoader &other) : 
    d(other.d)
{
}

template <typename T>
ClassLoader<T>::~ClassLoader()
{
    d.final();
}

template <typename T>
dbool ClassLoader<T>::load()
{
    return d.load();
}

template <typename T>
void ClassLoader<T>::unload()
{
    d.unload();
}

template <typename T>
inline dint ClassLoader<T>::version() const
{
    return d.version();
}

template <typename T>
inline const char* ClassLoader<T>::className() const
{
    return d.name();
}

template <typename T>
T* ClassLoader<T>::newInstance()
{
    return static_cast<T*>(d.create());
}

template <typename T>
dbool ClassLoader<T>::deleteInstance(T* p)
{
    return d.destroy(p);
}

template <typename T>
ClassLoader<T>& ClassLoader<T>::operator=(const ClassLoader &other)
{
    d = other.d;
    return *this;
}

DM_END_NAMESPACE

#endif // DMCLASSLOADER_H
