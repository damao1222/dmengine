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
     * ���캯��
     */
    Library();

    /** 
     * ���캯��
     * @param libraryFile  ��̬��·��
     */
    explicit Library(const UrlString &libraryFile);

    /** 
     * �������캯����ע�⣺��������clazz������newһ���µĶ�����other��clazz��ͬ
     * @param libraryFile  ��̬��·��
     */
    Library(const Library &other);

    /** 
     * ������������ж�ض�̬��
     */
    virtual ~Library();

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
     * ���ز���������ָ�룬���������ز��ɹ�����NULL
     * @return ���������ָ��
     */
    T* clazz();

    /** 
     * ָ����������أ����ز���������ָ�룬���������ز��ɹ�����NULL
     * @return ���������ָ��
     */
    inline T* operator->();

    /** 
     * ָ����������أ����ز���������ָ�룬���������ز��ɹ�����NULL
     * @return ���������ָ��
     */
    inline const T* operator->() const;
    
    /** 
     * ��ֵ�������ע�⣺��������clazz������newһ���µĶ�����other��clazz��ͬ
     * @param other  �����Ķ���
     * @return ��ǰʵ������
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
