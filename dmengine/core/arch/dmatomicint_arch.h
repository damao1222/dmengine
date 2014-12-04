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

#ifndef DMATOMICINT_ARCH_H
#define DMATOMICINT_ARCH_H

#if defined(DM_OS_WIN)
#include "win/dmatomicint_win.h"
#elif defined(DM_ARCH_ARM)
#include "arm/dmatomicint_arm.h"
#elif defined(DM_ARCH_I386)
#include "i386/dmatomicint_i386.h"
#elif defined(DM_ARCH_IA64)
#include "ia_64/dmatomicint_ia64.h"
#else
#include "dmatomicint_no.h"
#endif

#if defined(DM_OS_WIN)
#  define DM_BASIC_ATOMIC_INITIALIZER(a) { {(a)} }
#else
#  define DM_BASIC_ATOMIC_INITIALIZER(a) { (a) }
#endif

#endif // DMATOMICINT_ARCH_H
