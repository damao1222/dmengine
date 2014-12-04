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

#ifndef DMCONDITIONIMPL_H
#define DMCONDITIONIMPL_H
#include "thread/dmsinglelock.h"

#if defined(DM_OS_WIN)
#include "../win/dmwinpthreads.h"
#else
#include <pthread.h>
#endif

DM_BEGIN_NAMESPACE

class ConditionVariable
{
private:
    pthread_cond_t cond;

public:
    inline ConditionVariable() 
    { 
#ifdef DM_OS_WIN
		w32thread_init();
#endif
        pthread_cond_init(&cond, NULL); 
    }

    inline ~ConditionVariable() 
    { 
        pthread_cond_destroy(&cond);
    }

    inline void wait(Mutex& lock) 
    { 
        pthread_cond_wait(&cond, &lock.nativeLock());
    }

    inline bool wait(Mutex& lock, unsigned long milliseconds) 
    { 
        struct timespec ts;

    #ifdef DM_OS_DARWIN
        struct timeval tv;
        gettimeofday(&tv, NULL);
        ts.tv_nsec = tv.tv_usec * 1000;
        ts.tv_sec  = tv.tv_sec;
    #elif defined (DM_OS_UNIX)
        clock_gettime(CLOCK_REALTIME, &ts);
    #elif defined (DM_OS_WIN)
        ts.tv_nsec = 0;
        ts.tv_sec = 0;
    #endif

        ts.tv_nsec += milliseconds % 1000 * 1000000;
        ts.tv_sec  += milliseconds / 1000 + ts.tv_nsec / 1000000000;
        ts.tv_nsec %= 1000000000;

        return (pthread_cond_timedwait(&cond, &lock.nativeLock(),&ts) == 0);
    }

    inline void wait(SingleLock& lock) { wait(lock.getMutex()); }
    inline bool wait(SingleLock& lock, unsigned long milliseconds) { return wait(lock.getMutex(), milliseconds); }

    inline void notifyAll() 
    { 
        pthread_cond_broadcast(&cond);
    }

    inline void notify() 
    { 
        pthread_cond_signal(&cond);
    }

    DM_DISABLE_COPY(ConditionVariable);
};

DM_END_NAMESPACE

#endif // DMCONDITIONIMPL_H
