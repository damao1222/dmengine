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
     * 构造函数
     */
    PairList();

    /** 
     * 拷贝构造函数
     * @param other  拷贝对象
     */
    PairList(const PairList<T1, T2> &other);

    /** 
     * 插入一个对值，首先查找是否已经存在key一样的对值，如果存在，则使用新值对其覆盖
     * @param key  键
     * @param value  值
     */
    void insert(const T1 &key, const T2 &value);

    /** 
     * 直接在末尾插入一个对值，不做任何检查
     * @param key  键
     * @param value  值
     */
    void insertMulti(const T1 &key, const T2 &value);

    /** 
     * 获取第一个与key对应的值
     * @param key  键
     * @return 值
     */
    const T2 value(const T1 &key) const;

    /** 
     * 获取第一个与key对应的值，如果没有查找到则返回default值
     * @param key  键
     * @param defaultValue  默认值
     * @return 值
     */
    const T2 value(const T1 &key, const T2 &defaultValue) const;

    /** 
     * 获取第一个与value对应的键
     * @param value  值
     * @return 键
     */
    const T1 key(const T2 &value) const;

    /** 
     * 获取第一个与value对应的键，如果没有查找到则返回default值
     * @param value  值
     * @param defaultKey  默认键
     * @return 键
     */
    const T1 key(const T2 &value, const T1 &defaultKey) const;

    /** 
     * 清楚所有键值对
     */
    void clear();

    /** 
     * 键值对大小
     * @return 键值对总数
     */
    dint size() const;

    /** 
     * 键值对大小
     * @return 键值对总数
     */
    dint count() const;

    /** 
     * 判断键值对是否为空
     * @return 空功返回true，否则返回false
     */
    dbool isEmpty() const;

    /** 
     * 获得键值对所有键
     * @return 所有键的vector
     */
    Vector<T1> keys() const;

    /** 
     * 获得键值对所有的值
     * @return 所有值的vector
     */
    Vector<T2> values() const;

    /** 
     * 获得键值对中所有与key对应的值（使用insertMulti时可插入多个同key的value）
     * @param key  键
     * @return 值的vector
     */
    Vector<T2> values(const T1 &key) const;

    /** 
     * 获得在索引上的键值对
     * @param index  索引
     * @return 键值对
     */
    const Pair<T1, T2>& at(dint index) const;

    /** 
     * 获得在索引上的键值对引用
     * @param index  索引
     * @return 键值对
     */
    Pair<T1, T2>& _at(dint index);

    /** 
     * 删除一个与key相对应的键值对
     * @param key  键
     * @return 成功返回true，失败返回false
     */
    dbool removeOneByKey(const T1 &key);

    /** 
     * 删除一个与value相对应的键值对
     * @param value  值
     * @return 成功返回true，失败返回false
     */
    dbool removeOneByValue(const T2 &value);

    /** 
     * 删除所有与key相对应的键值对
     * @param key  键
     * @return 删除键值对的个数
     */
    dint removeAllByKey(const T1 &key);

    /** 
     * 删除所有与value相对应的键值对
     * @param value  值
     * @return 删除键值对的个数
     */
    dint removeAllByValue(const T2 &value);

    /** 
     * 运算符重载，获取第一个与key对应的值
     * @param key  键
     * @return 值
     */
    const T2 operator[](const T1 &key) const;

    /** 
     * 赋值运算符重载
     * @param other  赋值对象
     * @return 当前对象的实例
     */
    PairList<T1, T2>& operator=(const PairList<T1, T2> &other);

    /** 
     * 等于运算符重载
     * @param other  比较对象
     * @return 相等返回true，否则返回false
     */
    dbool operator==(const PairList<T1, T2> &other);

    /** 
     * 不等运算符重载
     * @param other  比较对象
     * @return 不相等返回true，否则返回false
     */
    dbool operator!=(const PairList<T1, T2> &other);

    /** 
     * 数据
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
