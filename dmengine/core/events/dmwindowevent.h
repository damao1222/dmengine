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
#ifndef DMWINDOWEVENT_H
#define DMWINDOWEVENT_H

#include "dmevent.h"

DM_BEGIN_NAMESPACE
enum{
    eFocusInEvent = eWindowBeginType + 1,
    eFocusOutEvent,
    eShowEvent,
    eCloseEvent,
    eVisibleChangeEvent,
};

class FocusEvent : public Event
{
public:
    FocusEvent(dint type) : focusEvent((EventType)type)
    {}

    virtual EventType type() const { return focusEvent; }

    inline dbool gotFocus() const { return type() == (EventType)eFocusInEvent; }
    inline dbool lostFocus() const { return type() == (EventType)eFocusOutEvent; }
    
private:
    EventType focusEvent;
};

class ShowEvent : public Event
{
public:
    virtual EventType type() const { return (EventType)eShowEvent; }    
};

class CloseEvent : public Event
{
public:
    virtual EventType type() const { return (EventType)eCloseEvent; }    
};

class VisibleChangeEvent : public Event
{
public:
    VisibleChangeEvent(dbool v): visible(v) {}

    virtual EventType type() const { return (EventType)eVisibleChangeEvent; }    

    dbool visible;
};
DM_END_NAMESPACE
#endif //DMWINDOWEVENT_H
