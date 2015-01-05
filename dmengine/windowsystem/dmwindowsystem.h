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
#ifndef DMWINDOWSYSTEM_H
#define DMWINDOWSYSTEM_H
#include "dmobject.h"
#include "dmsingleton.h"
DM_BEGIN_NAMESPACE
class KeyEvent;
class TouchEvent;
class Widget;
class DisplayLayer;
DM_PRIVATE_CLASS(WindowSystem);
class DM_DLL_EXPORT WindowSystem : public Object
{
    DM_DECLARE_PRIVATE(WindowSystem)
public:
    WindowSystem();
    virtual ~WindowSystem();

    dbool activeWindow(Widget *widget);
    dbool deactiveWindow(Widget *widget);
    dbool isActived(Widget *widget) const;

    void pushDisplayLayer(DisplayLayer *layer);
    void popDisplayLayer();

    void handleKeyEvent(KeyEvent *e);
    void handleTouchEvent(TouchEvent *e);

    //internal
    virtual dbool event(Event *e);

    /** 
     * ���WindowSystem�ĵ��������������������newһ��WindowSystem�����̰߳�ȫ��.
     * @return ����Logger����.
     */
    static WindowSystem* getInstance();

    /** 
     * ���WindowSystem�ĵ���������������ڷ��ص���ָ�룬���򷵻�NULL.
     * @return ����WindowSystem����.
     */
    static WindowSystem* instance();

    /** 
     * �ͷ�WindowSystem�ĵ������̰߳�ȫ��.
     */
    static void releaseInstance();
};

#define g_windowSystem DM_GLOBAL_OBJ(WindowSystem)
DM_END_NAMESPACE
#endif // DMWINDOWSYSTEM_H
