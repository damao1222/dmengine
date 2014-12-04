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
     * 运算符重载，判断原子变量与int值是否相等
     * @param value 比较的int值
     * @return 相等返回true，否则返回false
     */
    inline dbool operator==(int value) const
    {
        return _value_ == value;
    }

    /** 
     * 运算符重载，判断原子变量与int值是否不相等
     * @param value 比较的int值
     * @return 相等返回false，否则返回true
     */
    inline dbool operator!=(int value) const
    {
        return _value_ != value;
    }

    /** 
     * 运算符重载，判断原子变量是否为0
     * @return 为0返回true，否则返回false
     */
    inline dbool operator!() const
    {
        return _value_ == 0;
    }

    /** 
     * 运算符重载，int转换
     * @return 返回当前原子变量值
     */
    inline operator int() const
    {
        return _value_;
    }

    /** 
     * 赋值运算符重载
     * @param value 目标int值
     * @return 当前实例的引用
     */
    inline BasicAtomicInt &operator=(int value)
    {
        _value_ = value;
        return *this;
    }

    /** 
     * 自增
     * @return 不等于0返回true，等于0返回false
     */
    dbool ref();

    /** 
     * 自减
     * @return 不等于0返回true，等于0返回false
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
     * 构造函数
     * @param v 初始值
     */
    AtomicInt(dint v = 0) { _value_ = v; }

    /** 
     * 拷贝构造函数
     * @param other 赋值对象
     */
    AtomicInt(const AtomicInt &other) { _value_ = other._value_; }

    /** 
     * 赋值运算符
     * @param value 目标int值
     * @return 当前实例的引用
     */
    inline AtomicInt &operator=(int value)
    {
        (void) BasicAtomicInt::operator=(value);
        return *this;
    }

    /** 
     * 赋值运算符
     * @param other 赋值对象
     * @return 当前实例的引用
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
