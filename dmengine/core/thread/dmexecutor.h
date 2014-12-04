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
         * ����Ϊ��.
         */
        eTaskIsNull = -2,
         /** 
         * �����̶߳���ռ�ã��������޷���ִ��.
         */
        eNoIdleThread = -1,
        /** 
         * �̳߳�û���߳���������.
         */
        eThreadPoolEmpty = 0,
        /** 
         * ֱ��ִ�и�����.
         */
        eRun = 1,
        /** 
         * ���񱻷���ִ�ж���.
         */
        eAdd = 2
    };

	/** 
     * ���캯��.
     */
    Executor();

	/** 
     * ��������.
     */
    ~Executor();

	/** 
     * ��ʼ���̳߳�.
     * @param nNum  �߳���.
     * @return �ɹ�����true��ʧ�ܷ���false.
     */
	dbool init(duint nNum);

    /** 
     * �����̳߳��߳�����.
     * @param nNum  �����߳���.
     * @return �ɹ�����true��ʧ�ܷ���false.
     */
    dbool increase(duint nNum);

    /** 
     * �̳߳��߳�����.
     * @return �߳�����.
     */
    dint threadCount() const;

	/** 
     * ֹͣ���ȴ������̳߳�.
     */
	void finishAndWait();

	/** 
     * ����һ�����񣬵��п����߳̿���ִ������������ִ�У����򷵻�false.
     * @param task  ����.
     * @param timeoutInMilliSec  ����ִ�еĳ�ʱʱ�� 0(��������) �� UINT_MAX(һֱռ�ô��߳�).
     * @return ��������ִ�У�����true��û�п����̷߳���false.
     */
	dbool run(ExecTask *task, duint timeoutInMilliSec = 0);

	/** 
     * ����������������.
     * @param task  ����.
	 * @param priority  �������ȼ������ȼ��ߵ�������ִ��.
     * @param timeoutInMilliSec  ����ִ�еĳ�ʱʱ�� 0(��������) �� UINT_MAX(һֱռ�ô��߳�).
     * @return ���ؽ������enum EAddTaskResult.
     */
	EAddTaskResult addTask(ExecTask *task, dint priority = 0, duint timeoutInMilliSec = 0);

    /** 
     * ����̳߳����Ƿ�������ʱ,�����������ֹ��ʱ����.
     * @param delta  ʱ����.
     */
    void checkTaskTimeout(dreal delta);
};
DM_END_NAMESPACE

#define dmExecutor DM_GLOBAL_OBJ(DM::Executor)

#endif // DMEXECUTOR_H
