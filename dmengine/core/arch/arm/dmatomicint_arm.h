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

#ifndef DMATOMICINT_ARM_H
#define DMATOMICINT_ARM_H
#include "thread/dmatomic.h"
DM_BEGIN_NAMESPACE
#ifndef DM_ARCH_ARMV5
#ifdef __ARM_HAVE_DMB
inline dbool BasicAtomicInt::ref()
{
    register long val;
    asm volatile (
        "dmb      ish            \n" // Memory barrier. Make sure all memory accesses appearing before this complete before any that appear after
        "1:                     \n" 
        "ldrex   %0, [%1]       \n" // (val = *pAddr)
        "add     %0,  #1        \n" // (val += 1)
        "strex   r1,  %0, [%1]	\n"
        "cmp     r1,   #0       \n"
        "bne     1b             \n"
        "dmb     ish            \n" // Memory barrier.
        : "=&r" (val)
        : "r"(&_value_)
        : "r1"
    );
    return val != 0;
}

inline dbool BasicAtomicInt::deref()
{
    register long val;
    asm volatile (
        "dmb      ish           \n" // Memory barrier. Make sure all memory accesses appearing before this complete before any that appear after
        "1:                     \n" 
        "ldrex   %0, [%1]       \n" // (val = *pAddr)
        "sub     %0,  #1        \n" // (val -= 1)
        "strex   r1,  %0, [%1]	\n"
        "cmp     r1,   #0       \n"
        "bne     1b             \n"
        "dmb     ish            \n" // Memory barrier.
        : "=&r" (val)
        : "r"(&_value_)
        : "r1"
    );
    return val != 0;
}
#else //__ARM_HAVE_DMB
inline dbool BasicAtomicInt::ref()
{
    register int newValue;
    register int result;
    asm volatile("0:\n"
        "ldrex %[newValue], [%[_value_]]\n"
        "add %[newValue], %[newValue], #1\n"
        "strex %[result], %[newValue], [%[_value_]]\n"
        "teq %[result], #0\n"
        "bne 0b\n"
        : [newValue] "=&r" (newValue),
        [result] "=&r" (result),
        "+m" (_value_)
        : [_value_] "r" (&_value_)
        : "cc", "memory");
    return newValue != 0;
}

inline dbool BasicAtomicInt::deref()
{
    register int newValue;
    register int result;
    asm volatile("0:\n"
        "ldrex %[newValue], [%[_value_]]\n"
        "sub %[newValue], %[newValue], #1\n"
        "strex %[result], %[newValue], [%[_value_]]\n"
        "teq %[result], #0\n"
        "bne 0b\n"
        : [newValue] "=&r" (newValue),
        [result] "=&r" (result),
        "+m" (_value_)
        : [_value_] "r" (&_value_)
        : "cc", "memory");
    return newValue != 0;
}
#endif //__ARM_HAVE_DMB
#else
#error "not support armv5"
#endif
DM_END_NAMESPACE
#endif // DMATOMICINT_ARM_H
