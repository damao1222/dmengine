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
#include "dmeventhandler.h"
#include "dmevent.h"
#include "dmvector.h"
#include "thread/dmsinglelock.h"
#include "thread/dmcondition.h"
#include "dmlogger.h"

DM_BEGIN_NAMESPACE

class EventItem
{
public:
    EventItem() : 
        ev(NULL),
        prio(0)
    {
    }

    EventItem(Event *event, dint priority) : 
        ev(event),
        prio(priority)
    {
        if (ev)
            ev->retain();    
    }

    EventItem(const EventItem &item) : 
        ev(item.ev),
        prio(item.prio)
    {
        if (ev)
            ev->retain();    
    }

    ~EventItem()
    {
        DM_SAFE_RELEASE(ev);
    }

    EventItem& operator=(const EventItem& item)
    {
        DM_SAFE_RELEASE(ev);
        if(item.ev)
        {
            ev = item.ev;
            ev->retain();
        }
        prio = item.prio;
        return *this;
    }

    Event *ev;
    dint prio;
};

class EventHandlerPrivate
{
public:
    EventHandlerPrivate();

    Vector<EventItem> list;
    Mutex mutex;
    WaitCondition wait;
    volatile dbool inited;
    volatile dbool requestAbort;
    dint maxSize;
};

EventHandlerPrivate::EventHandlerPrivate():
    wait(true),
    inited(false),
    requestAbort(false),
    maxSize(MAX_HANDLE_EVENT_SIZE)
{}

EventHandler::EventHandler():
    C_D(EventHandler)
{
}

EventHandler::~EventHandler()
{
    clean();
    D_D(EventHandler);
}

void EventHandler::start(dint size /*= MAX_HANDLE_EVENT_SIZE*/)
{
    SingleLock lock(&pdm->mutex);
    pdm->inited = true;
    pdm->maxSize = size;
}

void EventHandler::stop()
{
    SingleLock lock(&pdm->mutex);
    pdm->inited = false;
    pdm->list.clear();
    pdm->requestAbort = false;
}

void EventHandler::abort()
{
    SingleLock lock(&pdm->mutex);
    pdm->requestAbort = true;
    pdm->wait.set(); // inform waiter for abort action
}

void EventHandler::clean()
{
    SingleLock lock(&pdm->mutex);
    pdm->list.clear();
}

dbool EventHandler::put(Event *pEvent, dint priority /*= 0*/)
{
    SingleLock lock(&pdm->mutex);

    if (!pEvent)
    {
        DM_LOGW("Invalid event");
        return false;
    }

    if (!pdm->inited)
    {
        DM_LOGW("EventHandler didn't initialed, call start()");
        //DM_SAFE_RELEASE(pEvent);
        return false;
    }

    if (pdm->list.size() == pdm->maxSize)
    {
        DM_LOGW("EventHandler buffer is full");
        //DM_SAFE_RELEASE(pEvent);
        return false;
    }

    Vector<EventItem>::iterator it = pdm->list.begin();
    while(it != pdm->list.constEnd())
    {
        if(priority <= it->prio)
            break;
        it++;
    }

    pdm->list.insert(it, EventItem(pEvent, priority));

    //DM_SAFE_RELEASE(pEvent);
    pdm->wait.set();

    return true;
}

EventHandler::Result EventHandler::getAndRetain(Event** pEvent, duint iTimeoutInMilliSeconds)
{
    SingleLock lock(&pdm->mutex);

    if (!pEvent)
    {
        DM_LOGW("Invalid param");
        return eInvalidEvent;
    }

    *pEvent = NULL;

    if (!pdm->inited)
    {
        DM_LOGW("EventHandler didn't initialed, call start()");
        return eUninitialized;
    }

    Result ret = eTimeout;
    while (!pdm->requestAbort)
    {
        if (!pdm->list.isEmpty())
        {
            *pEvent = pdm->list.last().ev;
            (*pEvent)->retain();
            pdm->list.removeLast();
            ret = eOK;
            break;
        }
        else if (!iTimeoutInMilliSeconds)
        {
            ret = eTimeout;
            break;
        }
        else
        {
            pdm->wait.reset();
            lock.unlock();
            // wait for a new message
            if (!pdm->wait.waitMSec(iTimeoutInMilliSeconds))
                return eTimeout;

            lock.lock();
        }
    }

    if (pdm->requestAbort) 
        return eAbort;

    return ret;
}

dbool EventHandler::isFull() const
{
    SingleLock lock(&pdm->mutex);
    return pdm->list.size() == pdm->maxSize;
}

dbool EventHandler::hasEvent() const
{
    SingleLock lock(&pdm->mutex);
    return !pdm->list.isEmpty();
}

dint EventHandler::eventCount() const
{
    SingleLock lock(&pdm->mutex);
    return pdm->list.size();
}

Mutex& EventHandler::mutex()
{
    return pdm->mutex;
}

DM_END_NAMESPACE


