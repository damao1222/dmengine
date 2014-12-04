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

#ifndef DMLOOPTHREAD_H
#define DMLOOPTHREAD_H

#include "thread/dmthread.h"
#include "dmeventhandler.h"

DM_BEGIN_NAMESPACE
class Event;
class Mutex;
DM_PRIVATE_CLASS(LoopThread);
class DM_DLL_EXPORT LoopThread : public Thread
{
    DM_DECLARE_PRIVATE(LoopThread)
public:
    /** 
     * 构造函数
     */
    LoopThread();

    /** 
     * 析构函数
     */
    virtual ~LoopThread();

    /** 
     * 将事件放入线程队列等待在线程内处理，调用此接口的代码需要release
     * @param e  事件
     * @param priority  事件优先级
     * @return 成功返回true，失败返回false
     */
    dbool acceptEvent(Event *e, dint priority = 0);

    /** 
     * 是否还能接受事件
     * @return 可以返回true，否则返回false
     */
    dbool canAcceptEvent() const;

    /** 
     * 判断线程中仍有事件等待被处理
     * @return 是返回true，否则返回false
     */
    dbool hasEvent() const;

    /** 
     * 等待线程中处理的事件个数
     * @return 事件个数
     */
    dint eventCount() const;

    /** 
     * 设置等待事件的超时时间，默认为0
     * @param iTimeoutInMilliSeconds  事件
     */
    void setWaitEventTimeout(duint iTimeoutInMilliSeconds);

    /** 
     * 设置每次循环最多处理的事件数量，默认每次循环将放入此线程的事件全部处理
     * @param number  数量
     */
    void setHandleEventNumber(duint number);

    /** 
     * 停止线程循环，尝试结束线程
     */
    void stopLoopAndFinish();

    /** 
     * 判断是否在循环过程中（线程安全）
     * @param  是返回true，否则返回false
     */
    dbool isLooping() const;

    /** 
     * 判断是否在循环过程中（非线程安全）
     * @return  是返回true，否则返回false
     */
    dbool isLoopingUnlock() const;

	/** 
     * 返回锁
     * @return  锁
     */
	Mutex* getLock() const;

protected:
    /** 
     * 线程开始时调用
     */
    virtual void onStart() {}

    /** 
     * 在处理线程循环中的事件之前调用，用于线程保护
     */
    virtual void prepareHandleEvent() {}

    /** 
     * 处理线程循环中的事件，每次循环都会将EventHandler中的事件全部处理
     * @param e  事件
     * @param ret  处理事件结果，见EventHandler::Result
     */
    virtual void handleEvent(Event *e, EventHandler::Result ret) { DM_UNUSED(e); DM_UNUSED(ret); }

    /** 
     * 在线程循环的事件处理完成后调用
     */
    virtual void process() {}

    /** 
     * 线程退出，即将结束时调用
     */
    virtual void onEnd() {}

protected:
    virtual void run();
};
DM_END_NAMESPACE
#endif // DMLOOPTHREAD_H
