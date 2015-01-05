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

#include "dmliveobject_p.h"
#include "dmcoreapplication.h"
#include "dmevent.h"
#include "dmutilstring.h"

DM_BEGIN_NAMESPACE

BasicAtomicInt TimerImpl::g_id = DM_BASIC_ATOMIC_INITIALIZER(0);

duint32 TimerImpl::generateId()
{
    g_id.ref();
    return g_id;
}

LiveObjectPrivate::~LiveObjectPrivate()
{
    
}

void LiveObjectPrivate::checkTime(dreal dt, Object *obj)
{
#if 0
    Vector<dint> v;
    for (dint i=0; i<timers.size(); ++i)
    {
        TimerImpl* t = timers.at(i);
        if (t->stop)
        {
            v.append(i);
            continue;
        }
        t->__count += dt;
        if (t->interval <= t->__count)
        {
            TimeEvent *e = new TimeEvent();
            e->id = t->id;
            e->interval = t->interval;
            e->repeat = t->repeat_count;
            CoreApplication::sendEvent(obj, e);
            e->release();

            if (!t->repeat)
            {
                v.append(i);
            }
            else
            {
                ++t->repeat_count;
                t->__count = 0;
            }
            continue;
        }
    }

    //Expired
    for (dint j=0; j<v.size(); ++j)
    {
        dint index = v.at(j);
        TimerImpl* t = timers.at(index);
        timers.removeAt(index);
        delete t;
    }
#endif
    Hash<dint, TimerImpl*>::iterator it = timers.begin();
    while (it != timers.constEnd()) {
        TimerImpl* t = it.value();
        if (t->stop)
        {
            it = timers.erase(it);
            continue;
        }
        t->__count += dt;
        if (t->interval <= t->__count)
        {
            TimeEvent *e = new TimeEvent();
            e->id = t->id;
            e->interval = t->interval;
            e->repeat = t->repeat_count;
            CoreApplication::sendEvent(obj, e);
            e->release();

            if (!t->repeat)
            {
                it = timers.erase(it);
                continue;
            }
            else
            {
                ++t->repeat_count;
                t->__count = 0;
            }
        }
        ++it;
    }
}

dint LiveObjectPrivate::repeatCount(duint32 id) const
{
/*
    for (dint i=0; i<timers.size(); ++i)
    {
        TimerImpl* t = timers.at(i);
        if (t->id == id)
        {
            return t->repeat_count;
        }
    }
    return 0;
    */
    TimerImpl* t = timers.value(id, NULL);
    if (t != NULL)
        return t->repeat_count;
    else
        return 0;
}

DM_END_NAMESPACE
