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
#ifndef DMPAIRLIST_H
#define DMPAIRLIST_H
#include "dmpair.h"
#include "dmvector.h"

DM_BEGIN_NAMESPACE
template <typename T1, typename T2>
class PairList
{
public:
    /** 
     * ���캯��
     */
    PairList();

    /** 
     * �������캯��
     * @param other  ��������
     */
    PairList(const PairList<T1, T2> &other);

    /** 
     * ����һ����ֵ�����Ȳ����Ƿ��Ѿ�����keyһ���Ķ�ֵ��������ڣ���ʹ����ֵ���串��
     * @param key  ��
     * @param value  ֵ
     */
    void insert(const T1 &key, const T2 &value);

    /** 
     * ֱ����ĩβ����һ����ֵ�������κμ��
     * @param key  ��
     * @param value  ֵ
     */
    void insertMulti(const T1 &key, const T2 &value);

    /** 
     * ��ȡ��һ����key��Ӧ��ֵ
     * @param key  ��
     * @return ֵ
     */
    const T2 value(const T1 &key) const;

    /** 
     * ��ȡ��һ����key��Ӧ��ֵ�����û�в��ҵ��򷵻�defaultֵ
     * @param key  ��
     * @param defaultValue  Ĭ��ֵ
     * @return ֵ
     */
    const T2 value(const T1 &key, const T2 &defaultValue) const;

    /** 
     * ��ȡ��һ����value��Ӧ�ļ�
     * @param value  ֵ
     * @return ��
     */
    const T1 key(const T2 &value) const;

    /** 
     * ��ȡ��һ����value��Ӧ�ļ������û�в��ҵ��򷵻�defaultֵ
     * @param value  ֵ
     * @param defaultKey  Ĭ�ϼ�
     * @return ��
     */
    const T1 key(const T2 &value, const T1 &defaultKey) const;

    /** 
     * ������м�ֵ��
     */
    void clear();

    /** 
     * ��ֵ�Դ�С
     * @return ��ֵ������
     */
    dint size() const;

    /** 
     * ��ֵ�Դ�С
     * @return ��ֵ������
     */
    dint count() const;

    /** 
     * �жϼ�ֵ���Ƿ�Ϊ��
     * @return �չ�����true�����򷵻�false
     */
    dbool isEmpty() const;

    /** 
     * ��ü�ֵ�����м�
     * @return ���м���vector
     */
    Vector<T1> keys() const;

    /** 
     * ��ü�ֵ�����е�ֵ
     * @return ����ֵ��vector
     */
    Vector<T2> values() const;

    /** 
     * ��ü�ֵ����������key��Ӧ��ֵ��ʹ��insertMultiʱ�ɲ�����ͬkey��value��
     * @param key  ��
     * @return ֵ��vector
     */
    Vector<T2> values(const T1 &key) const;

    /** 
     * ����������ϵļ�ֵ��
     * @param index  ����
     * @return ��ֵ��
     */
    const Pair<T1, T2>& at(dint index) const;

    /** 
     * ����������ϵļ�ֵ������
     * @param index  ����
     * @return ��ֵ��
     */
    Pair<T1, T2>& _at(dint index);

    /** 
     * ɾ��һ����key���Ӧ�ļ�ֵ��
     * @param key  ��
     * @return �ɹ�����true��ʧ�ܷ���false
     */
    dbool removeOneByKey(const T1 &key);

    /** 
     * ɾ��һ����value���Ӧ�ļ�ֵ��
     * @param value  ֵ
     * @return �ɹ�����true��ʧ�ܷ���false
     */
    dbool removeOneByValue(const T2 &value);

    /** 
     * ɾ��������key���Ӧ�ļ�ֵ��
     * @param key  ��
     * @return ɾ����ֵ�Եĸ���
     */
    dint removeAllByKey(const T1 &key);

    /** 
     * ɾ��������value���Ӧ�ļ�ֵ��
     * @param value  ֵ
     * @return ɾ����ֵ�Եĸ���
     */
    dint removeAllByValue(const T2 &value);

    /** 
     * ��������أ���ȡ��һ����key��Ӧ��ֵ
     * @param key  ��
     * @return ֵ
     */
    const T2 operator[](const T1 &key) const;

    /** 
     * ��ֵ���������
     * @param other  ��ֵ����
     * @return ��ǰ�����ʵ��
     */
    PairList<T1, T2>& operator=(const PairList<T1, T2> &other);

    /** 
     * �������������
     * @param other  �Ƚ϶���
     * @return ��ȷ���true�����򷵻�false
     */
    dbool operator==(const PairList<T1, T2> &other);

    /** 
     * �������������
     * @param other  �Ƚ϶���
     * @return ����ȷ���true�����򷵻�false
     */
    dbool operator!=(const PairList<T1, T2> &other);

    /** 
     * ����
     */
    Vector<Pair<T1, T2> > list;
};

template <typename T1, typename T2>
PairList<T1, T2>::PairList()
{ }

template <typename T1, typename T2>
PairList<T1, T2>::PairList(const PairList<T1, T2> &other)
{ list = other.list; }

template <typename T1, typename T2>
void PairList<T1, T2>::insert(const T1 &key, const T2 &value)
{
    Vector<Pair<T1, T2> >::Iterator it = list.begin();
    for (; it != list.constEnd(); ++it)
    {
        if ((*it).first == key)
        {
            (*it).second = value;
            return ;
        }
    }
    list.append(Pair<T1, T2>(key, value));
}

