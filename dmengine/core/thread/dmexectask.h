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
         * ���ص�.
         */
		eNone = 0, 
        /** 
         * �߳��е��ûص�.
         */
		eCurrentThread = 1,
        /** 
         * ���߳��е��ûص�.
         */
		eMainThread = 1<<1,
        /** 
         * �̼߳����߳��ж����ûص�.
         */
		eCurrentAndMain = eCurrentThread | eMainThread
	};

    /** 
     * Ĭ�Ϲ��캯��.
     */
	ExecResult() : param(NULL), type(eNone) {}

    /** 
     * ���캯��.
     * @param eType  ������ͣ�ִ�������������ͽ��лص�.
     * @param pParam  ����.
     */
	ExecResult(CallbackType eType, void *pParam = NULL) : param(pParam), type(eType) {}

	void			*param;
	CallbackType	type; 
	
    /** 
     * ��ʵ�֣����ص�.
     */
	static ExecResult empty;
};

class DM_DLL_EXPORT ExecTask
{
public:
    enum AbortType{
        /** 
         * δ֪�ж�����.
         */
        eUnknown = 0,
        /** 
         * ����ʱ������ֹ.
         */
        eTimeout,
        /** 
         * �߳��˳�������ֹ.
         */
        eQuit
    };

    /** 
     * ����.
     */
    virtual ExecResult run() = 0;

    /** 
     * ����ص�.
     * @param pParam  ����.
     * @param bMainThread  �Ƿ�����̷߳���.
     */
	virtual void onTaskDone(void *pParam, dbool bMainThread) { DM_UNUSED(pParam); DM_UNUSED(bMainThread); }

    /** 
     * ��ֹ���񣬵�ִ�����˳���������ʱ�������Ҫ��ֹ��ǰ����ʱ����ô˺�����������Ҫ����Դ���д��������˳�.
     * @notice �����������˳�ʱʱ�䣬���ڼ�ⳬʱ������ѭ���н��У����Ե�����ʱʱ���ܻ��ε��ô˺���.
     * @param eType  ��ֹ��ԭ�򣬼�enum AbortType.
     */
    virtual void abort(AbortType eType) { DM_UNUSED(eType); }
};
DM_END_NAMESPACE

#endif // DMEXECTASK_H
