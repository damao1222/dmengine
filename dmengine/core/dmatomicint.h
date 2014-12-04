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

#ifndef DMATOMICINT_H
#define DMATOMICINT_H
#include "dmnamespace.h"
DM_BEGIN_NAMESPACE
class DM_DLL_EXPORT BasicAtomicInt
{
public:
    /** 
     * ��������أ��ж�ԭ�ӱ�����intֵ�Ƿ����
     * @param value �Ƚϵ�intֵ
     * @return ��ȷ���true�����򷵻�false
     */
    inline dbool operator==(int value) const
    {
        return _value_ == value;
    }

    /** 
     * ��������أ��ж�ԭ�ӱ�����intֵ�Ƿ����
     * @param value �Ƚϵ�intֵ
     * @return ��ȷ���false�����򷵻�true
     */
    inline dbool operator!=(int value) const
    {
        return _value_ != value;
    }

    /** 
     * ��������أ��ж�ԭ�ӱ����Ƿ�Ϊ0
     * @return Ϊ0����true�����򷵻�false
     */
    inline dbool operator!() const
    {
        return _value_ == 0;
    }

    /** 
     * ��������أ�intת��
     * @return ���ص�ǰԭ�ӱ���ֵ
     */
    inline operator int() const
    {
        return _value_;
    }

    /** 
     * ��ֵ���������
     * @param value Ŀ��intֵ
     * @return ��ǰʵ��������
     */
    inline BasicAtomicInt &operator=(int value)
    {
        _value_ = value;
        return *this;
    }

    /** 
     * ����
     * @return ������0����true������0����false
     */
    dbool ref();

    /** 
     * �Լ�
     * @return ������0����true������0����false
     */
    dbool deref();

#if defined(DM_OS_WIN)
    union { // needed for DM_BASIC_ATOMIC_INITIALIZER
        volatile dlong _value_;
    };
#else
    volatile dlong _value_;
#endif
};

class DM_DLL_EXPORT AtomicInt: public BasicAtomicInt
{
public:
    /** 
     * ���캯��
     * @param v ��ʼֵ
     */
    AtomicInt(dint v = 0) { _value_ = v; }

    /** 
     * �������캯��
     * @param other ��ֵ����
     */
    AtomicInt(const AtomicInt &other) { _value_ = other._value_; }

    /** 
     * ��ֵ�����
     * @param value Ŀ��intֵ
     * @return ��ǰʵ��������
     */
    inline AtomicInt &operator=(int value)
    {
        (void) BasicAtomicInt::operator=(value);
        return *this;
    }

    /** 
     * ��ֵ�����
     * @param other ��ֵ����
     * @return ��ǰʵ��������
     */
    inline AtomicInt &operator=(const AtomicInt &other)
    {
        (void) BasicAtomicInt::operator=(other);
        return *this;
    }
};
DM_END_NAMESPACE

#include "arch/dmatomicint_arch.h"

#endif // DMATOMICINT_H
