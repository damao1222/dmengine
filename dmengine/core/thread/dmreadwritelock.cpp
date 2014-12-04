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

#include "dmreadwritelock.h"
#include "dmatomicint.h"
#include "dmsinglelock.h"
#include "dmcondition.h"

DM_BEGIN_NAMESPACE
#ifdef DM_READ_HIGH_PRIORITY

class ReadWriteLockPrivate
{
public:
    void doLockUnlock(dint delta, dint op, SingleLock &lock);
    dint shareCount;
    volatile dint current;
    Mutex mutex;
    ConditionVariable cond;
};

ReadWriteLock::ReadWriteLock(dint share /*= DM_SHARE_READ_COUNT*/) :
    C_D(ReadWriteLock)
{  
    DM_ASSERT(share > 0);
    pdm->shareCount = pdm->current = share;
}  

ReadWriteLock::~ReadWriteLock()
{
    D_D(ReadWriteLock);
}

void ReadWriteLock::lockForRead() 
{
    SingleLock lock(&pdm->mutex);
    pdm->doLockUnlock(1, 1, lock);
}

void ReadWriteLock::unlockFoRead()
{
    SingleLock lock(&pdm->mutex);
    pdm->doLockUnlock(1, 0, lock);
}

void ReadWriteLock::lockForWrite()
{
    SingleLock lock(&pdm->mutex);
    pdm->doLockUnlock(pdm->shareCount, 1, lock);
}

void ReadWriteLock::unlockForWrite()
{
    SingleLock lock(&pdm->mutex);
    pdm->doLockUnlock(pdm->shareCount, 0, lock);
}

void ReadWriteLockPrivate::doLockUnlock(dint delta, dint op, SingleLock &lock) 
{
    if (op)
    {
        while(current < delta)
        {
            cond.wait(lock.getMutex());
        }
        current -= delta;
    }
    else 
    {
        current += delta;
        cond.notifyAll();
    }
}

#else
class ReadWriteLockPrivate
{
public:
    volatile dint read_ref, write_ref;
    Mutex mutex;
    ConditionVariable cond;
};

ReadWriteLock::ReadWriteLock(dint share /*= DM_SHARE_READ_COUNT*/) :
C_D(ReadWriteLock)
{  
    pdm->read_ref = pdm->write_ref = 0;
}  

ReadWriteLock::~ReadWriteLock()
{
    D_D(ReadWriteLock);
}

void ReadWriteLock::lockForRead() 
{
    SingleLock lock(&pdm->mutex);
    ++pdm->read_ref;
    while (pdm->write_ref > 0) 
    {
        pdm->cond.wait(pdm->mutex);
    }
}

void ReadWriteLock::unlockFoRead()
{
    SingleLock lock(&pdm->mutex);
    --pdm->read_ref;
    pdm->cond.notifyAll();
}

void ReadWriteLock::lockForWrite()
{
    SingleLock lock(&pdm->mutex);
    ++pdm->write_ref;
    if (pdm->write_ref + pdm->read_ref > 1)
    {
        pdm->cond.wait(pdm->mutex);
    }
}

void ReadWriteLock::unlockForWrite()
{
    SingleLock lock(&pdm->mutex);
    --pdm->write_ref;
    if (pdm->write_ref == 0)
    {
        pdm->cond.notifyAll();
    }
}

#endif

DM_END_NAMESPACE

