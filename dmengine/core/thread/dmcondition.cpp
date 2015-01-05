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

#include "dmcondition.h"
#include "dmvector.h"
#include <limits>
#include <stdarg.h>

DM_BEGIN_NAMESPACE
class WaitConditionPrivate
{
public:
    WaitConditionPrivate(dbool manual , dbool signaled_);
    dbool manualReset;
    volatile dbool signaled;
    duint numWaits;

    Mutex groupListMutex; // lock for the groups list
    Vector<ConditionGroup*> * groups;

    /**
    * To satisfy the TightConditionVariable requirements and allow the 
    *  predicate being monitored to include both the signaled and interrupted
    *  states.
    */
    ConditionVariable actualCv;
    TightConditionVariable<volatile dbool&> condVar;
    Mutex mutex;

    // helper for the two wait methods
    inline dbool prepReturn();
};

class ConditionGroupPrivate
{
public:
    ConditionGroupPrivate();
    Vector<WaitCondition*> events;
    WaitCondition* signaled;
    ConditionVariable actualCv;
    TightConditionVariable<WaitCondition*&> condVar;
    Mutex mutex;

    duint numWaits;

    // This is ONLY called from WaitCondition::Set.
    inline void set(WaitCondition* child) 
    { 
        SingleLock l(&mutex); 
        signaled = child; 
        condVar.notifyAll(); 
    }
};

WaitConditionPrivate::WaitConditionPrivate(dbool manual , dbool signaled_):
    manualReset(manual), signaled(signaled_), numWaits(0), groups(NULL), condVar(actualCv, signaled)
{

}

dbool WaitConditionPrivate::prepReturn() 
{ 
    dbool ret = signaled; 
    if (!manualReset && numWaits == 0) 
        signaled = false; 
    return ret; 
}

WaitCondition::WaitCondition(dbool manual /*= false*/, dbool signaled_ /*= false*/) : 
    C_D2(WaitCondition, manual, signaled_)
{ 

}

WaitCondition::~WaitCondition()
{
    D_D(WaitCondition);
}

void WaitCondition::reset() 
{ 
    SingleLock lock(&pdm->mutex); 
    pdm->signaled = false; 
}

void WaitCondition::set()
{
    {
        SingleLock slock(&pdm->mutex);
        pdm->signaled = true; 
    }

    pdm->condVar.notifyAll();

    SingleLock l(&pdm->groupListMutex);
    if (pdm->groups)
    {
        for (dint i=0; i<pdm->groups->size(); ++i)
            pdm->groups->at(i)->d_func()->set(this);
    }
}

dbool WaitCondition::waitMSec(duint milliSeconds) 
{ 
    SingleLock lock(&pdm->mutex); 
    pdm->numWaits++; 
    pdm->condVar.wait(pdm->mutex, milliSeconds); 
    pdm->numWaits--; 
    return pdm->prepReturn(); 
}

dbool WaitCondition::wait()
{ 
    SingleLock lock(&pdm->mutex); 
    pdm->numWaits++; 
    pdm->condVar.wait(pdm->mutex); 
    pdm->numWaits--; 
    return pdm->prepReturn(); 
}

dint WaitCondition::getNumWaits() const
{ 
    SingleLock lock(&pdm->mutex); 
    return pdm->numWaits; 
}

void WaitCondition::addGroup(ConditionGroup* group)
{
    SingleLock lock(&pdm->groupListMutex);
    if (pdm->groups == NULL)
        pdm->groups = new Vector<ConditionGroup*>();

    pdm->groups->append(group);
}

void WaitCondition::removeGroup(ConditionGroup* group)
{
    SingleLock lock(&pdm->groupListMutex);
    if (pdm->groups)
    {
        for (Vector<ConditionGroup*>::iterator iter = pdm->groups->begin(); iter != pdm->groups->constEnd(); iter++)
        {
            if ((*iter) == group)
            {
                pdm->groups->erase(iter);
                break;
            }
        }

        if (pdm->groups->size() <= 0)
        {
            delete pdm->groups;
            pdm->groups = NULL;
        }
    }
}

