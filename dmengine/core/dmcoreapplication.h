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
#ifndef DMCOREAPPLICATION_H
#define DMCOREAPPLICATION_H
#include "dmnamespace.h"
DM_BEGIN_NAMESPACE
class Object;
class Event;
class EventFilter;
class Application;
DM_PRIVATE_CLASS(CoreApplication);
class DM_DLL_EXPORT CoreApplication
{
    DM_DECLARE_PRIVATE(CoreApplication)
public:
    CoreApplication();
    virtual ~CoreApplication();

    /** 
     * 判断是否是在主线程
     * @return 在主线程返回true，否则返回false
     */
    static dbool isMainThread();

    /** 
     * 向Object发送一个事件（同步）
     * @param obj  事件发送的对象
     * @param e  需要发送的事件
     * @return 如果Object希望该事件被丢弃，返回true，否则返回false
     */
    static dbool sendEvent(Object *obj, Event *e);

    /** 
     * 向Object发送一个事件（如果调用此方法是在主线程，则直接发送事件，效果与sendEvent一致
     * 否则将立即返回并在主线程获得控制权后处理该event）
     * @param obj  事件发送的对象
     * @param e  需要发送的事件
     * @param priority  处理的优先级
     */
    static void postEvent(Object *obj, Event *e, dint priority = 0);

    /** 
     * 应用单例
     * @return 单例指针
     */
    static CoreApplication* global();

    /** 
     * 添加一个事件过滤器，事件过滤器比事件需要传递的对象更早接收到事件，对事件进行处理
     * @param filter  事件过滤器
     * @return 如果该事件过滤器已经添加，返回false，否则返回true
     */
    static dbool addEventFilter(EventFilter *filter);

    /** 
     * 移除一个事件过滤器
     * @param filter  事件过滤器
     * @return 如果该事件过滤器已经添加，并被顺利移除，返回true，否则返回false
     */
    static dbool removeEventFilter(EventFilter *filter);

    /** 
     * 启用application的frameMove
     */
    void start();

    /** 
     * 使application的frameMove失效
     */
    void stop();

    /** 
     * 使application的frameMove暂时失效
     * @param timeout  frameMove暂时失效的时间
     */
    void pause(float timeout);

    /** 
     * 应用是否停止（frameMove是否失效）
     * @return 如果应用停止，则返回true，否则返回false
     */
    dbool isStopped() const;

    /** 
     * 应用是否停止（frameMove是否失效）
     * @return 如果应用没有被停止或暂停，则返回true，否则返回false
     */
    dbool isRunning() const;

    /** 
     * 应用是否被暂停（frameMove是否失效）
     * @return 如果应用被暂停，则返回true，否则返回false
     */
    dbool isPaused() const;

    /** 
     * 给予应用时间片，推动应用内LiveObject的时间因素影响
     * 注意：每次调用该接口都将释放AutoReleaseManager中管理的auto release对象
     * @param interval  时间片的长度，是单一的时间片，即上次调用frameMove到这次调用frameMove的时间间隔
     */
    void frameMove(float interval);

    /** 
     * 启动应用循环，该循环为主线程，如果不需要此循环则调用frameMove刷新应用
     */
    dint loop();

    /** 
     * 退出应用循环
     */
    void quit();

    //internal
    friend class Application;
protected:
    CoreApplication(CoreApplicationPrivate *data);
    virtual void onframeMove(float interval) { DM_UNUSED(interval); }

    static CoreApplication *self;
};
DM_END_NAMESPACE
#endif //DMCOREAPPLICATION_H
