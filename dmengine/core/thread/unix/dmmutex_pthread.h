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

#ifndef DMMUTEX_PTHREAD_H
#define DMMUTEX_PTHREAD_H

#include <pthread.h>
#include <errno.h>
#include "dmglobal.h"

DM_BEGIN_NAMESPACE
class DM_DLL_EXPORT Mutex
{
public:
    inline Mutex() 
    { 
        pthread_mutex_init(&mutex, NULL); 
    }

    inline ~Mutex() 
    { 
        pthread_mutex_destroy(&mutex); 
    }

    inline void lock() 
    { 
        pthread_mutex_lock(&mutex); 
    }

    inline void unlock() 
    { 
        pthread_mutex_unlock(&mutex);
    }

    inline dbool tryLock() 
    { 
        return (pthread_mutex_trylock(&mutex) == 0);
    }

    inline pthread_mutex_t& nativeLock() 
    {
        return mutex;
    }
private:
    pthread_mutex_t mutex;
};
DM_END_NAMESPACE

#endif // DMMUTEX_PTHREAD_H
