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

#include "dmpoolthread.h"
#include "thread/dmmutex.h"
#include "thread/dmatomic.h"
#include "thread/dmsinglelock.h"

DM_BEGIN_NAMESPACE
DM_BEGIN_NS_THREAD
class PoolThreadPrivate
{
public:
    Mutex mutex;
    volatile long lock;
    ITask* volatile task;
};

PoolThread::PoolThread():
    C_D(PoolThread)
{
    pdm->lock = 0;
    pdm->task = NULL;
}

PoolThread::~PoolThread()
{
    stop();
    while (this->isRunning())
    {
        Thread::sleep(10);
    }
    D_D(PoolThread);
}

void PoolThread::stop()
{
    SingleLock lock(&pdm->mutex);
    pdm->lock = 0;
    if (pdm->task)
        pdm->task->abort();
}

dbool PoolThread::setTask(ITask *task)
{
    SingleLock lock(&pdm->mutex);

    if (pdm->task)
        return false;

    pdm->task = task;
    return true;
}

dbool PoolThread::isRunning() const
{
    return pdm->lock != 2;
}

dbool PoolThread::isIdle() const
{
    SingleLock lock(&pdm->mutex);
    return pdm->task == NULL;
}

void PoolThread::run()
{   
    SingleLock lock(&pdm->mutex);
    //set
    pdm->lock = 1;

    while (pdm->lock == 1) {

        if (pdm->task)
        {
            lock.unlock();

            pdm->task->run();

            lock.lock();
            //Task have done, set task to NULL
            pdm->task = NULL;
            lock.unlock();
        }
        else
        {
            lock.unlock();
            Thread::yieldCurrentThread();
        }
        lock.lock();
    };

	//all done, set lock to 2
	pdm->lock = 2;
    lock.unlock();
}

DM_END_NS_THREAD
DM_END_NAMESPACE

