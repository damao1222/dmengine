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

DM_BEGIN_NAMESPACE
class TimerPrivate
{
public:
    TimerPrivate():target(NULL), func(NULL), cb(NULL), interval(0), id(0), repeat(false), isRunning(false) {}
    void* target;
    TimerCallbackFunc func;
    TimeoutCallback *cb;
    dreal interval;
    duint32 id;
    dbool repeat;
    dbool isRunning;
};

Timer::Timer() :
    C_D(Timer)
{
}

Timer::Timer(TimerCallbackFunc func, void *target/* = NULL*/):
    C_D(Timer)
{
    pdm->target = target;
    pdm->func = func;
}

Timer::Timer(TimeoutCallback *cb):
    C_D(Timer)
{
    pdm->cb = cb;
}

Timer::~Timer()
{
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
    return LiveObject::timerRepeatCount(pdm->id);
}

dint Timer::id() const
{
    return pdm->id;
}

dbool Timer::isTiming() const
{
    return pdm->isRunning;
}

void Timer::start(dreal interval, dbool repeat/* = false*/)
{
    pdm->interval = interval;
    pdm->repeat = repeat;
    start();
}

void Timer::start()
{
    stopAllTimer();
    activate();
    pdm->id = LiveObject::startTimer(pdm->interval, pdm->repeat);
    pdm->isRunning = true;
}

dbool Timer::reset()
{
    return resetTimer(pdm->id);
}

void Timer::stop()
{
    //Don't stop twice
    if (pdm->isRunning) 
    {
        LiveObject::stopTimer(pdm->id);
        LiveObject::stop();
        pdm->isRunning = false;
        deactivate();
    }
}

void Timer::onTimeEvent(TimeEvent *event)
{
    /*
    if (pdm->target && pdm->func == NULL)
        sendEvent(pdm->target, event);
    */
    emitTimeout();

    if (pdm->func)
        (*(pdm->func))(pdm->target, event);
    if (pdm->cb)
        pdm->cb->onTimeout(this, event);

    if (!pdm->repeat)
    {
        pdm->isRunning = false;
        deactivate();
    }
}
DM_END_NAMESPACE

