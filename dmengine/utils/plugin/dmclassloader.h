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
     * Ĭ�Ϲ��캯��
     */
    ClassLoader();

    /** 
     * ���캯��
     * @param libraryFile  ��̬��·��
     */
    explicit ClassLoader(const UrlString &libraryFile);

    /** 
     * �������캯��
     * @param other  ��һ��ClassLoader
     */
    ClassLoader(const ClassLoader &other);

    /** 
     * ������������ж�ض�̬��
     */
    virtual ~ClassLoader();

    /** 
     * ���ض�̬��
     * @return ���سɹ�����true�����򷵻�false
     */
    virtual dbool load();

    /** 
     * ж�ض�̬��
     */
    virtual void unload();

    /** 
     * ��ÿ�İ汾�����û�а汾�ŷ���0
     * @return �汾��
     */
    virtual dint version() const;

    /** 
     * ����������
     * @return ����а��������򷵻����������򷵻�NULL
     */
    const char* className() const;

    /** 
     * ��õ�ǰ����ʵ��
     * @return ʵ��ָ��
     */
    T* newInstance();

    /** 
     * ���ٵ�ǰ����ʵ��
     * @param p  ����ָ��
     * @return ������а���destroy�������򷵻�true�����򷵻�false
     */
    dbool deleteInstance(T* p);

    /** 
     * ��ֵ�����
     * @param other  �����Ķ���
     * @return ��ǰʵ������
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
