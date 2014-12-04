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
     * 开始一个计时器，达到设定时间后会发出一个TimeEvent,需要重写onTimeEvent(TimeEvent*)来捕捉这个事件
     * @param interval  超时时间，以秒为单位
     * @param repeat  是否一直重复
     * @return 计时器的ID
     */
    duint32 startTimer(dreal interval, dbool repeat = false);

    /** 
     * Reset the timer with specified timer ID
     * @param id  Id of timer
     * @return Return true if success else return false
     */
    dbool resetTimer(duint32 id);

    /** 
     * 停止所有计时器
     */
    void stopAllTimer();

    /** 
     * 停止指定定时器
     * @param id  需要停止的计时器的ID
     */
    dbool stopTimer(duint32 id);

    /** 
     * 获得指定定时器重复的次数
     * @param id  计时器的ID
     * @return 重复次数
     */
    dint timerRepeatCount(duint32 id) const;

    /** 
     * 获得计时器运行的状态
     * @param id  计时器的ID
     * @return 如果计时器正在运行返回true，否则返回false
     */
    dbool isTimerRunning(duint32 id) const;

    /** 
     * 获得计时器剩余的时间
     * @param id  计时器的ID
     * @return 计时器剩余时间
     */
    dreal remainTime(duint32 id) const;

     /** 
     * 激活该object，使之可以受到时间因素影响
     */
    void activate();

    /** 
     * 停用该object，使之不在受时间因素影响
     */
    void deactivate();

    /** 
     * 判断该LiveObject是否已经被激活(使用activate或g_application.activateObject激活该object)
     * @return 如果已经被激活返回true，否则返回false
     */
    dbool isActivated() const;

    /** 
     * 判断该LiveObject是否已经被停用
     * @return 如果没有被激活返回true，否则返回false
     */
    dbool isDeactivated() const;

    /** 
     * 判断该LiveObject是否接受输入
     * @return 接受输入，返回true，否则返回false
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
     * 每次应用到达时间片时调用，推动Object对时间因素做出反应
     * @param interval  时间片的大小
     */
    virtual void onframeMove(float interval) { DM_UNUSED(interval); }

    /** 
     * 当调用start时被调用
     */
    virtual void onStart() {}

    /** 
     * 当调用stop时被调用
     */
    virtual void onStop() {}

    /** 
     * 当计时器时间到达时被调用
     * @param event  时间事件
     */
    virtual void onTimeEvent(TimeEvent *event) { DM_UNUSED(event); } 
    
private:
    void frameMove(float dt) ;
    DM_DISABLE_COPY(LiveObject)
};
DM_END_NAMESPACE
#endif // LIVEOBJECT_H
