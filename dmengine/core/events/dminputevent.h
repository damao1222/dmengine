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
#ifndef DMINPUTEVENT_H
#define DMINPUTEVENT_H

#include "dmevent.h"
#include "dmpoint.h"

DM_BEGIN_NAMESPACE
enum{
    eKeyPressEvent = eInputBeginType + 1,
    eKeyReleaseEvent,
    eTouchBeganEvent,
    eTouchMovedEvent,
    eTouchEndedEvent
};

class KeyEvent : public Event
{
public:
    KeyEvent(dint type) : keyType((EventType)type)
    {}

    virtual EventType type() const { return keyType; }

    dint keyCode;
private:
    EventType keyType;
};

class TouchEvent : public Event
{
public:
    TouchEvent(dint type) : touchType((EventType)type)
    {}

    virtual EventType type() const { return touchType; }
    
    Point pos;

private:
    EventType touchType;
};

DM_END_NAMESPACE
#endif //DMINPUTEVENT_H
