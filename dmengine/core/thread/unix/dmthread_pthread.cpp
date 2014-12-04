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

#include "pthread.h"
#include "../private/dmthread_p.h"
#include "thread/dmthread.h"
#include "dmlogger.h"
#include "dmmath.h"
#include <unistd.h>

DM_BEGIN_NAMESPACE

void ThreadPrivate::finish(void *arg, dbool needlock /*= true*/)
{
    Thread *thr = reinterpret_cast<Thread *>(arg);
    ThreadPrivate *d = thr->d_func();

    SingleLock lock(needlock ? &d->mutex : NULL);

    d->threadId = 0;
    d->isFinished = true;
    d->wait.set();
}

dbool Thread::globalInit()
{
    return true;
}

Thread::Thread():
    C_D(Thread)
{
    pdm->threadId = 0;
    pdm->isFinished = true;
    pdm->stackSize = 0;
    pdm->priority = PTHREAD_INHERIT_SCHED;
}

Thread::~Thread()
{
    D_D(Thread);
}

dint Thread::start(duint stacksize /*= 0*/)
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    if (pdm->stackSize < stacksize)
        pdm->stackSize = stacksize;
#if !defined(DM_OS_ANDROID) // http://code.google.com/p/android/issues/detail?id=7808
    if (stacksize > PTHREAD_STACK_MIN)
        pthread_attr_setstacksize(&attr, pdm->stackSize);
#endif
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pdm->isFinished = false;
    if (pthread_create(&pdm->threadId, &attr, (void*(*)(void*))ThreadPrivate::staticThread, this) != 0)
    {
        pdm->isFinished = true;
        DM_FATAL("creating thread");
        return -1;
    }
    pthread_attr_destroy(&attr);
    return 1;
}

dbool Thread::isRunning() const
{
    SingleLock l(&pdm->mutex);
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

ThreadIdentifier Thread::currentThreadId()
{
    return pthread_self();
}

dbool Thread::isCurrentThread(const ThreadIdentifier id)
{
    return pthread_equal(pthread_self(), id);
}

void Thread::sleep(dint ms)
{
    ::usleep(1000*ms);
}

void Thread::yieldCurrentThread()
{
    sched_yield();
}

void Thread::terminate()
{
	//FIXME: pthread terminate is invalid
	DM_ASSERT(false);
    SingleLock locker(&pdm->mutex);

    if (pdm->isFinished || pdm->threadId)
        return;

    //dint code = pthread_cancel(pdm->threadId);
    dint code = 0;
    if (code) {
        DM_LOGW("Thread termination error: %d", code);
    } else {
        ThreadPrivate::finish(this, false);
    }
}

dbool Thread::wait(duint msec /*= UINT_MAX*/)
{
    dbool bReturn = true;

    SingleLock lock(&pdm->mutex);
    if (pdm->threadId == pthread_self()) {
        DM_LOGW("Thread::wait: Thread tried to wait on itself");
        return false;
    }

    if (pdm->isFinished)
        return true;

    return pdm->wait.waitMSec(msec);
}

enum Priority {
        IdlePriority,

        LowestPriority,
        LowPriority,
        NormalPriority,
        HighPriority,
        HighestPriority,

        TimeCriticalPriority,

        InheritPriority
};
static dbool CalculateUnixPriority(int priority, int *sched_policy, int *sched_priority)
{
#ifdef SCHED_IDLE
    if (priority == IdlePriority) {
        *sched_policy = SCHED_IDLE;
        *sched_priority = 0;
        return true;
    }
    const int lowestPriority = LowestPriority;
#else
    const int lowestPriority = IdlePriority;
#endif
    const int highestPriority = TimeCriticalPriority;

    int prio_min = sched_get_priority_min(*sched_policy);
    int prio_max = sched_get_priority_max(*sched_policy);
    if (prio_min == -1 || prio_max == -1)
        return false;

    int prio;
    // crudely scale our priority enum values to the prio_min/prio_max
    prio = ((priority - lowestPriority) * (prio_max - prio_min) / highestPriority) + prio_min;
    prio = Max(prio_min, Min(prio_max, prio));

    *sched_priority = prio;
    return true;
}

dbool Thread::setPriority(dint priority)
{
    SingleLock locker(&pdm->mutex);
    if (pdm->isFinished) {
        DM_LOGW("Thread::setPriority: Cannot set priority, thread is not running");
        return false;
    }

    pdm->priority = priority;

    // copied from start() with a few modifications:

    int sched_policy;
    sched_param param;

    if (pthread_getschedparam(pdm->threadId, &sched_policy, &param) != 0) {
        // failed to get the scheduling policy, don't bother setting
        // the priority
        DM_LOGW("Thread::setPriority: Cannot get scheduler parameters");
        return false;
    }

    int prio;
    if (!CalculateUnixPriority(priority, &sched_policy, &prio)) {
        // failed to get the scheduling parameters, don't
        // bother setting the priority
        DM_LOGW("Thread::setPriority: Cannot determine scheduler priority range");
        return false;
    }

    param.sched_priority = prio;
    int status = pthread_setschedparam(pdm->threadId, sched_policy, &param);

# ifdef SCHED_IDLE
    // were we trying to set to idle priority and failed?
    if (status == -1 && sched_policy == SCHED_IDLE && errno == EINVAL) {
        // reset to lowest priority possible
        pthread_getschedparam(d->thread_id, &sched_policy, &param);
        param.sched_priority = sched_get_priority_min(sched_policy);
        pthread_setschedparam(d->thread_id, sched_policy, &param);
    }
# else
    DM_UNUSED(status);
# endif // SCHED_IDLE
    return true;
}

dint Thread::priority() const
{
    SingleLock locker(&pdm->mutex);
    return pdm->priority;
}

THREADFUNC ThreadPrivate::staticThread(void *data)
{
    Thread* pThread = (Thread*)(data);
        
    pThread->run();

    finish(pThread);
    return 0;
}

DM_END_NAMESPACE

