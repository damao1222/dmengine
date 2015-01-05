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

#ifndef DMATOMICINT_IA64_H
#define DMATOMICINT_IA64_H

DM_BEGIN_NAMESPACE
bool BasicAtomicInt::ref()
{
    register long result;
    __asm__ __volatile__ (
    "lock/xaddq %q0, %1"
    : "=r" (result), "=m" (_value_)
    : "0" ((long) (1)), "m" (_value_));
    return _value_ != 0;
}

bool BasicAtomicInt::deref()
{
    register long result;
    __asm__ __volatile__ (
    "lock/xaddq %q0, %1"
    : "=r" (result), "=m" (_value_)
    : "0" ((long) (-1)), "m" (_value_));
    return _value_ != 0;
}
DM_END_NAMESPACE
#endif // DMATOMICINT_IA64_H