template <typename T1, typename T2>
void PairList<T1, T2>::insertMulti(const T1 &key, const T2 &value)
{
    list.append(Pair<T1, T2>(key, value));
}

template <typename T1, typename T2>
const T2 PairList<T1, T2>::value(const T1 &key) const
{
    for (dint i=0; i<list.size(); ++i)
    {
        const Pair<T1, T2> p = list.at(i);
        if (p.first == key)
        {
            return p.second;
        }
    }
    return T2();
}

template <typename T1, typename T2>
const T2 PairList<T1, T2>::value(const T1 &key, const T2 &defaultValue) const
{
    for (dint i=0; i<list.size(); ++i)
    {
        const Pair<T1, T2> p = list.at(i);
        if (p.first == key)
        {
            return p.second;
        }
    }
    return defaultValue;
}

template <typename T1, typename T2>
const T1 PairList<T1, T2>::key(const T2 &value) const
{
    for (dint i=0; i<list.size(); ++i)
    {
        const Pair<T1, T2> p = list.at(i);
        if (p.second == value)
        {
            return p.first;
        }
    }
    return T1();
}

template <typename T1, typename T2>
const T1 PairList<T1, T2>::key(const T2 &value, const T1 &defaultKey) const
{
    for (dint i=0; i<list.size(); ++i)
    {
        const Pair<T1, T2> p = list.at(i);
        if (p.second == value)
        {
            return p.first;
        }
    }
    return defaultKey;
}

template <typename T1, typename T2>
inline void PairList<T1, T2>::clear()
{ list.clear(); }

template <typename T1, typename T2>
inline  dint PairList<T1, T2>::size() const
{ return list.size(); }

template <typename T1, typename T2>
inline dint PairList<T1, T2>::count() const
{ return list.count(); }

template <typename T1, typename T2>
inline dbool PairList<T1, T2>::isEmpty() const
{
    return list.isEmpty();
}

template <typename T1, typename T2>
Vector<T2> PairList<T1, T2>::values() const
{
    Vector<T2> l;
    for (dint i=0; i<list.size(); ++i)
    {
        l.append(list.at(i).second);
    }
    return l;
}

template <typename T1, typename T2>
Vector<T2> PairList<T1, T2>::values(const T1 &key) const
{
    Vector<T2> l;
    for (dint i=0; i<list.size(); ++i)
    {
        const Pair<T1, T2> &p = list.at(i);
        if (p.first == key)
            l.append(p.second);
    }
    return l;
}

template <typename T1, typename T2>
inline const Pair<T1, T2>& PairList<T1, T2>::at(dint index) const
{ return list.at(index); }

template <typename T1, typename T2>
inline Pair<T1, T2>& PairList<T1, T2>::_at(dint index)
{ return list[index]; }

template <typename T1, typename T2>
dbool PairList<T1, T2>::removeOneByKey(const T1 &key)
{
    Vector<Pair<T1, T2> >::Iterator it = list.begin();
    while (it != list.constEnd())
    {
        if (it->first == key)
        {
            list.erase(it);
            return true;
        }
        else
            ++it;
    }
    return false;
}

template <typename T1, typename T2>
dbool PairList<T1, T2>::removeOneByValue(const T2 &value)
{
    Vector<Pair<T1, T2> >::Iterator it = list.begin();
    while (it != list.constEnd())
    {
        if (it->second == value)
        {
            list.erase(it);
            return true;
        }
        else
            ++it;
    }
    return false;
}

template <typename T1, typename T2>
dint PairList<T1, T2>::removeAllByKey(const T1 &key)
{
    dint nRemoved = 0; 
    Vector<Pair<T1, T2> >::Iterator it = list.begin();
    while (it != list.constEnd())
    {
        if (it->first == key)
        {
            it = list.erase(it);
            ++nRemoved;
        }
        else
            ++it;
    }
    return nRemoved;
}

template <typename T1, typename T2>
dint PairList<T1, T2>::removeAllByValue(const T2 &value)
{
    dint nRemoved = 0; 
    Vector<Pair<T1, T2> >::Iterator it = list.begin();
    while (it != list.constEnd())
    {
        if (it->second == value)
        {
            it = list.erase(it);
            ++nRemoved;
        }
        else
            ++it;
    }
    return nRemoved;
}

template <typename T1, typename T2>
Vector<T1> PairList<T1, T2>::keys() const
{
    Vector<T1> l;
    for (dint i=0; i<list.size(); ++i)
    {
        l.append(list.at(i).first);
    }
    return l;
}

template <typename T1, typename T2>
inline const T2 PairList<T1, T2>::operator[](const T1 &key) const
{ return value(key); }

template <class T1, class T2>
PairList<T1, T2>& PairList<T1, T2>::operator=(const PairList<T1, T2> &other)
{ list = other.list; return *this; }

template <class T1, class T2>
inline dbool PairList<T1, T2>::operator==(const PairList<T1, T2> &other)
{ return list == other.list; }

template <class T1, class T2>
inline dbool PairList<T1, T2>::operator!=(const PairList<T1, T2> &other)
{ return !(*this == other); }

DM_END_NAMESPACE
#endif // DMPAIRLIST_H
