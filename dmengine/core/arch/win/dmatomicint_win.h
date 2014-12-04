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

#ifndef DMATOMICINT_WIN_H
#define DMATOMICINT_WIN_H

//microsoft suck
#ifndef DM_OS_WP8
#include <winsock2.h>
#endif
#include <windows.h>

DM_BEGIN_NAMESPACE
inline dbool BasicAtomicInt::ref()
{
    return InterlockedIncrement(&_value_) != 0;
}

inline dbool BasicAtomicInt::deref()
{
    return InterlockedDecrement(&_value_) != 0;
}

inline long Win_cas(long *lock, long expectedVal, long swapVal)
{
    return InterlockedCompareExchange(lock, swapVal, expectedVal);
}
DM_END_NAMESPACE
#endif // DMATOMICINT_WIN_H
