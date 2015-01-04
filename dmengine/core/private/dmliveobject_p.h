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
#include "dmhash.h"
#include "dmatomicint.h"
DM_BEGIN_NAMESPACE
class Object;
class LiveObjectPrivate;
class UtilString;
class TimerImpl
{
    friend class LiveObject;
    friend class LiveObjectPrivate;
public:
    TimerImpl(): interval(0), __count(0), id(0), repeat(false), repeat_count(0), isRunning(false), stop(false){  }
    TimerImpl(dreal fInv, duint32 nId, dbool bRepeat): interval(fInv), __count(0), id(nId), repeat(bRepeat), repeat_count(0), isRunning(false), stop(false){  }
    static duint32 generateId();

private:
    dreal interval;
    dreal __count;
    duint32 id;
    dbool repeat;
    dint repeat_count;
    dbool isRunning;
    dbool stop;
    static BasicAtomicInt g_id;
};

class LiveObjectPrivate
{
public:
    LiveObjectPrivate() : stop(true), pausetime(0), lastpause(0), __remove(true), acceptInput(false) {}
    virtual ~LiveObjectPrivate();

    void checkTime(dreal dt, Object *obj);
    dint repeatCount(duint32 id) const;

    Hash<dint, TimerImpl*> timers;
    dbool stop;
    dreal pausetime;
    dreal lastpause;

    //WARNING: __remove is only used by Application, Don't change it's value any way!
    duint8 __remove : 1;
    duint8 acceptInput : 1;
    duint8 unused : 6;
    /*
    duint __remove : 1;
    duint acceptInput : 1;
    duint pendTimer : 1;
    duint blockSig : 1;
    duint wasDeleted : 1;
    duint ownObjectName : 1;
    duint sendChildEvents : 1;
    duint receiveChildEvents : 1;
    duint inThreadChangeEvent : 1;
    duint hasGuards : 1; 
    duint unused : 22;
    */
};

DM_END_NAMESPACE
#endif // DMLIVEOBJECT_P_H
