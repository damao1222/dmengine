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


#ifndef LIVEOBJECT_H
#define LIVEOBJECT_H

#include "dmobject.h"
#include "dmevent.h"

DM_BEGIN_NAMESPACE
class Application;
class InteractiveObject;
DM_PRIVATE_CLASS(LiveObject);
class DM_DLL_EXPORT LiveObject: public Object
{
    DM_DECLARE_PRIVATE(LiveObject)
public:
    /**
     * Default destructor
     */
    virtual ~LiveObject();

    /** 
     * Start this LiveObject, you need call activate or g_application.activateObject to 
     * activate this object at first
     */
    virtual void start();

    /** 
     * Stop this LiveObject
     */
    virtual void stop();

    /** 
     * Pause this LiveObject
     * @param timeout  Pause time, if timeout, this object will move on
     */
    virtual void pause(float timeout);

    /** 
     * Get the stop state of object
     * @return Return true if this object is stopped
     */
    dbool isStopped() const;

    /** 
     * Get the running state of object
     * @return Return true if this object is not stopped
     */
    dbool isRunning() const;

    /** 
     * Get the pause state of object
     * @return Return true if this object is paused
     */
    dbool isPaused() const;

    /** 
     * ��ʼһ����ʱ�����ﵽ�趨ʱ���ᷢ��һ��TimeEvent,��Ҫ��дonTimeEvent(TimeEvent*)����׽����¼�
     * @param interval  ��ʱʱ�䣬����Ϊ��λ
     * @param repeat  �Ƿ�һֱ�ظ�
     * @return ��ʱ����ID
     */
    duint32 startTimer(dreal interval, dbool repeat = false);

    /** 
     * Reset the timer with specified timer ID
     * @param id  Id of timer
     * @return Return true if success else return false
     */
    dbool resetTimer(duint32 id);

    /** 
     * ֹͣ���м�ʱ��
     */
    void stopAllTimer();

    /** 
     * ָֹͣ����ʱ��
     * @param id  ��Ҫֹͣ�ļ�ʱ����ID
     */
    dbool stopTimer(duint32 id);

    /** 
     * ���ָ����ʱ���ظ��Ĵ���
     * @param id  ��ʱ����ID
     * @return �ظ�����
     */
    dint timerRepeatCount(duint32 id) const;

    /** 
     * ��ü�ʱ�����е�״̬
     * @param id  ��ʱ����ID
     * @return �����ʱ���������з���true�����򷵻�false
     */
    dbool isTimerRunning(duint32 id) const;

    /** 
     * ��ü�ʱ��ʣ���ʱ��
     * @param id  ��ʱ����ID
     * @return ��ʱ��ʣ��ʱ��
     */
    dreal remainTime(duint32 id) const;

     /** 
     * �����object��ʹ֮�����ܵ�ʱ������Ӱ��
     */
    void activate();

    /** 
     * ͣ�ø�object��ʹ֮������ʱ������Ӱ��
     */
    void deactivate();

    /** 
     * �жϸ�LiveObject�Ƿ��Ѿ�������(ʹ��activate��g_application.activateObject�����object)
     * @return ����Ѿ��������true�����򷵻�false
     */
    dbool isActivated() const;

    /** 
     * �жϸ�LiveObject�Ƿ��Ѿ���ͣ��
     * @return ���û�б������true�����򷵻�false
     */
    dbool isDeactivated() const;

    /** 
     * �жϸ�LiveObject�Ƿ��������
     * @return �������룬����true�����򷵻�false
     */
    dbool isAcceptInput() const;

    //internal
    dbool event(Event *e);
    friend class Application;
    friend class InteractiveObject;
protected:
    /**
     * Default constructor
     */
    LiveObject();
    LiveObject(LiveObjectPrivate *p);

    /** 
     * ÿ��Ӧ�õ���ʱ��Ƭʱ���ã��ƶ�Object��ʱ������������Ӧ
     * @param interval  ʱ��Ƭ�Ĵ�С
     */
    virtual void onframeMove(float interval) { DM_UNUSED(interval); }

    /** 
     * ������startʱ������
     */
    virtual void onStart() {}

    /** 
     * ������stopʱ������
     */
    virtual void onStop() {}

    /** 
     * ����ʱ��ʱ�䵽��ʱ������
     * @param event  ʱ���¼�
     */
    virtual void onTimeEvent(TimeEvent *event) { DM_UNUSED(event); } 
    
private:
    void frameMove(float dt) ;
    DM_DISABLE_COPY(LiveObject)
};
DM_END_NAMESPACE
#endif // LIVEOBJECT_H