ConditionGroupPrivate::ConditionGroupPrivate() :
    signaled(NULL), condVar(actualCv, signaled), numWaits(0)
{

}

ConditionGroup::ConditionGroup(int num, WaitCondition* v1, ...) : 
    C_D(ConditionGroup)
{
    va_list ap;

    va_start(ap, v1);
    pdm->events.append(v1);
    num--; // account for v1
    for (;num > 0; num--)
        pdm->events.append(va_arg(ap,WaitCondition*));
    va_end(ap);

    // we preping for a wait, so we need to set the group value on
    // all of the CEvents. 
    for (dint i=0; i<pdm->events.size(); ++i)
        pdm->events.at(i)->addGroup(this);
}

ConditionGroup::ConditionGroup(WaitCondition* v1, ...) : 
    C_D(ConditionGroup)
{
    va_list ap;

    va_start(ap, v1);
    pdm->events.append(v1);
    bool done = false;
    while(!done)
    {
        WaitCondition* cur = va_arg(ap,WaitCondition*);
        if (cur)
            pdm->events.append(cur);
        else
            done = true;
    }
    va_end(ap);

    // we preping for a wait, so we need to set the group value on
    // all of the CEvents. 
    for (dint i=0; i<pdm->events.size(); ++i)
        pdm->events.at(i)->addGroup(this);
}

ConditionGroup::~ConditionGroup()
{
    for (dint i=0; i<pdm->events.size(); ++i)
        pdm->events.at(i)->removeGroup(this);

    D_D(ConditionGroup);
}

WaitCondition* ConditionGroup::wait() 
{ 
    return wait(UINT_MAX);
}

/**
* This will block until any one of the CEvents in the group are
* signaled or the timeout is reachec. If an event is signaled then
* it will return a pointer to that WaitCondition, otherwise it will return
* NULL.
*/
// locking is ALWAYS done in this order:
//  WaitCondition::groupListMutex -> ConditionGroup::mutex -> WaitCondition::mutex
//
// Notice that this method doesn't grab the WaitCondition::groupListMutex at all. This
// is fine. It just grabs the ConditionGroup::mutex and THEN the individual 
// WaitCondition::mutex's
WaitCondition* ConditionGroup::wait(unsigned int milliseconds)  
{ 
    SingleLock lock(&pdm->mutex); // grab ConditionGroup::mutex
    pdm->numWaits++; 

    // ==================================================
    // This block checks to see if any child events are 
    // signaled and sets 'signaled' to the first one it
    // finds.
    // ==================================================
    pdm->signaled = NULL;
    for (dint i=0; i<pdm->events.size(); ++i)
    {
        WaitCondition* cur = pdm->events.at(i);
        if (cur->pdm->signaled) 
            pdm->signaled = cur;
    }
    // ==================================================

    if(!pdm->signaled)
    {
        // both of these release the ConditionGroup::mutex
        if (milliseconds == UINT_MAX)
            pdm->condVar.wait(pdm->mutex); 
        else
            pdm->condVar.wait(pdm->mutex,milliseconds); 
    } // at this point the ConditionGroup::mutex is reacquired
    pdm->numWaits--; 

    // signaled should have been set by a call to ConditionGroup::Set
    WaitCondition* ret = pdm->signaled;
    if (pdm->numWaits == 0) 
    {
        if (pdm->signaled)
        // This acquires and releases the WaitCondition::mutex. This is fine since the
        //  ConditionGroup::mutex is already being held
            pdm->signaled->waitMSec(0); // reset the event if needed
        pdm->signaled = NULL;  // clear the signaled if all the waiters are gone
    }
    return ret;
}

dint ConditionGroup::getNumWaits()
{ 
    SingleLock lock(&pdm->mutex); 
    return pdm->numWaits; 
}

DM_END_NAMESPACE

