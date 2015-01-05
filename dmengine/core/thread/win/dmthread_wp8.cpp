/*
   Copyright (C) 2012-2013 Xiongfa Li
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

#include "thread/dmthread.h"
#include "thread/dmsinglelock.h"
#include <Windows.h>
#include "thread/dmatomic.h"
DM_BEGIN_NAMESPACE
class ThreadPrivate
{
public:
    volatile long lock;
    IAsyncAction action;
};

Thread::Thread():
    C_D(Thread)
{
    pdm->lock = 0;
    pdm->action = nullptr;
}

Thread::~Thread()
{
    D_D(Thread);
}

dint Thread::start(duint stacksize /*= 0*/)
{
    auto workItemHandler = ref new WorkItemHandler([=](IAsyncAction^)
    {
        // Run the user callback.
        try
        {
            do {
                SingleLock lock(m_mutex);
                if (m_task)
                {
                    lock.unlock();
                    m_task->run();
                }
                else
                {
                    lock.unlock();
                    ::Sleep(0);
                }
            } while (m_lock == 0);
        }
        catch (...) { }
    }, CallbackContext::Any);

      = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::Normal, WorkItemOptions::TimeSliced);
}

void Thread::stop()
{
    DM_ASSERT_X(false, "Cannot stop thread, this is not implemented");
    ::CloseHandle(pdm->handle);
}

dbool Thread::isRunning() const
{
    return pdm->lock != 0;
}

dbool Thread::isCurrentThread() const
{
    return isCurrentThread(pdm->threadId);
}

ThreadIdentifier Thread::currentId() const
{
    return pdm->threadId;
}

ThreadIdentifier Thread::currentThreadId()
{
    return ::GetCurrentThreadId();
}

#ifdef DM_OS_WP8
_Use_decl_annotations_ VOID WINAPI Sleep(DWORD dwMilliseconds)
{
    static HANDLE singletonEvent = nullptr;

    HANDLE sleepEvent = singletonEvent;

    // Demand create the event.
    if (!sleepEvent)
    {
        sleepEvent = CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);

        if (!sleepEvent)
            return;

        HANDLE previousEvent = InterlockedCompareExchangePointerRelease(&singletonEvent, sleepEvent, nullptr);
            
        if (previousEvent)
        {
            // Back out if multiple threads try to demand create at the same time.
            CloseHandle(sleepEvent);
            sleepEvent = previousEvent;
        }
    }

    // Emulate sleep by waiting with timeout on an event that is never signalled.
    WaitForSingleObjectEx(sleepEvent, dwMilliseconds, false);
}
#endif

void Thread::sleep(dint ms)
{
    Sleep(ms);
}

dbool Thread::isCurrentThread(const ThreadIdentifier id)
{
    return id == ::GetCurrentThreadId();
}

void Thread::yieldCurrentThread()
{
    ::Sleep(0);
}

DM_END_NAMESPACE
