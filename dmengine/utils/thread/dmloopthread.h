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
     * ���캯��
     */
    LoopThread();

    /** 
     * ��������
     */
    virtual ~LoopThread();

    /** 
     * ���¼������̶߳��еȴ����߳��ڴ������ô˽ӿڵĴ�����Ҫrelease
     * @param e  �¼�
     * @param priority  �¼����ȼ�
     * @return �ɹ�����true��ʧ�ܷ���false
     */
    dbool acceptEvent(Event *e, dint priority = 0);

    /** 
     * �Ƿ��ܽ����¼�
     * @return ���Է���true�����򷵻�false
     */
    dbool canAcceptEvent() const;

    /** 
     * �ж��߳��������¼��ȴ�������
     * @return �Ƿ���true�����򷵻�false
     */
    dbool hasEvent() const;

    /** 
     * �ȴ��߳��д�����¼�����
     * @return �¼�����
     */
    dint eventCount() const;

    /** 
     * ���õȴ��¼��ĳ�ʱʱ�䣬Ĭ��Ϊ0
     * @param iTimeoutInMilliSeconds  �¼�
     */
    void setWaitEventTimeout(duint iTimeoutInMilliSeconds);

    /** 
     * ����ÿ��ѭ����ദ����¼�������Ĭ��ÿ��ѭ����������̵߳��¼�ȫ������
     * @param number  ����
     */
    void setHandleEventNumber(duint number);

    /** 
     * ֹͣ�߳�ѭ�������Խ����߳�
     */
    void stopLoopAndFinish();

    /** 
     * �ж��Ƿ���ѭ�������У��̰߳�ȫ��
     * @param  �Ƿ���true�����򷵻�false
     */
    dbool isLooping() const;

    /** 
     * �ж��Ƿ���ѭ�������У����̰߳�ȫ��
     * @return  �Ƿ���true�����򷵻�false
     */
    dbool isLoopingUnlock() const;

	/** 
     * ������
     * @return  ��
     */
	Mutex* getLock() const;

protected:
    /** 
     * �߳̿�ʼʱ����
     */
    virtual void onStart() {}

    /** 
     * �ڴ����߳�ѭ���е��¼�֮ǰ���ã������̱߳���
     */
    virtual void prepareHandleEvent() {}

    /** 
     * �����߳�ѭ���е��¼���ÿ��ѭ�����ὫEventHandler�е��¼�ȫ������
     * @param e  �¼�
     * @param ret  �����¼��������EventHandler::Result
     */
    virtual void handleEvent(Event *e, EventHandler::Result ret) { DM_UNUSED(e); DM_UNUSED(ret); }

    /** 
     * ���߳�ѭ�����¼�������ɺ����
     */
    virtual void process() {}

    /** 
     * �߳��˳�����������ʱ����
     */
    virtual void onEnd() {}

protected:
    virtual void run();
};
DM_END_NAMESPACE
#endif // DMLOOPTHREAD_H
