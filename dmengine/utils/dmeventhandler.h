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
        eOK               = 1,  //����
        eTimeout          = 0,  //����¼���ʱ
        eAbort            = -1, //������
        eUninitialized    = -2, //δ��ʼ��
        eInvalidEvent     = -3, //��Ч���¼�
        eOOM              = -4  //�ڴ����
    };

    EventHandler();
    ~EventHandler();

    /** 
     * ��ʼ�����¼������趨���ɴ����¼��Ĵ�С��Ĭ��Ϊ4096�����̰߳�ȫ��
     * @param size  �����Դ�����¼��ܴ�С
     */
    void start(dint size = MAX_HANDLE_EVENT_SIZE);

    /** 
     * ֹͣ�����¼�����������еȴ�������¼����̰߳�ȫ��
     */
    void stop();

    /** 
     * ȡ��getAndRetain�ȴ���ȡ�¼����̰߳�ȫ��
     */
    void abort();

    /** 
     * �������д�������¼����̰߳�ȫ��
     */
    void clean();

    /** 
     * ���һ��event��handler, ���ô˽ӿڵĴ����Ӧ�ø���release event���̰߳�ȫ��
     * @param pEvent  �¼�
     * @param priority  �¼����ȼ��������ȼ��Ľ����ȱ�����
     * @return �ɹ�����true��ʧ�ܷ���false
     */
    dbool put(Event *pEvent, dint priority = 0);

    /** 
     * ��handler�л��һ��event�����ô˽ӿڵĴ����Ӧ�ø���release event���̰߳�ȫ��
     * @param pEvent  �洢�¼�ָ���ָ��
     * @param iTimeoutInMilliSeconds  ��ʱʱ��
     * @return ��ȡ�¼��Ľ���������ö��Result�Ķ���
     */
    Result getAndRetain(Event** pEvent, duint iTimeoutInMilliSeconds);

    /** 
     * �ж�handler�Ƿ��ܽ����¼����̰߳�ȫ��
     * @return �ܼ������ܷ���false�����򷵻�true
     */
    dbool isFull() const;

    /** 
     * �ж��Ƿ����¼��ȴ��������̰߳�ȫ��
     * @return ���¼�����true�����򷵻�false
     */
    dbool hasEvent() const;

    /** 
     * �õ��ȴ���������¼����������̰߳�ȫ��
     * @return �¼�����
     */
    dint eventCount() const;

    /** 
     * ȡ���Զ��ͷţ��Ӷ������ȡ���ö���ע�⣬���ø÷�������release�ö�����Ҫ�Լ�release
     * @return ���ظ�handler�Ļ�����
     */
    Mutex& mutex();
};
DM_END_NAMESPACE
#endif //DMEVENTHANDLER_H
