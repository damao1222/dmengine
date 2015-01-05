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

#include "dminteractiveobject.h"
#include "events/dminputevent.h"
#include "private/dmliveobject_p.h"

DM_BEGIN_NAMESPACE
class InteractiveObjectPrivate : public LiveObjectPrivate
{
public:
    dbool touchable;
};

InteractiveObject::InteractiveObject():
    LiveObject(new InteractiveObjectPrivate)
{
    d_func()->acceptInput = true;
    d_func()->touchable = false;    
}

InteractiveObject::~InteractiveObject()
{
//    D_D(InteractiveObject);
}

dbool InteractiveObject::isTouchable() const
{
    return static_cast<InteractiveObjectPrivate*>(pdm)->touchable;
}

void InteractiveObject::setTouchable(dbool flag)
{
    d_func()->touchable = flag;
}

dbool InteractiveObject::event(Event *e)
{
    switch (e->type())
    {    
    case eKeyPressEvent:
        {
            return onKeyPress(static_cast<KeyEvent*>(e));
        }
    case eKeyReleaseEvent:
        {
            return onKeyRelease(static_cast<KeyEvent*>(e));
        }
    case eTouchBeganEvent:
        {
            return onTouchBegan(static_cast<TouchEvent*>(e));
        }
    case eTouchMovedEvent:
        {
            return onTouchMoved(static_cast<TouchEvent*>(e));
        }
    case eTouchEndedEvent:
        {
            return onTouchEnded(static_cast<TouchEvent*>(e));
        }
    default:
        break;
    }

    return LiveObject::event(e);
}

dbool InteractiveObject::onKeyPress(KeyEvent *e)
{
    DM_UNUSED(e);
    return false;
}

dbool InteractiveObject::onKeyRelease(KeyEvent *e)
{
    DM_UNUSED(e);
    return false;
}

dbool InteractiveObject::onTouchBegan(TouchEvent *e)
{
    DM_UNUSED(e);
    return false;
}

dbool InteractiveObject::onTouchMoved(TouchEvent *e)
{
    DM_UNUSED(e);
    return false;
}

dbool InteractiveObject::onTouchEnded(TouchEvent *e)
{
    DM_UNUSED(e);
    return false;
}

DM_END_NAMESPACE
