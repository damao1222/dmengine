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

#ifndef DMTHREAD_P_H
#define DMTHREAD_P_H
#include "../dmthread_arch.h"
#include "thread/dmcondition.h"

DM_BEGIN_NAMESPACE
class ThreadPrivate
{
public:
#ifdef DM_OS_WIN
    Dm::HANDLE handle;
#endif
    ThreadIdentifier threadId;
    volatile dbool isFinished;
    duint stackSize;
    dint priority;
    Mutex mutex;
    WaitCondition wait;

    static THREADFUNC staticThread(void *data);
    static void finish(void *arg, dbool lock = true);
};
DM_END_NAMESPACE
#endif // DMTHREAD_P_H
