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

#ifndef DMEVENTFILTER_H
#define DMEVENTFILTER_H

#include "dmnamespace.h"
DM_BEGIN_NAMESPACE
class Object;
class Event;
class EventFilter
{
public:
    /** 
     * 事件过滤器，注册到Application，用于过滤事件
     * @param obj  事件分发的目标对象
     * @param e  分发的事件
     * @return 返回true表示丢弃该事件，返回false表示系统可以继续分发和处理该事件
     */
    virtual dbool filter(Object *obj, Event *e) { DM_UNUSED(obj); DM_UNUSED(e); return false; }
};
DM_END_NAMESPACE
#endif // DMEVENTFILTER_H
