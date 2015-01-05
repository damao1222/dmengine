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
#ifndef DMPAIR_H
#define DMPAIR_H
#include "dmnamespace.h"
DM_BEGIN_NAMESPACE

template <typename T1, typename T2>
struct Pair
{
    typedef T1 first_type;
    typedef T2 second_type;

    /** 
     * ���캯��
     */
    Pair() : first(T1()), second(T2()) {}

    /** 
     * ���캯��
     * @param t1  ��ֵ��һ��Ԫ��
     * @param t2  ��ֵ�ڶ���Ԫ��
     */
    Pair(const T1 &t1, const T2 &t2) : first(t1), second(t2) {}

    /** 
     * ��ֵ�����
     * @param other  ��ֵ����
     * @return ��ǰʵ��
     */
    Pair<T1, T2> &operator=(const Pair<T1, T2> &other)
    { first = other.first; second = other.second; return *this; }

    /** 
     * ��ֵ�ĵ�һ��Ԫ��
     */
    T1 first;

    /** 
     * ��ֵ�ĵڶ���Ԫ��
     */
    T2 second;
};


template <class T1, class T2>
inline dbool operator==(const Pair<T1, T2> &p1, const Pair<T1, T2> &p2)
{ return p1.first == p2.first && p1.second == p2.second; }

template <class T1, class T2>
inline dbool operator!=(const Pair<T1, T2> &p1, const Pair<T1, T2> &p2)
{ return !(p1 == p2); }

template <class T1, class T2>
inline dbool operator<(const Pair<T1, T2> &p1, const Pair<T1, T2> &p2)
{
    return p1.first < p2.first || (!(p2.first < p1.first) && p1.second < p2.second);
}

template <class T1, class T2>
inline dbool operator>(const Pair<T1, T2> &p1, const Pair<T1, T2> &p2)
{
    return p2 < p1;
}

template <class T1, class T2>
inline dbool operator<=(const Pair<T1, T2> &p1, const Pair<T1, T2> &p2)
{
    return !(p2 < p1);
}

template <class T1, class T2>
inline dbool operator>=(const Pair<T1, T2> &p1, const Pair<T1, T2> &p2)
{
    return !(p1 < p2);
}

template <class T1, class T2>
inline Pair<T1, T2> dmMakePair(const T1 &x, const T2 &y)
{
    return Pair<T1, T2>(x, y);
}

DM_END_NAMESPACE
#endif // DMPAIR_H
