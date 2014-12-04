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
//�����¼�����
typedef enum{
    eUnknownEvent = -1,
    ePostEvent,             //���̴߳����¼�
    ePaintEvent,            //�����¼�
    eTimeEvent,             //ʱ���¼�
    eBroadcastEvent,        //�㲥�¼�
    eUpdateEvent,           //�����¼�
	eDeferredDeleteEvent,	//�ӳ�ɾ���¼������ص�event������
	eDeferredReleaseEvent,	//�ӳ��ͷ��¼������ص�event���ͷ�
	eExecutorEvent,			//�߳�ִ�����¼�
    eInputBeginType = 128,  //�����¼����Ϳ�ʼֵ
    eWindowBeginType = 256, //�����¼����Ϳ�ʼֵ
    eUserEvent = 1<<16      //�û��Զ����¼����Ϳ�ʼֵ
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
