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

#ifndef DMAPPLICATION_H
#define DMAPPLICATION_H

#include "dmsingleton.h"
#include "dmcoreapplication.h"
#include "input/dmtouchdispatcher.h"
#include "dmsize.h"

DM_BEGIN_NAMESPACE
class Variant;
class UtilString;
class BroadcastFilter;
class LiveObject;
class AppConfig;
class DisplayLayer;
DM_PRIVATE_CLASS(Application);
class DM_DLL_EXPORT Application: public CoreApplication, public TouchDispatcher
{
    DM_DECLARE_PRIVATE_N(Application)
public:
    /**
     * Default constructor
     */
    Application();
    /**
     * Default destructor
     */
    ~Application();

    /**
     * ʹ��Ĭ��config��ʼ��Ӧ��
     */
    void init();

    /**
     * ʹ��config��ʼ��Ӧ��
     * @param config  Ӧ������
     */
    void init(const AppConfig &config);

    /** 
     * ����Ӧ�÷ֱ���
     * @param res  �ֱ��ʴ�С
     */
    void setResolution(const Size &res);

    /** 
     * ���Ӧ�õ�ǰ�ķֱ���
     * @return �ֱ���
     */
    const Size& resolution() const;

    /** 
     * ���Ӧ�÷ֱ��ʵĿ��
     * @return ���
     */
    dint width() const;

    /** 
     * ���Ӧ�÷ֱ��ʵĸ߶�
     * @return �߶�
     */
    dint height() const;

    /** 
     * Activate the LiveObject
     * ����LiveObject��Ӧ������Ҫʱ�����ص�Object��Ӧ�ü���
     * @param obj  The Object witch need to be activated
     * @return Return true if Success else return false
     */
    dbool activateObject(LiveObject *obj);

    /** 
     * Deactivate the LiveObject
     * ֹͣLiveObject����������Ҫʱ������ʱ���ã������Ƽ����ô˷�����
     * ���ֻ����ʱֹͣʱ�����ض�Object��Ӱ�죬���Ե���LiveObject->stop���档
     * @param obj  The Object witch need to be inactivated
     * @return Return true if Success else return false
     */
    dbool deactivateObject(LiveObject *obj);

    /** 
     * Deactivate the LiveObject
     * ֹͣ����LiveObject��ʹ֮������ʱ������Ӱ��
     */
    void deactivateAll();

    /** 
     * ��Ӧ���ڹ㲥��Ϣ
     * @param action  �㲥��action����Ӧ������action��filter�Ľ����������յ��㲥
     * @param msg  �㲥������
     * @return �ɹ�����true�����򷵻�false
     */
    dbool sendBroadcast(const UtilString &action, const Variant &msg);

    /** 
     * ��Ӧ���ڹ㲥��Ϣ
     * @param filter  �㲥��filter����Ӧfilter�Ľ����������յ��㲥
     * @param msg  �㲥������
     * @return �ɹ�����true�����򷵻�false
     */
    dbool sendBroadcast(const BroadcastFilter &filter, const Variant &msg);

    /** 
     * ���Application�ĵ��������û���򴴽�.
     * @return ����.
     */
    static Application* getInstance();

    /** 
     * ���Application�ĵ��������û���򷵻�NULL.
     * @return ����
     */
    static Application* instance();

    /** 
     * �ͷ�Application�ĵ���.
     */
    static void releaseInstance();

    void pushDisplayLayer(DisplayLayer *layer);
    void popDisplayLayer();

    void setTouchDispatcher(TouchNotifier *dispatcher);
    void processTouchEvent(TouchEvent *e);
    void processKeyEvent(KeyEvent *e);

protected:
    /**
     * Call when application exit
     */
    DM_INTERNAL_FUNC(void, final, ());
    
    virtual void onframeMove(float interval);
    
private:
    DM_DISABLE_COPY(Application)
};

DM_END_NAMESPACE
#define g_application DM_GLOBAL_OBJ(DM_NS::Application)
#define dmApp (*DM_NS::Application::getInstance())
#endif // DMAPPLICATION_H
