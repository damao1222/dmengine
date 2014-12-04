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
#ifndef DMCOREAPPLICATION_H
#define DMCOREAPPLICATION_H
#include "dmnamespace.h"
DM_BEGIN_NAMESPACE
class Object;
class Event;
class EventFilter;
class Application;
DM_PRIVATE_CLASS(CoreApplication);
class DM_DLL_EXPORT CoreApplication
{
    DM_DECLARE_PRIVATE(CoreApplication)
public:
    CoreApplication();
    virtual ~CoreApplication();

    /** 
     * �ж��Ƿ��������߳�
     * @return �����̷߳���true�����򷵻�false
     */
    static dbool isMainThread();

    /** 
     * ��Object����һ���¼���ͬ����
     * @param obj  �¼����͵Ķ���
     * @param e  ��Ҫ���͵��¼�
     * @return ���Objectϣ�����¼�������������true�����򷵻�false
     */
    static dbool sendEvent(Object *obj, Event *e);

    /** 
     * ��Object����һ���¼���������ô˷����������̣߳���ֱ�ӷ����¼���Ч����sendEventһ��
     * �����������ز������̻߳�ÿ���Ȩ�����event��
     * @param obj  �¼����͵Ķ���
     * @param e  ��Ҫ���͵��¼�
     * @param priority  ��������ȼ�
     */
    static void postEvent(Object *obj, Event *e, dint priority = 0);

    /** 
     * Ӧ�õ���
     * @return ����ָ��
     */
    static CoreApplication* global();

    /** 
     * ���һ���¼����������¼����������¼���Ҫ���ݵĶ��������յ��¼������¼����д���
     * @param filter  �¼�������
     * @return ������¼��������Ѿ���ӣ�����false�����򷵻�true
     */
    static dbool addEventFilter(EventFilter *filter);

    /** 
     * �Ƴ�һ���¼�������
     * @param filter  �¼�������
     * @return ������¼��������Ѿ���ӣ�����˳���Ƴ�������true�����򷵻�false
     */
    static dbool removeEventFilter(EventFilter *filter);

    /** 
     * ����application��frameMove
     */
    void start();

    /** 
     * ʹapplication��frameMoveʧЧ
     */
    void stop();

    /** 
     * ʹapplication��frameMove��ʱʧЧ
     * @param timeout  frameMove��ʱʧЧ��ʱ��
     */
    void pause(float timeout);

    /** 
     * Ӧ���Ƿ�ֹͣ��frameMove�Ƿ�ʧЧ��
     * @return ���Ӧ��ֹͣ���򷵻�true�����򷵻�false
     */
    dbool isStopped() const;

    /** 
     * Ӧ���Ƿ�ֹͣ��frameMove�Ƿ�ʧЧ��
     * @return ���Ӧ��û�б�ֹͣ����ͣ���򷵻�true�����򷵻�false
     */
    dbool isRunning() const;

    /** 
     * Ӧ���Ƿ���ͣ��frameMove�Ƿ�ʧЧ��
     * @return ���Ӧ�ñ���ͣ���򷵻�true�����򷵻�false
     */
    dbool isPaused() const;

    /** 
     * ����Ӧ��ʱ��Ƭ���ƶ�Ӧ����LiveObject��ʱ������Ӱ��
     * ע�⣺ÿ�ε��øýӿڶ����ͷ�AutoReleaseManager�й����auto release����
     * @param interval  ʱ��Ƭ�ĳ��ȣ��ǵ�һ��ʱ��Ƭ�����ϴε���frameMove����ε���frameMove��ʱ����
     */
    void frameMove(float interval);

    /** 
     * ����Ӧ��ѭ������ѭ��Ϊ���̣߳��������Ҫ��ѭ�������frameMoveˢ��Ӧ��
     */
    dint loop();

    /** 
     * �˳�Ӧ��ѭ��
     */
    void quit();

    //internal
    friend class Application;
protected:
    CoreApplication(CoreApplicationPrivate *data);
    virtual void onframeMove(float interval) { DM_UNUSED(interval); }

    static CoreApplication *self;
};
DM_END_NAMESPACE
#endif //DMCOREAPPLICATION_H
