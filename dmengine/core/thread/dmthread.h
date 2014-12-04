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
     * �����߳�
     * @param stacksize  �߳�ջ��С
     * @return �ɹ�����1��ʧ�ܷ���-1
     */
    dint start(duint stacksize = 0);

    /** 
     * �ж��߳��Ƿ���Ȼ������
     * @return ����ִ�з���true�����򷵻�false
     */
    virtual dbool isRunning() const;

    /** 
     * �ж��Ƿ��ڵ�ǰ�߳�
     * @return �ڵ�ǰ�̷߳���true�����򷵻�false
     */
    dbool isCurrentThread() const;

    /** 
     * ����̱߳�ʶ��id
     * @return �߳�id
     */
    ThreadIdentifier currentId() const;

    /** 
     * ǿ�ƽ����̣߳����������쳣��Ӱ�죬������ʹ��
     */
    void terminate();

    /** 
     * �ȴ���ֱ�����߳��˳��������������̵߳���
     * @param �ȴ���ʱ�䣬��λΪ����
     * @return �ɹ�����true��ʧ�ܷ���false
     */
    dbool wait(duint msec = UINT_MAX);

    /** 
     * �����߳����ȼ�
     * @param priority  ���ȼ�
     * @return �ɹ�����true��ʧ�ܷ���false
     */
    dbool setPriority(dint priority);

    /** 
     * ��ø��̵߳����ȼ�
     * @return �߳����ȼ�
     */
    dint priority() const;

    /** 
     * �����߳�ջ�Ĵ�С�������������߳�֮ǰ�͵���
     * @param stacksize  �߳�ջ��С
     */
    void setStackSize(duint stackSize);

    /** 
     * ����߳�ջ�Ĵ�С
     * @return �߳�ջ��С
     */
    duint stackSize() const;

    /** 
     * ���ߵ�ǰ�߳�
     * @param ms  ���ߵ�ʱ�䣬��λΪ����
     */
    static void sleep(dint ms);

    /** 
     * ��õ�ǰ�̵߳�id
     * @return ��ǰ�߳�id
     */
    static ThreadIdentifier currentThreadId();

    /** 
     * �ж��Ƿ���ָ���߳�id���߳���
     * @param id �߳�id
     * @return �����߳�idΪ����id���߳�ʱ����true�����򷵻�false
     */
    static dbool isCurrentThread(const ThreadIdentifier id);

    /** 
     * �ó�cpu��ʹ������ͬ���ȼ����߳̿���ִ��
     */
    static void yieldCurrentThread();

    //internal
    static dbool globalInit();
protected:

    /** 
     * �߳������Ĺ������뵽�÷����У���д�˷�����
     */
    virtual void run();
};
DM_END_NAMESPACE
#endif // DMTHREAD_H
