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

#include "dmliveobject.h"
#include "private/dmliveobject_p.h"
#include "dmapplication.h"
#include "dmlogger.h"

DM_BEGIN_NAMESPACE
LiveObject::LiveObject():
    C_D(LiveObject)
{
}

LiveObject::LiveObject(LiveObjectPrivate *p):
    pdm(p)
{

}

LiveObject::~LiveObject()
{
    /*
    for (dint i=0; i<pdm->timers.size(); ++i)
    {
        delete pdm->timers.at(i);
    }
    */
    TimerImpl *p = NULL;
    DM_FOREACH_HASH(p, pdm->timers, dint, TimerImpl*)
    {
        DM_SAFE_DELETE(p);
    }
    pdm->timers.clear();
    D_D(LiveObject);
}

void LiveObject::frameMove(dreal dt)
{
    if (!pdm->stop)
    {
        //pause
        if (pdm->pausetime > 0)
        {
            pdm->lastpause += dt;
            if (pdm->lastpause < pdm->pausetime)
                return ;
            else
                //We done
                pdm->pausetime = 0;
        }

        //timer
        if (pdm->timers.size() > 0)
        {
            pdm->checkTime(dt, this);
        }

        onframeMove(dt);
    }
}

dbool LiveObject::event(Event *e)
{
    switch (e->type())
    {
    case eTimeEvent:
        onTimeEvent(static_cast<TimeEvent*>(e));
        return true;
    default:
        break;
    }

    return Object::event(e);
}

dbool LiveObject::isDeactivated() const
{
    return pdm->__remove;
}

dbool LiveObject::isAcceptInput() const
{
    return pdm->acceptInput;
}

void LiveObject::start()
{   if (pdm->stop) 
    { 
        pdm->stop = false; 
        pdm->lastpause = 0; 
        pdm->pausetime = 0; 
        onStart(); 
    } 
}

void LiveObject::stop()
{ if (!pdm->stop) { pdm->stop = true; onStop(); } }

void LiveObject::pause(float timeout)
{ pdm->lastpause = 0; pdm->pausetime = timeout; }

dbool LiveObject::isStopped() const
{ return pdm->stop; }

dbool LiveObject::isRunning() const
{ return !pdm->stop && !isPaused();  }

dbool LiveObject::isPaused() const
{ return pdm->pausetime > 0 && pdm->lastpause < pdm->pausetime;  }

duint32 LiveObject::startTimer(dreal interval, dbool repeat/* = false*/)
{
    if (!isActivated())
    {
        DM_LOGW("Object not activate, you need to call activate() or g_application.activateObject(this) at first");
    }
    //stop first
    pdm->stop = true;
    duint32 id = TimerImpl::generateId();
    TimerImpl* t = new TimerImpl(interval, id, repeat);
    //go
    t->stop = false;
    //pdm->timers.append(t);
    pdm->timers.insert(id, t);
    pdm->stop = false;
    return id;
}

dbool LiveObject::resetTimer(duint32 id)
{
    /*
    if (pdm->timers.size() == 0) return false;
    for (dint i=0; i<pdm->timers.size(); ++i)
    {
        if (pdm->timers.at(i)->id == id)
        {
            pdm->timers.at(i)->__count = 0;
            pdm->timers.at(i)->repeat_count = 0;
            return true;
        }
    }
    */
    TimerImpl* t = pdm->timers.value(id, NULL);
    if (t != NULL)
    {
        t->__count = 0;
        t->repeat_count = 0;
        return true;
    }

    DM_LOGI("Timer not found! Maybe expire");
    return false;
}

void LiveObject::stopAllTimer()
{
    /*
    if (pdm->timers.size() == 0) return ;
    for (dint i=0; i<pdm->timers.size(); ++i)
    {
        pdm->timers.at(i)->stop = true;
    }
    */
    TimerImpl *t = NULL;
    DM_FOREACH_HASH(t, pdm->timers, dint, TimerImpl*)
    {
        t->stop = true;
    }
}

dbool LiveObject::stopTimer(duint32 id)
{
    /*
    if (pdm->timers.size() == 0) return false;

    for (dint i=0; i<pdm->timers.size(); ++i)
    {
        if (pdm->timers.at(i)->id == id)
        {
            pdm->timers.at(i)->stop = true;
            return true;
        }
    }
    */
    TimerImpl* t = pdm->timers.value(id, NULL);
    if (t != NULL)
    {
        t->stop = true;
        return true;
    }

    DM_LOGW("Timer not found!");
    return false;
}

dint LiveObject::timerRepeatCount(duint32 id) const
{
    return pdm->repeatCount(id);
}

dbool LiveObject::isTimerRunning(duint32 id) const
{
    /*
    for (dint i=0; i<pdm->timers.size(); ++i)
    {
        TimerImpl* t = pdm->timers.at(i);
        if (t->id == id)
        {
            return !t->stop;
        }
    }
    */
    TimerImpl* t = pdm->timers.value(id, NULL);
    if (t != NULL)
    {
        return !t->stop;
    }
    return false;
}

dreal LiveObject::remainTime(duint32 id) const
{
    /*
    for (dint i=0; i<pdm->timers.size(); ++i)
    {
        TimerImpl* t = pdm->timers.at(i);
        if (t->id == id)
        {
            return t->interval - t->__count;
        }
    }
    */
    TimerImpl* t = pdm->timers.value(id, NULL);
    if (t != NULL)
    {
        return (t->interval - t->__count);
    }
    return -1;
}

void LiveObject::activate()
{
    g_application.activateObject(this);
}

void LiveObject::deactivate()
{
    g_application.deactivateObject(this);
}

dbool LiveObject::isActivated() const
{
    return !pdm->__remove;
}

DM_END_NAMESPACE
