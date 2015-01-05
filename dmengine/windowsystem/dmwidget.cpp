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
#include "dmwidget.h"
#include "events/dminputevent.h"
#include "events/dmwindowevent.h"
#include "dmapplication.h"
#include "dmwindowsystem.h"

DM_BEGIN_NAMESPACE
class WidgetPrivate
{
public:
    WindowSystem *ws;
};

Widget::Widget() :
    C_D(Widget)
{
    pdm->ws = &g_windowSystem;
}

Widget::~Widget()
{
    close();
    D_D(Widget);
}

void Widget::show()
{
    ShowEvent *e = new ShowEvent;
    e->sender = this;
    CoreApplication::postEvent(pdm->ws, e);
    e->release();

    setVisible(true);
}

void Widget::close()
{
    CloseEvent *e = new CloseEvent;
    e->sender = this;
    CoreApplication::postEvent(pdm->ws, e);
    e->release();

    setVisible(false);
}

void Widget::setVisible(dbool visible)
{
    DM_UNUSED(visible);
}

dbool Widget::isVisible() const
{
    return false;
}

void Widget::requestFocus()
{
    pdm->ws->activeWindow(this);
}

dbool Widget::hasFocus() const
{
    return pdm->ws->isActived(const_cast<Widget*>(this));
}

dbool Widget::event(Event *e)
{
    switch (e->type())
    {    
    case eFocusInEvent:
        {
            onFocusInEvent(static_cast<FocusEvent*>(e));
            return true;
        }
    case eFocusOutEvent:
        {
            onFocusOutEvent(static_cast<FocusEvent*>(e));
            return true;
        }
    case eShowEvent:
        {
            onShowEvent(static_cast<ShowEvent*>(e));
            return true;
        }
    case eCloseEvent:
        {
            onCloseEvent(static_cast<CloseEvent*>(e));
            return true;
        }
    default:
        break;
    }

    return InteractiveObject::event(e);
}

void Widget::onFocusInEvent(FocusEvent *e)
{
    DM_UNUSED(e);
}

void Widget::onFocusOutEvent(FocusEvent *e)
{
    DM_UNUSED(e);
}

void Widget::onShowEvent(ShowEvent *e)
{
    DM_UNUSED(e);
}

void Widget::onCloseEvent(CloseEvent *e)
{
    DM_UNUSED(e);
}

DM_END_NAMESPACE
