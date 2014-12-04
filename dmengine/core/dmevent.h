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
#ifndef DMEVENT_H
#define DMEVENT_H
#include "dmglobal.h"
#include "dmrefptr.h"
DM_BEGIN_NAMESPACE
//内置事件类型
typedef enum{
    eUnknownEvent = -1,
    ePostEvent,             //跨线程传递事件
    ePaintEvent,            //绘制事件
    eTimeEvent,             //时间事件
    eBroadcastEvent,        //广播事件
    eUpdateEvent,           //更新事件
	eDeferredDeleteEvent,	//延迟删除事件：返回到event后销毁
	eDeferredReleaseEvent,	//延迟释放事件：返回到event后释放
	eExecutorEvent,			//线程执行器事件
    eInputBeginType = 128,  //输入事件类型开始值
    eWindowBeginType = 256, //窗体事件类型开始值
    eUserEvent = 1<<16      //用户自定义事件类型开始值
}EventType;

class Object;
class DM_DLL_EXPORT Event : public RefPtr<Event>
{
public:
    Event(): sender(NULL) {}
    virtual EventType type() const { return eUnknownEvent; }
    Object* sender;
};

class TimeEvent:public Event
{
public:
    TimeEvent() : id(0), interval(0), repeat(0) {}
    EventType type() const { return eTimeEvent; }

    duint32 id;
    dreal interval;
    dint repeat;
};

class DeferredDeleteEvent: public Event
{
public:
	EventType type() const { return eDeferredDeleteEvent; }
};

class DeferredReleaseEvent: public Event
{
public:
	EventType type() const { return eDeferredReleaseEvent; }
};

DM_END_NAMESPACE
#endif //DMEVENT_H
