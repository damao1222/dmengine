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

#ifndef DMEXECUTOR_H
#define DMEXECUTOR_H

#include "dmnamespace.h"
#include "dmsingleton.h"

DM_BEGIN_NAMESPACE
class ExecTask;
DM_PRIVATE_CLASS(Executor);
class DM_DLL_EXPORT Executor : public Singleton<Executor>
{
    DM_DECLARE_PRIVATE(Executor)
public:
    enum EAddTaskResult{
        /** 
         * 任务为空.
         */
        eTaskIsNull = -2,
         /** 
         * 所有线程都被占用，该任务无法被执行.
         */
        eNoIdleThread = -1,
        /** 
         * 线程池没有线程正在运行.
         */
        eThreadPoolEmpty = 0,
        /** 
         * 直接执行该任务.
         */
        eRun = 1,
        /** 
         * 任务被放入执行队列.
         */
        eAdd = 2
    };

	/** 
     * 构造函数.
     */
    Executor();

	/** 
     * 析构函数.
     */
    ~Executor();

	/** 
     * 初始化线程池.
     * @param nNum  线程数.
     * @return 成功返回true，失败返回false.
     */
	dbool init(duint nNum);

    /** 
     * 增加线程池线程数量.
     * @param nNum  增加线程数.
     * @return 成功返回true，失败返回false.
     */
    dbool increase(duint nNum);

    /** 
     * 线程池线程数量.
     * @return 线程数量.
     */
    dint threadCount() const;

	/** 
     * 停止并等待所有线程池.
     */
	void finishAndWait();

	/** 
     * 运行一个任务，当有空闲线程可以执行任务则立即执行，否则返回false.
     * @param task  任务.
     * @param timeoutInMilliSec  任务执行的超时时间 0(短暂任务) 或 UINT_MAX(一直占用此线程).
     * @return 可以立即执行，返回true，没有空闲线程返回false.
     */
	dbool run(ExecTask *task, duint timeoutInMilliSec = 0);

	/** 
     * 将任务加入任务队列.
     * @param task  任务.
	 * @param priority  任务优先级，优先级高的任务先执行.
     * @param timeoutInMilliSec  任务执行的超时时间 0(短暂任务) 或 UINT_MAX(一直占用此线程).
     * @return 返回结果，见enum EAddTaskResult.
     */
	EAddTaskResult addTask(ExecTask *task, dint priority = 0, duint timeoutInMilliSec = 0);

    /** 
     * 检测线程池中是否有任务超时,如果有则尝试终止超时任务.
     * @param delta  时间间隔.
     */
    void checkTaskTimeout(dreal delta);
};
DM_END_NAMESPACE

#define dmExecutor DM_GLOBAL_OBJ(DM::Executor)

#endif // DMEXECUTOR_H
