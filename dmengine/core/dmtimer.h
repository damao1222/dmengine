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

#ifndef TIMER_H
#define TIMER_H
#include "private/dmliveobject_p.h"
#include "dmevent.h"

DM_BEGIN_NAMESPACE
class Application;
class DM_DLL_EXPORT Timer
{
    DM_DECLARE_PRIVATE(Timer)
public:
    Timer();

    /** 
     * ���캯��
     * @param func  ��ʱ�ص�����
     * @param target ��ʱ�ص����Ӳ���  
     */
    explicit Timer(TimerCallbackFunc func, void *param = NULL);

    /** 
     * ��������
     */
    ~Timer();

    /** 
     * ���ü�ʱ���ĳ�ʱʱ��
     * @param interval  ��ʱʱ��
     */
    void setInterval(dreal interval);

    /** 
     * ���ü�ʱ���Ƿ��ظ���Ĭ��Ϊ���ظ�
     * @param flag  �ظ�����Ϊtrue����������Ϊfalse
     */
    void setRepeat(dbool flag);

    /** 
     * ��ü�ʱ����ʱ��ص��Ĵ���
     * @return �ظ�����
     */
    dint repeatCount() const;

    /** 
     * ��øü�ʱ��ID
     * @return ��ʱ����ID
     */
    dint id() const;

    /** 
     * �ж��ü�ʱ���Ƿ����ڼ�ʱ
     * @return ���ڼ�ʱ����true�����򷵻�false
     */
    dbool isTiming() const;

    /** 
     * ��ü�ʱ��ʣ���ʱ��
     * @return ��ʱ��ʣ��ʱ��
     */
    dreal remainTime() const;

    /** 
     * ������ʱ��
     * @param interval  ��ʱʱ�䣬����Ϊ��λ
     * @param repeat  �Ƿ�һֱ�ظ�
     */
    void start(dreal interval, dbool repeat = false);

    /** 
     * ������ʱ������ʱʱ�����setInterval�趨
     */
    void start();

    /** 
     * ���ü�ʱ������ʱ�������¿�ʼ��ʱ������ʱ���Ѿ���ʱ��ʧ��
     * @return ����ɹ�����true�����򷵻�false
     */
    dbool reset();

    /** 
     * ֹͣ��ʱ
     */
    void stop();

private:
    friend class TimerProxy;
    friend class Application;
    DM_DISABLE_COPY(Timer)
};

template<class T, void(T::*fn)(TimeEvent *event)>
void TimerCallback(void *obj, TimeEvent *event)
{
    (static_cast<T*>(obj)->*fn)(event);
}

#define TIMER_CLASS_CALLBACK(CLASS, CLASSFUNC) \
            TimerCallback<CLASS, &CLASS::CLASSFUNC>
/*
 * This Template is useful, for example:
    class Test
    {
    public:
        Test()
        {
            timer = new Timer(TimerCallback<Test, &Test::onMyTimeout>, this);

            //Test::onMyTimeout will be called after 0.1 second
            timer->start(0.1f);
        }

        ~Test()
        {
            timer->release();
        }

        void onMyTimeout(TimeEvent *event) 
        {
            //do something
        }

    private:
        Timer *timer;
    }
 */
DM_END_NAMESPACE
#endif // TIMER_H
