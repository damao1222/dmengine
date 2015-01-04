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
     * 使用默认config初始化应用
     */
    void init();

    /**
     * 使用config初始化应用
     * @param config  应用配置
     */
    void init(const AppConfig &config);

    /** 
     * 设置应用分辨率
     * @param res  分辨率大小
     */
    void setResolution(const Size &res);

    /** 
     * 获得应用当前的分辨率
     * @return 分辨率
     */
    const Size& resolution() const;

    /** 
     * 获得应用分辨率的宽度
     * @return 宽度
     */
    dint width() const;

    /** 
     * 获得应用分辨率的高度
     * @return 高度
     */
    dint height() const;

    /** 
     * Activate the LiveObject
     * 激活LiveObject，应用中需要时间因素的Object都应该激活
     * @param obj  The Object witch need to be activated
     * @return Return true if Success else return false
     */
    dbool activateObject(LiveObject *obj);

    /** 
     * Deactivate the LiveObject
     * 停止LiveObject，当不再需要时间因素时调用，但不推荐调用此方法，
     * 如果只是暂时停止时间因素对Object的影响，可以调用LiveObject->stop代替。
     * @param obj  The Object witch need to be inactivated
     * @return Return true if Success else return false
     */
    dbool deactivateObject(LiveObject *obj);

    /** 
     * Deactivate the LiveObject
     * 停止所有LiveObject，使之不再受时间因素影响
     */
    void deactivateAll();

    /** 
     * 在应用内广播消息
     * @param action  广播的action，对应包含该action的filter的接收器将接收到广播
     * @param msg  广播的数据
     * @return 成功返回true，否则返回false
     */
    dbool sendBroadcast(const UtilString &action, const Variant &msg);

    /** 
     * 在应用内广播消息
     * @param filter  广播的filter，对应filter的接收器将接收到广播
     * @param msg  广播的数据
     * @return 成功返回true，否则返回false
     */
    dbool sendBroadcast(const BroadcastFilter &filter, const Variant &msg);

    /** 
     * 获得Application的单例，如果没有则创建.
     * @return 单例.
     */
    static Application* getInstance();

    /** 
     * 获得Application的单例，如果没有则返回NULL.
     * @return 单例
     */
    static Application* instance();

    /** 
     * 释放Application的单例.
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
