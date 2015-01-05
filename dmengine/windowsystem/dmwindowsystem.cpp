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
#include "dmwindowsystem.h"
#include "dmwidget.h"
#include "dmrefarray.h"
#include "dmcoreapplication.h"
#include "events/dminputevent.h"
#include "events/dmwindowevent.h"
#include "dmdisplaylayer.h"
#include "dmdllsingleton.h"

DM_BEGIN_NAMESPACE
class WindowSystemPrivate
{
public:
    void addWidget(Widget *w);
    void removeWidget(Widget *w);

    RefArray *arr;
    RefArray *layerArr;

    DM_DECLARE_SINGLETON(WindowSystem);
};

DM_INTERNAL_SINGLETON(WindowSystem)

void WindowSystemPrivate::addWidget(Widget *w)
{
    if (w && !layerArr->isEmpty())
    {
        DisplayLayer *layer = (DisplayLayer*)layerArr->last();
        layer->add(w);
    }
}

void WindowSystemPrivate::removeWidget(Widget *w)
{
    if (w && !layerArr->isEmpty())
    {
        DisplayLayer *layer = (DisplayLayer*)layerArr->last();
        layer->remove(w);
    }
}

WindowSystem::WindowSystem():
    C_D(WindowSystem)
{
    pdm->arr = new RefArray;
    pdm->layerArr = new RefArray;
}

WindowSystem::~WindowSystem()
{
    DM_SAFE_DELETE(pdm->arr);
    DM_SAFE_DELETE(pdm->layerArr);
    D_D(WindowSystem);
}

dbool WindowSystem::activeWindow(Widget *widget)
{
    dint i = pdm->arr->indexOf(widget);
    
    //已经是最顶层窗体，拥有焦点
    if (!pdm->arr->isEmpty() && i == pdm->arr->size() - 1)
    {
        return false;
    }

    widget->retain();

    //是否已经添加到窗口系统
    dbool bAdded = false;
    //如果该窗体已在队列，则先移出队列
    if (i != -1)
    {
        bAdded = true;
        pdm->arr->removeAt(i);
    }

    //最后一个widget失去焦点，发送事件通知
    if (!pdm->arr->isEmpty())
    {
        Widget* old = (Widget*)pdm->arr->last();
        FocusEvent *out = new FocusEvent(eFocusOutEvent);
        CoreApplication::sendEvent(old, out);
        out->release();
    }

    //添加到末尾
    pdm->arr->addObject(widget);

    //将widget注册到display layer
    if (!bAdded)
    {
        pdm->addWidget(widget);
    }

    //发送焦点事件
    FocusEvent *in = new FocusEvent(eFocusInEvent);
    CoreApplication::sendEvent(widget, in);
    in->release();

    widget->release();
    return true;
}

dbool WindowSystem::deactiveWindow(Widget *widget)
{
    dint i = pdm->arr->lastIndexOf(widget);
    
    //已经是最顶层窗体
    if (!pdm->arr->isEmpty() && i == pdm->arr->size() - 1)
    {
        //发送失去焦点的事件
        FocusEvent *out = new FocusEvent(eFocusOutEvent);
        CoreApplication::sendEvent(widget, out);
        out->release();

        //将widget从display layer移出
        pdm->removeWidget(widget);
        
        pdm->arr->removeLast();

        //新顶点窗体获得焦点
        if (!pdm->arr->isEmpty())
        {
            Widget* last = (Widget*)pdm->arr->last();
            FocusEvent *in = new FocusEvent(eFocusInEvent);
            CoreApplication::sendEvent(last, in);
            in->release();
        }

        return true;
    }

    //如果该窗体已在队列，则移出队列
    if (i != -1)
    {
        //pdm->arr->removeAt(i);
        //return true;
    }
    return false;
}

dbool WindowSystem::isActived(Widget *widget) const
{
    return (pdm->arr->last() == widget);
}

void WindowSystem::pushDisplayLayer(DisplayLayer *layer)
{
    if (layer)
    {
        pdm->layerArr->addObject(layer);
    }
}

void WindowSystem::popDisplayLayer()
{
    pdm->layerArr->removeLast();
}

void WindowSystem::handleKeyEvent(KeyEvent *e)
{
    if (!pdm->arr->isEmpty())
    {
        Widget* w = (Widget*)pdm->arr->last();
        CoreApplication::sendEvent(w, e);
    }
}

void WindowSystem::handleTouchEvent(TouchEvent *e)
{
    if (!pdm->arr->isEmpty())
    {
        Widget* w = (Widget*)pdm->arr->last();
        CoreApplication::sendEvent(w, e);
    }
}

dbool WindowSystem::event(Event *e)
{
    switch (e->type())
    {    
    case eShowEvent:
        {
            activeWindow((Widget*)e->sender);
            //widget收到事件
            sendEvent(e->sender, e);
            return true;
        }
    case eCloseEvent:
        {
            //widget收到事件
            sendEvent(e->sender, e);
            deactiveWindow((Widget*)e->sender);
            return true;
        }
    default:
        break;
    }

    return Object::event(e);
}
DM_END_NAMESPACE
