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

#ifndef DMTHREAD_H
#define DMTHREAD_H
#include "dmnamespace.h"
#include "dmthread_arch.h"
DM_BEGIN_NAMESPACE
DM_PRIVATE_CLASS(Thread);
class DM_DLL_EXPORT Thread
{
    DM_DECLARE_PRIVATE(Thread)
public:
    Thread();
    virtual ~Thread();

    /** 
     * 启动线程
     * @param stacksize  线程栈大小
     * @return 成功返回1，失败返回-1
     */
    dint start(duint stacksize = 0);

    /** 
     * 判断线程是否仍然在运行
     * @return 仍在执行返回true，否则返回false
     */
    virtual dbool isRunning() const;

    /** 
     * 判断是否在当前线程
     * @return 在当前线程返回true，否则返回false
     */
    dbool isCurrentThread() const;

    /** 
     * 获得线程标识的id
     * @return 线程id
     */
    ThreadIdentifier currentId() const;

    /** 
     * 强制结束线程，将可能有异常的影响，不建议使用
     */
    void terminate();

    /** 
     * 等待，直到该线程退出，必须在其他线程调用
     * @param 等待的时间，单位为毫秒
     * @return 成功返回true，失败返回false
     */
    dbool wait(duint msec = UINT_MAX);

    /** 
     * 设置线程优先级
     * @param priority  优先级
     * @return 成功返回true，失败返回false
     */
    dbool setPriority(dint priority);

    /** 
     * 获得该线程的优先级
     * @return 线程优先级
     */
    dint priority() const;

    /** 
     * 设置线程栈的大小，必须在启动线程之前就调用
     * @param stacksize  线程栈大小
     */
    void setStackSize(duint stackSize);

    /** 
     * 获得线程栈的大小
     * @return 线程栈大小
     */
    duint stackSize() const;

    /** 
     * 休眠当前线程
     * @param ms  休眠的时间，单位为毫秒
     */
    static void sleep(dint ms);

    /** 
     * 获得当前线程的id
     * @return 当前线程id
     */
    static ThreadIdentifier currentThreadId();

    /** 
     * 判定是否处于指定线程id的线程中
     * @param id 线程id
     * @return 处于线程id为参数id的线程时返回true，否则返回false
     */
    static dbool isCurrentThread(const ThreadIdentifier id);

    /** 
     * 让出cpu，使其他相同优先级的线程可以执行
     */
    static void yieldCurrentThread();

    //internal
    static dbool globalInit();
protected:

    /** 
     * 线程内做的工作放入到该方法中，重写此方法。
     */
    virtual void run();
};
DM_END_NAMESPACE
#endif // DMTHREAD_H
