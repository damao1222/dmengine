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

#ifndef DMCONDITION_H
#define DMCONDITION_H

#include "private/dmconditionimpl.h"
#include "dmendtime.h"

DM_BEGIN_NAMESPACE
template <typename P> 
class TightConditionVariable
{
    ConditionVariable& cond;
    P predicate;

public:
    inline TightConditionVariable(ConditionVariable& cv, P predicate_) : 
        cond(cv), predicate(predicate_) 
    {}

    template <typename L> 
    inline void wait(L& lock) 
    { 
        while(!predicate) 
            cond.wait(lock); 
    }

    template <typename L> 
    inline dbool wait(L& lock, dulong milliseconds);

    inline void notifyAll() { cond.notifyAll(); }
    inline void notify() { cond.notify(); }
};

class ConditionGroup;
DM_PRIVATE_CLASS(WaitCondition);
class DM_DLL_EXPORT WaitCondition
{
    friend class ConditionGroup;
    DM_DECLARE_PRIVATE(WaitCondition)
public:
    WaitCondition(dbool manual = false, dbool signaled_ = false);
    ~WaitCondition();

    void reset();

    void set();

    /**
    * This will wait up to 'milliSeconds' milliseconds for the Event
    *  to be triggered. The method will return 'true' if the Event
    *  was triggered. Otherwise it will return false.
    */
    dbool waitMSec(duint milliSeconds);

    /**
    * This will wait for the Event to be triggered. The method will return 
    * 'true' if the Event was triggered. If it was either interrupted
    * it will return false. Otherwise it will return false.
    */
    dbool wait();

    /**
    * This is mostly for testing. It allows a thread to make sure there are 
    *  the right amount of other threads waiting.
    */
    dint getNumWaits() const;

    void addGroup(ConditionGroup* group);
    void removeGroup(ConditionGroup* group);

    DM_DISABLE_COPY(WaitCondition);
};

DM_PRIVATE_CLASS(ConditionGroup);
class ConditionGroup
{
    friend class WaitCondition;
    DM_DECLARE_PRIVATE(ConditionGroup)
public:

    /**
    * Create a ConditionGroup from a number of WaitCondition. num is the number
    *  of Conditions that follow. E.g.:
    *
    *  ConditionGroup g(3, event1, event2, event3);
    */
    ConditionGroup(dint num, WaitCondition* v1, ...);

    /**
    * Create a ConditionGroup from a number of WaitCondition. The parameters
    *  should form a NULL terminated list of WaitCondition*'s
    *
    *  ConditionGroup g(event1, event2, event3, NULL);
    */
    ConditionGroup(WaitCondition* v1, ...);
    ~ConditionGroup();

    /**
    * This will block until any one of the WaitCondition in the group are
    * signaled at which point a pointer to that WaitConditions will be 
    * returned.
    */
    WaitCondition* wait();

    /**
    * This will block until any one of the WaitConditions in the group are
    * signaled or the timeout is reachec. If an event is signaled then
    * it will return a pointer to that WaitCondition, otherwise it will return
    * NULL.
    */
    WaitCondition* wait(duint milliseconds);

    /**
    * This is mostly for testing. It allows a thread to make sure there are 
    *  the right amount of other threads waiting.
    */
    dint getNumWaits();

    DM_DISABLE_COPY(ConditionGroup);
};

template <typename P> 
template <typename L> 
inline dbool TightConditionVariable<P>::wait(L& lock, dulong milliseconds)
{
    dbool ret = true;
    if (!predicate)
    {
        if (!milliseconds)
        {
            cond.wait(lock, milliseconds /* zero */);
            return !(!predicate); // eh? I only require the ! operation on P
        }
        else
        {
            EndTime endTime((duint)milliseconds);
            for (dbool notdone = true; notdone && ret == true;
                        ret = (notdone = (!predicate)) ? ((milliseconds = endTime.leftTime()) != 0) : true)
                cond.wait(lock,milliseconds);
        }
    }
    return ret;
}
DM_END_NAMESPACE
#endif // DMCONDITION_H
