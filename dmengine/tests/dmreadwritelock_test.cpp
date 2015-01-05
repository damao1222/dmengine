/*
   Copyright (C) 2012-2014 Xiongfa Li
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

#include "dmreadwritelock_test.h"
#include "thread/dmreadwritelock.h"
#include "thread/dmthread.h"
#include "dmlogger.h"

DM_BEGIN_NAMESPACE

class TestThread : public Thread
{
public:
    TestThread() : stop(false) {}
    volatile dbool stop;

protected:
    void run();
};

#define DMREADWRITELOCK_TEST_THREAD_SIZE 8
static volatile dint g_value = 0;
static ReadWriteLock *g_rwLock = NULL;
static TestThread* g_thread_pool[DMREADWRITELOCK_TEST_THREAD_SIZE];

void TestThread::run()
{
    while (!stop)
    {
        g_rwLock->lockForRead();
        DM_LOGI("read %d current value is %d", Thread::currentThreadId(), g_value);
        //Thread::sleep(10);
        g_rwLock->unlockFoRead();
        //Thread::sleep(10);
    }
}


dbool ReadWriteLockTest::init()
{
    g_rwLock = new ReadWriteLock;
    for (dint i = 0; i<DMREADWRITELOCK_TEST_THREAD_SIZE; ++i)
    {
        g_thread_pool[i] = new TestThread;
    }
    return true;
}

void ReadWriteLockTest::cleanup()
{
    for (dint i = 0; i<DMREADWRITELOCK_TEST_THREAD_SIZE; ++i)
    {
        g_thread_pool[i]->stop = true;
        if (g_thread_pool[i]->isRunning())
            g_thread_pool[i]->wait();
    }
}

void ReadWriteLockTest::run()
{
    for (dint i = 0; i<DMREADWRITELOCK_TEST_THREAD_SIZE; ++i)
    {
        g_thread_pool[i]->start();
    }

    while (1)
    {
        g_rwLock->lockForWrite();
        g_value++;
        Thread::sleep(10);
        g_rwLock->unlockForWrite();
        DM_LOGI("write current value is %d", g_value);
        
    }
}
DM_END_NAMESPACE

