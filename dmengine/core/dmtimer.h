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

#ifndef TIMER_H
#define TIMER_H
#include "dmliveobject.h"
#include "dmobjectpro.h"
DM_BEGIN_NAMESPACE
DM_PRIVATE_CLASS(Timer);
class Timer;
class TimeoutCallback
{
public:
    virtual void onTimeout(Timer *timer, TimeEvent *event) = 0;
};

typedef void (*TimerCallbackFunc)(void *obj, TimeEvent *event);

class DM_DLL_EXPORT Timer: public LiveObject
{
    DM_DECLARE_PRIVATE(Timer)
public:
    Timer();

    /** 
     * 构造函数
     * @param func  超时回调函数
     * @param target 超时回调附加参数  
     */
    explicit Timer(TimerCallbackFunc func, void *target = NULL);

    /** 
     * 构造函数
     * @param cb  超时回调类指针
     */
    explicit Timer(TimeoutCallback *cb);
    ~Timer();

    /** 
     * 设置计时器的超时时间
     * @param interval  超时时间
     */
    void setInterval(dreal interval);

    /** 
     * 设置计时器是否重复，默认为非重复
     * @param flag  重复设置为true，否则设置为false
     */
    void setRepeat(dbool flag);

    /** 
     * 获得计时器超时后回调的次数
     * @return 重复次数
     */
    dint repeatCount() const;

    /** 
     * 获得该计时器ID
     * @return 计时器的ID
     */
    dint id() const;

    /** 
     * 判定该计时器是否正在计时
     * @return 正在及时返回true，否则返回false
     */
    dbool isTiming() const;

    /** 
     * 启动计时器
     * @param interval  超时时间，以秒为单位
     * @param repeat  是否一直重复
     */
    void start(dreal interval, dbool repeat = false);

    /** 
     * 启动计时器，超时时间调用setInterval设定
     */
    void start();

    /** 
     * 重置计时器，计时器将重新开始计时，当计时器已经超时将失败
     * @return 如果成功返回true，否则返回false
     */
    dbool reset();

    /** 
     * 停止计时
     */
    void stop();

protected:
    void onTimeEvent(TimeEvent *event);

    DM_OBJECT
    /** 
     * 超时信号，将调用连接到该信号的方法
     */
    DM_SIGNAL(emitTimeout)

private:
    DM_DISABLE_COPY(Timer)
};

#define DM_CONNECT_TIMER(TimerPtr, ClassName, ClassPtr, FuncName) \
            DM_CONNECT(DM::Timer, TimerPtr, emitTimeout, ClassName, ClassPtr, FuncName)

template<class T, void(T::*fn)(TimeEvent *event)>
void TimerCallback(void *obj, TimeEvent *event)
{
    (static_cast<T*>(obj)->*fn)(event);
}

/*
 * This Template is useful, for example:
    class Test
    {
    public:
        Test()
        {
            timer = new Timer(TimerCallback<Test, &Test::onMyTimeout>, this);

            //Test::onMyTimeout will be called after 0.1 second
            timer->start(0.1f);
        }

        ~Test()
        {
            timer->release();
        }

        void onMyTimeout(TimeEvent *event) 
        {
            //do something
        }

    private:
        Timer *timer;
    }
 */
DM_END_NAMESPACE
#endif // TIMER_H
