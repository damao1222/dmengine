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

#ifndef DMEXECTASK_H
#define DMEXECTASK_H

#include "dmnamespace.h"

DM_BEGIN_NAMESPACE

class DM_DLL_EXPORT ExecResult
{
public:
	enum CallbackType{
        /** 
         * 不回调.
         */
		eNone = 0, 
        /** 
         * 线程中调用回调.
         */
		eCurrentThread = 1,
        /** 
         * 主线程中调用回调.
         */
		eMainThread = 1<<1,
        /** 
         * 线程及主线程中都调用回调.
         */
		eCurrentAndMain = eCurrentThread | eMainThread
	};

    /** 
     * 默认构造函数.
     */
	ExecResult() : param(NULL), type(eNone) {}

    /** 
     * 构造函数.
     * @param eType  结果类型，执行器将根据类型进行回调.
     * @param pParam  参数.
     */
	ExecResult(CallbackType eType, void *pParam = NULL) : param(pParam), type(eType) {}

	void			*param;
	CallbackType	type; 
	
    /** 
     * 空实现，不回调.
     */
	static ExecResult empty;
};

class DM_DLL_EXPORT ExecTask
{
public:
    enum AbortType{
        /** 
         * 未知中断类型.
         */
        eUnknown = 0,
        /** 
         * 任务超时，被终止.
         */
        eTimeout,
        /** 
         * 线程退出，被终止.
         */
        eQuit
    };

    /** 
     * 任务.
     */
    virtual ExecResult run() = 0;

    /** 
     * 任务回调.
     * @param pParam  参数.
     * @param bMainThread  是否从主线程返回.
     */
	virtual void onTaskDone(void *pParam, dbool bMainThread) { DM_UNUSED(pParam); DM_UNUSED(bMainThread); }

    /** 
     * 终止任务，当执行器退出，或任务超时等情况需要终止当前任务时会调用此函数，任务需要对资源进行处理并尽快退出.
     * @notice 当任务设置了超时时间，由于检测超时是在主循环中进行，所以当任务超时时可能会多次调用此函数.
     * @param eType  终止的原因，见enum AbortType.
     */
    virtual void abort(AbortType eType) { DM_UNUSED(eType); }
};
DM_END_NAMESPACE

#endif // DMEXECTASK_H
