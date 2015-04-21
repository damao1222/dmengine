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

#ifndef DMLIVEOBJECT_P_H
#define DMLIVEOBJECT_P_H
#include "dmnamespace.h"
#include "dmatomicint.h"
#include "dmrefptr.h"
#include "dmevent.h"

DM_BEGIN_NAMESPACE
class Timer;
class TimerPrivate;
class Application;
class TimerProxy : public RefPtr<TimerProxy>
{
    friend class Timer;
    friend class TimerPrivate;
    friend class Application;
public:
    TimerProxy(Timer *timer);
    TimerProxy(Timer *timer, dreal fInv, duint32 nId, dbool bRepeat);
    void checkTime(dreal dt);

    static duint generateId();
    static BasicAtomicInt g_id;
private:
    Timer *timer;

    dreal interval;
    dreal time_count;
    duint32 id;
    dint repeat_count;

    duint pausetime;
    duint lastpause;

    duint8 repeat : 1;
    //WARNING: remove_mark is only used by Application, Don't change it's value any way!
    duint8 remove_mark : 1;
};

typedef void (*TimerCallbackFunc)(void *obj, TimeEvent *event);

class TimerPrivate
{
public:
    TimerPrivate();
    void onTimeout(TimeEvent *e);
    void* param;
    TimerCallbackFunc func;
    TimerProxy *proxy;
    dreal interval;
    dbool repeat;
};

class LiveObjectPrivate
{
public:
    LiveObjectPrivate() : acceptInput(false) {}
    virtual ~LiveObjectPrivate();

    duint8 acceptInput : 1;
    duint8 unused : 7;
};

DM_END_NAMESPACE
#endif // DMLIVEOBJECT_P_H
