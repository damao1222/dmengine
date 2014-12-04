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

#include "thread/dmthreadpool.h"
#include "thread/arch/common/dmpoolthread.h"
#include "thread/dmatomicarray.h"
#include "thread/arch/dmthreadpool_arch.h"

DM_BEGIN_NAMESPACE
DM_BEGIN_NS_THREAD
class ThreadPoolPrivate
{
public:
    Threads::RefArray *threadArr;
};

ThreadPool::ThreadPool():
    C_D(ThreadPool)
{

}

ThreadPool::~ThreadPool()
{
	stop();
	wait();
    DM_SAFE_RELEASE(pdm->threadArr);
    D_D(ThreadPool);
}

dbool ThreadPool::initWithThreadSize(dint size)
{
    pdm->threadArr = new Threads::RefArray;
    for (int i=0; i<size; ++i)
    {
        PoolThread *thread = new PoolThread;
        pdm->threadArr->addObject(thread);
        thread->release();

        thread->start();
    }
    return true;
}

dbool ThreadPool::runTask(ITask *task)
{
    DM_ASSERT(pdm->threadArr);
    for (int i=0; i<pdm->threadArr->size(); ++i)
    {
        PoolThread *thread = (PoolThread*)pdm->threadArr->at(i);
        if (thread->isIdle())
        {
            thread->setTask(task);
            return true;
        }
    }

    return false;
}

void ThreadPool::stop()
{
	for (dint i=0; i<pdm->threadArr->size(); ++i)
	{
		PoolThread *thread = (PoolThread*)pdm->threadArr->at(i);
		thread->stop();
	}
}

void ThreadPool::wait()
{
	for (dint i=0; i<pdm->threadArr->size(); ++i)
	{
		PoolThread *thread = (PoolThread*)pdm->threadArr->at(i);
		thread->wait();
	}
}

DM_END_NS_THREAD
DM_END_NAMESPACE

