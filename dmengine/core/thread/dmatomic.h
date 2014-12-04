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

#ifndef DMATOMIC_H
#define DMATOMIC_H
#include "dmglobal.h"
DM_BEGIN_NAMESPACE

/*
 * 32-bit atomic compare-and-swap
 * Returns previous value of *pAddr
 */
DM_DLL_EXPORT long CAS(volatile long *pAddr, long expectedVal, long swapVal);

/*
 * 64-bit atomic compare-and-swap
 * Returns previous value of *pAddr
 */
#if !defined(__ppc__) && !defined(__powerpc__) && !defined(__arm__)
DM_DLL_EXPORT long long CAS2(volatile long long* pAddr, long long expectedVal, long long swapVal);
#endif

/*
 * 32-bit atomic increment
 * Returns new value of *pAddr
 */
DM_DLL_EXPORT long AtomicIncrement(volatile long* pAddr);

/*
 * 32-bit atomic decrement
 * Returns new value of *pAddr
 */
DM_DLL_EXPORT long AtomicDecrement(volatile long* pAddr);

/*
 * 32-bit atomic add
 * Returns new value of *pAddr
 */
DM_DLL_EXPORT long AtomicAdd(volatile long* pAddr, long amount);

/*
 * 32-bit atomic subtract
 * Returns new value of *pAddr
 */
DM_DLL_EXPORT long AtomicSubtract(volatile long* pAddr, long amount);

/*
 * Fast spinlock implmentation. No backoff when busy
 */
class DM_DLL_EXPORT AtomicSpinLock
{
public:
    AtomicSpinLock(volatile long& lock);
    ~AtomicSpinLock();

private:
    volatile long &m_lock;
};

#define DM_SPINLOCK_ACQUIRE(l) while(DM::CAS(&l, 0, 1)) {}
#define DM_SPINLOCK_RELEASE(l) l = 0

DM_END_NAMESPACE
#endif // DMATOMIC_H
