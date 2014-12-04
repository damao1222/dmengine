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
#ifndef DMCOREAPPLICATION_P_H
#define DMCOREAPPLICATION_P_H

#include "dmobject.h"
#include "events/dmpostevent.h"
#include "thread/dmthread.h"
#include "dmvector.h"
#include "dmeventfilter.h"
#include "dmeventhandler.h"

DM_BEGIN_NAMESPACE  
class CoreApplicationPrivate
{
public:
    virtual ~CoreApplicationPrivate();
    dbool filterEvent(Object *obj, Event *e);
    dbool sendEvent(Object *obj, Event *e);
	dbool sendEvent(PostEvent *post);
    void processPostEvent(float dt);

    ThreadIdentifier mainThreadId;
    Vector<EventFilter*> filterList;
    EventHandler handler;

    dbool quit;
    volatile dbool stop;
    dreal pausetime;
    dreal lastpause;
    duint delta;
};

DM_END_NAMESPACE
#endif //DMCOREAPPLICATION_P_H
