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

#ifndef DMTHREAD_PTHREAD_H
#define DMTHREAD_PTHREAD_H
#include "pthread.h"
#define PTHREAD_CANCEL_DISABLE 1
#define PTHREAD_CANCEL_ENABLE 2
#define PTHREAD_INHERIT_SCHED 3
typedef pthread_t ThreadIdentifier;
typedef int THREADFUNC;
#define DM_INVALID_THREADID 0
#endif // DMTHREAD_PTHREAD_H
