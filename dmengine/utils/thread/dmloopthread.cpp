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

#include "dmloopthread.h"
#include "thread/dmsinglelock.h"
#include "dmlogger.h"
#include "dmeventhandler.h"
#include "dmevent.h"

//platform implement
DM_BEGIN_NAMESPACE
class LoopThreadPrivate
{
public:
    LoopThreadPrivate();

    Mutex lock;
    volatile dbool stop;
    volatile duint timeoutInMilliSeconds;
    volatile duint eventNumber;
    EventHandler handler;
};

LoopThreadPrivate::LoopThreadPrivate():
    stop(false),
    timeoutInMilliSeconds(0),
    eventNumber(UINT_MAX)
{
}

LoopThread::LoopThread() :
    C_D(LoopThread)
{
    //start handler
    pdm->handler.start();
}

LoopThread::~LoopThread()
{
    D_D(LoopThread);
}

dbool LoopThread::acceptEvent(Event *e, dint priority /*= 0*/)
{
    if (!isLooping())
        return false;

    return pdm->handler.put(e, priority);
}

dbool LoopThread::canAcceptEvent() const
{
    return !pdm->handler.isFull();
}

dbool LoopThread::hasEvent() const
{
    return pdm->handler.hasEvent();
}

dint LoopThread::eventCount() const
{
    return pdm->handler.eventCount();
}

void LoopThread::setWaitEventTimeout(duint iTimeoutInMilliSeconds)
{
    SingleLock locker(&pdm->lock);
    pdm->timeoutInMilliSeconds = iTimeoutInMilliSeconds;
}

void LoopThread::setHandleEventNumber(duint number)
{
    SingleLock locker(&pdm->lock);
    pdm->eventNumber = number;
}

void LoopThread::stopLoopAndFinish()
{
    SingleLock locker(&pdm->lock);
    pdm->handler.abort();
    pdm->stop = true;
}

dbool LoopThread::isLooping() const
{
    SingleLock locker(&pdm->lock);
    return !pdm->stop;
}

dbool LoopThread::isLoopingUnlock() const
{
    return !pdm->stop;
}

Mutex* LoopThread::getLock() const
{
	return &pdm->lock;
}

void LoopThread::run()
{
    {
        SingleLock locker(&pdm->lock);
    
        if (pdm->stop)
        {
            DM_LOGW("LoopThread is stopped.this is error state!");
            return ;
        }

        pdm->stop = false;
    }

    onStart();

    //pdm->lock.lock();
    while (!pdm->stop) 
    {
        //pdm->lock.unlock();
        dint count = pdm->eventNumber;
        Event *e;
        while (1)
        {
            prepareHandleEvent();
            EventHandler::Result ret = pdm->handler.getAndRetain(&e, pdm->timeoutInMilliSeconds);
            handleEvent(e, ret);
            //handler get will retain ref, release here.
            DM_SAFE_RELEASE(e);

            if (ret != EventHandler::eOK || 
                (pdm->eventNumber != UINT_MAX && --count == 0))
                break;

            if (!pdm->timeoutInMilliSeconds)
                Thread::yieldCurrentThread();
        };

        process();

        //如果没有等待消息，则让其他线程执行
        if (!pdm->timeoutInMilliSeconds)
            Thread::yieldCurrentThread();
        //pdm->lock.lock();
    }

    pdm->handler.stop();
    //pdm->lock.unlock();

    onEnd();
}
DM_END_NAMESPACE
