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
#ifndef DMEVENTHANDLER_H
#define DMEVENTHANDLER_H
#include "dmnamespace.h"

DM_BEGIN_NAMESPACE

#define MAX_HANDLE_EVENT_SIZE 1<<12

DM_PRIVATE_CLASS(EventHandler);
class Event;
class Mutex;
class DM_DLL_EXPORT EventHandler
{
    DM_DECLARE_PRIVATE(EventHandler)
public:
    enum Result
    {
        eOK               = 1,  //正常
        eTimeout          = 0,  //获得事件超时
        eAbort            = -1, //被忽略
        eUninitialized    = -2, //未初始化
        eInvalidEvent     = -3, //无效的事件
        eOOM              = -4  //内存溢出
    };

    EventHandler();
    ~EventHandler();

    /** 
     * 开始接受事件，并设定最多可处理事件的大小，默认为4096个（线程安全）
     * @param size  最多可以处理的事件总大小
     */
    void start(dint size = MAX_HANDLE_EVENT_SIZE);

    /** 
     * 停止接收事件，并清空所有等待处理的事件（线程安全）
     */
    void stop();

    /** 
     * 取消getAndRetain等待获取事件（线程安全）
     */
    void abort();

    /** 
     * 清理所有待处理的事件（线程安全）
     */
    void clean();

    /** 
     * 添加一个event到handler, 调用此接口的代码段应该负责release event（线程安全）
     * @param pEvent  事件
     * @param priority  事件优先级，高优先级的将优先被处理
     * @return 成功返回true，失败返回false
     */
    dbool put(Event *pEvent, dint priority = 0);

    /** 
     * 从handler中获得一个event，调用此接口的代码段应该负责release event（线程安全）
     * @param pEvent  存储事件指针的指针
     * @param iTimeoutInMilliSeconds  超时时间
     * @return 获取事件的结果，详情见枚举Result的定义
     */
    Result getAndRetain(Event** pEvent, duint iTimeoutInMilliSeconds);

    /** 
     * 判断handler是否还能接收事件（线程安全）
     * @return 能继续接受返回false，否则返回true
     */
    dbool isFull() const;

    /** 
     * 判断是否有事件等待被处理（线程安全）
     * @return 有事件返回true，否则返回false
     */
    dbool hasEvent() const;

    /** 
     * 得到等待被处理的事件的总数（线程安全）
     * @return 事件总数
     */
    dint eventCount() const;

    /** 
     * 取消自动释放，从对象池中取出该对象。注意，调用该方法不会release该对象，需要自己release
     * @return 返回该handler的互斥锁
     */
    Mutex& mutex();
};
DM_END_NAMESPACE
#endif //DMEVENTHANDLER_H
