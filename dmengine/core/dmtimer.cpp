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

#include "dmtimer.h"
#include "dmapplication.h"

DM_BEGIN_NAMESPACE

TimerPrivate::TimerPrivate():
    param(NULL), func(NULL), proxy(NULL), interval(0), repeat(false)
{}

TimerProxy::TimerProxy(Timer *t) : 
    timer(t), 
    interval(0),
    time_count(0),
    id(0),
    repeat_count(0),
    pausetime(0), 
    lastpause(0), 
    repeat(false),
    stop(false), 
    remove_mark(true)
{

}

TimerProxy::TimerProxy(Timer *t, dreal fInv, duint32 nId, dbool bRepeat) : 
    timer(t), 
    interval(fInv),
    time_count(0),
    id(nId),
    repeat_count(0),
    pausetime(0), 
    lastpause(0), 
    repeat(bRepeat),
    stop(false), 
    remove_mark(true)
{

}

void TimerProxy::checkTime(dreal dt)
{
    if (!timer) 
    {
        remove_mark = true;
        return ;
    }

    if (stop)
    {
        return ;
    }

    time_count += dt;
    if (interval <= time_count)
    {
        TimeEvent *e = new TimeEvent();
        e->id = id;
        e->interval = interval;
        e->repeat = repeat_count;
        if (!repeat)
        {
            remove_mark = true;
            timer->d_func()->proxy = NULL;
        }
        timer->d_func()->onTimeout(e);
        e->release();

        if (repeat)
        {
            ++repeat_count;
            time_count = 0;
        }
        else
        {
            timer = NULL;
        }
    }
}

Timer::Timer() :
    C_D(Timer)
{
}

Timer::Timer(TimerCallbackFunc func, void *param/* = NULL*/):
    C_D(Timer)
{
    pdm->param = param;
    pdm->func = func;
}

Timer::~Timer()
{
	stop();
	D_D(Timer);
}

void Timer::setInterval(dreal interval)
{
    pdm->interval = interval;
}

void Timer::setRepeat(dbool flag)
{
    pdm->repeat = flag;
}

dint Timer::repeatCount() const
{
    return pdm->proxy ? pdm->proxy->repeat_count : 0;
}

dint Timer::id() const
{
    return pdm->proxy ? pdm->proxy->id : 0;
}

dbool Timer::isTiming() const
{
    return pdm->proxy ? !pdm->proxy->remove_mark : false;
}

dreal Timer::remainTime() const
{
    return pdm->proxy ? pdm->proxy->interval - pdm->proxy->time_count : 0;
}

void Timer::start(dreal interval, dbool repeat/* = false*/)
{
    setInterval(interval);
    setRepeat(repeat);
    start();
}

void Timer::start()
{
    stop();
    pdm->proxy = new TimerProxy(this, pdm->interval, TimerProxy::generateId(), pdm->repeat);
    dmApp.activateTimer(*this);
}

dbool Timer::reset()
{
    if (pdm->proxy)
    {
        pdm->proxy->time_count = 0;
        pdm->proxy->repeat_count = 0;
        return true;
    }
    return false;
}

void Timer::stop()
{
    if (pdm->proxy)
    {
        pdm->proxy->remove_mark = true;
        pdm->proxy->timer = NULL;
        DM_SAFE_RELEASE(pdm->proxy);
    }
	dmApp.deactivateTimer(*this);
}

void TimerPrivate::onTimeout(TimeEvent *event)
{
    if (func)
        (*func)(param, event);
}

DM_END_NAMESPACE

