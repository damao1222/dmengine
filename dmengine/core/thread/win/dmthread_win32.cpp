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

#include "../private/dmthread_p.h"
#include "thread/dmthread.h"
#include "dmlogger.h"

DM_BEGIN_NAMESPACE

void ThreadPrivate::finish(void *arg, dbool needlock /*= true*/)
{
    Thread *thr = reinterpret_cast<Thread *>(arg);
    ThreadPrivate *d = thr->d_func();

    SingleLock lock(needlock ? &d->mutex : NULL);

    d->threadId = 0;
    if (d->handle)
        ::CloseHandle(d->handle);
    d->handle = NULL;
    d->isFinished = true;
    d->wait.set();
}

Thread::Thread():
    C_D(Thread)
{
    pdm->threadId = 0;
    pdm->handle = NULL;
    pdm->isFinished = true;
    pdm->stackSize = 0;
    pdm->priority = GetThreadPriority(GetCurrentThread());
}

Thread::~Thread()
{
    D_D(Thread);
}

dint Thread::start(duint stacksize /*= 0*/)
{
    pdm->isFinished = false;
    if (pdm->stackSize < stacksize)
        pdm->stackSize = stacksize;
    pdm->handle = ::CreateThread(NULL, pdm->stackSize, (LPTHREAD_START_ROUTINE)&ThreadPrivate::staticThread, this, CREATE_SUSPENDED, &pdm->threadId);
    if (pdm->handle == NULL)
    {
        pdm->isFinished = true;
        DM_FATAL("%d creating thread", GetLastError());
        return -1;
    }

    if (ResumeThread(pdm->handle) == -1)
    {
        DM_FATAL("%d resuming thread", GetLastError());
        return -1;
    }
    return 1;
}

dbool Thread::isRunning() const
{
    SingleLock locker(&pdm->mutex);
    return !pdm->isFinished;
}

dbool Thread::isCurrentThread() const
{
    return isCurrentThread(pdm->threadId);
}

ThreadIdentifier Thread::currentId() const
{
    return pdm->threadId;
}

void Thread::terminate()
{
    SingleLock locker(&pdm->mutex);
    if (pdm->isFinished || pdm->handle == NULL)
        return;

    TerminateThread(pdm->handle, 0);
    ThreadPrivate::finish(this, false);
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

#include "dmwinpthreads.h"
dbool Thread::globalInit()
{
    w32thread_init();
	return true;
}

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

dbool Thread::wait(duint msec /*= UINT_MAX*/)
{
    SingleLock lock(&pdm->mutex);
    if (pdm->threadId == GetCurrentThreadId()) {
        DM_LOGW("Thread::wait: Thread tried to wait on itself");
        return false;
    }

    if (pdm->isFinished)
        return true;

    dbool bReturn = true;
    if (pdm->threadId && pdm->handle != NULL)
    {
        // boost priority of thread we are waiting on to same as caller
        dint callee = GetThreadPriority(pdm->handle);
        dint caller = GetThreadPriority(::GetCurrentThread());
        if(caller != THREAD_PRIORITY_ERROR_RETURN && caller > callee)
            SetThreadPriority(pdm->handle, caller);

        lock.unlock();
        bReturn = pdm->wait.waitMSec(msec);
        lock.lock();

        // restore thread priority if thread hasn't exited
        if(callee != THREAD_PRIORITY_ERROR_RETURN && caller > callee && pdm->handle)
            SetThreadPriority(pdm->handle, callee);
    }
    return bReturn;
}

dbool Thread::setPriority(dint iPriority)
{
    dbool bReturn = false;

    SingleLock lock(&pdm->mutex);
    if (pdm->handle)
    {
        bReturn = SetThreadPriority(pdm->handle, iPriority) == TRUE;
    }

    return bReturn;
}

dint Thread::priority() const
{
    SingleLock lock(&pdm->mutex);

    int iReturn = THREAD_PRIORITY_NORMAL;
    if (pdm->handle)
    {
        iReturn = GetThreadPriority(pdm->handle);
    }

    return iReturn;
}

THREADFUNC ThreadPrivate::staticThread(void *data)
{
    Thread* pThread = (Thread*)(data);
        
    pThread->run();

    finish(pThread);

    return 0;
}

DM_END_NAMESPACE

