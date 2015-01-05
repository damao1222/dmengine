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
#include "dmbroadcast.h"
#include "private/dmbroadcast_p.h"
#include "dmbroadcastreceiver.h"
#include "dmlogger.h"
#include "dmevent.h"

DM_BEGIN_NAMESPACE

class BroadcastEvent : public Event
{
public:
    EventType type() const { return eBroadcastEvent; }
    BroadcastFilter filter;
    Variant msg;
};

Hash<UtilString, BroadcastReceiver*>& BroadcastPrivate::instanceHash()
{
    DM_ASSERT(instance);
    return instance->recieverMap;
}

Mutex& BroadcastPrivate::instanceLock()
{
    DM_ASSERT(instance);
    return instance->mutex;
}

BroadcastPrivate* BroadcastPrivate::instance = NULL;

dbool BroadcastPrivate::init(dbool multiThread)
{
    if (BroadcastPrivate::instance == NULL)
    {
        BroadcastPrivate::instance = new BroadcastPrivate;
        BroadcastPrivate::instance->multiThread = multiThread;
        if (multiThread)
        {
            BroadcastPrivate::instance->setWaitEventTimeout(10);
            BroadcastPrivate::instance->start();
        }
    }

    return true;
}

dbool BroadcastPrivate::cleanup()
{
    DM_ASSERT(BroadcastPrivate::instance);
    BroadcastPrivate::instanceHash().clear();
    delete BroadcastPrivate::instance;
    BroadcastPrivate::instance = NULL;

    return true;
}

void BroadcastPrivate::handleEvent(Event *e, EventHandler::Result ret)
{
    if (ret == EventHandler::eOK)
    {
        DM_ASSERT(e->type() == eBroadcastEvent);

        BroadcastEvent *broadcast = (BroadcastEvent*)e;
        sendBroadcastSync(broadcast->filter, broadcast->msg);
    }
}

dbool BroadcastPrivate::sendBroadcast(const UtilString &action, const Variant &msg)
{
    return multiThread ? sendBroadcastAsync(action, msg) : sendBroadcastSync(action, msg);
}
dbool BroadcastPrivate::sendBroadcast(const BroadcastFilter &filter, const Variant &msg)
{
    return multiThread ? sendBroadcastAsync(filter, msg) : sendBroadcastSync(filter, msg);
}

dbool BroadcastPrivate::sendBroadcastSync(const UtilString &action, const Variant &msg)
{
    if (!action.isEmpty())
    {
        mutex.lock();
        Vector<BroadcastReceiver*> recievers = recieverMap.values(action);
        mutex.unlock();

        for (int i=0; i<recievers.size(); ++i)
        {
            ReceiverMessage message;
            message.setAction(action);
            message.setData(msg);
            recievers.at(i)->onReceive(message);
        }
        //return !recievers.isEmpty();
        return true;
    }
    return false;
}

dbool BroadcastPrivate::sendBroadcastSync(const BroadcastFilter &filter, const Variant &msg)
{
    for (dint i=0; i<filter.actionSize(); ++i)
    {
        const UtilString &action = filter.actionAt(i);
        if (!action.isEmpty())
        {
            mutex.lock();
            Vector<BroadcastReceiver*> recievers = recieverMap.values(action);
            mutex.unlock();

            for (int i=0; i<recievers.size(); ++i)
            {
                ReceiverMessage message;
                message.setAction(action);
                message.setData(msg);
                recievers.at(i)->onReceive(message);
            }
            //return !recievers.isEmpty();
            return true;
        }
    }
    return false;
}

dbool BroadcastPrivate::sendBroadcastAsync(const UtilString &action, const Variant &msg)
{
    BroadcastFilter filter;
    filter.addAction(action);
    
    return sendBroadcastAsync(filter, msg);
}

dbool BroadcastPrivate::sendBroadcastAsync(const BroadcastFilter &filter, const Variant &msg)
{
    if (!multiThread)
    {
        DM_LOGW("Broadcast multithread is disable");
        return false;
    }

    if (filter.actionSize() == 0)
    {
        DM_LOGW("filter is empty!");
        return false;
    }

    BroadcastEvent *e = new BroadcastEvent();
    e->filter = filter;
    e->msg = msg;

    acceptEvent(e);

    DM_SAFE_RELEASE(e);

    return true;
}

dbool Broadcast::sendBroadcast(const UtilString &action, const Variant &msg)
{
    DM_ASSERT(BroadcastPrivate::instance);
    return BroadcastPrivate::instance->sendBroadcast(action, msg);
}

dbool Broadcast::sendBroadcast(const BroadcastFilter &filter, const Variant &msg)
{
    DM_ASSERT(BroadcastPrivate::instance);
    return BroadcastPrivate::instance->sendBroadcast(filter, msg);
}

dbool BroadcastPrivate::registReceiver(BroadcastReceiver *receiver)
{
    DM_ASSERT(BroadcastPrivate::instance);

    if (receiver)
    {
        dint ret = 0;
        for (dint i=0; i<receiver->receivefilter().actionSize(); ++i)
        {
            const UtilString &action = receiver->receivefilter().actionAt(i);
            if (!action.isEmpty())
            {
                SingleLock lock(&BroadcastPrivate::instanceLock());
                {
                    Vector<BroadcastReceiver*> l = BroadcastPrivate::instanceHash().values(action);
                    //Found reciever first
                    if (l.indexOf(receiver) == -1)
                    {
                        BroadcastPrivate::instanceHash().insertMulti(action, receiver);
                        DM_LOGI("registReciever: regist a reciever, filter action is %s", action.toCharStr());
                    }
                    else
                        DM_LOGI("registReciever: reciever have been registed");
                    ++ret;
                }
            }
        }
        return ret > 0;
    }
    return false;
}

dbool BroadcastPrivate::unregistReceiver(BroadcastReceiver *reciever)
{
    DM_ASSERT(BroadcastPrivate::instance);
    dint ret = 0;
    if (reciever)
    {
        for (dint i=0; i<reciever->receivefilter().actionSize(); ++i)
        {
            const UtilString &action = reciever->receivefilter().actionAt(i);
            if (!action.isEmpty())
            {
                SingleLock lock(&BroadcastPrivate::instanceLock());
                {
                    Hash<UtilString, BroadcastReceiver*>::Iterator it = BroadcastPrivate::instanceHash().begin();
                    while (it != BroadcastPrivate::instanceHash().constEnd())
                    {
                        if (action == it.key() && reciever == it.value())
                        {
                            BroadcastPrivate::instanceHash().erase(it);
                            ++ret;
                            break;
                        }
                        else
                            ++it;
                    }
                }
                return true;
            }
        }
    }
    return ret > 0;
}

DM_END_NAMESPACE
