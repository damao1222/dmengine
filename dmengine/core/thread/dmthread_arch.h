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

#ifndef DMTHREAD_ARCH_H
#define DMTHREAD_ARCH_H
#include "dmpublic.h"

#if defined (DM_OS_WIN32)
#include "win/dmthread_win32.h"
//windows phone8
#elif defined (DM_OS_WP8)
#include "win/dmthread_wp8.h"
//pthread
#else
#include "unix/dmthread_pthread.h"
#endif
#endif // DMTHREAD_ARCH_H
