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

#include "dmpostevent.h"
#include "dmobject.h"
DM_BEGIN_NAMESPACE

PostEvent::PostEvent(Object *obj, Event *ev)
{
    DM_ASSERT(obj && e);

    obj->retain();
    ev->retain();

    o = obj;
    e = ev;
}

PostEvent::PostEvent(const PostEvent &other)
{
    other.o->retain();
    other.e->retain();
    o = other.o;
    e = other.e;
}

PostEvent::~PostEvent()
{
    DM_SAFE_RELEASE(o);
    DM_SAFE_RELEASE(e);
}

PostEvent& PostEvent::operator=(const PostEvent &other)
{
    DM_SAFE_RELEASE(o);
    DM_SAFE_RELEASE(e);

    other.o->retain();
    other.e->retain();
    o = other.o;
    e = other.e;

    return *this;
}
DM_END_NAMESPACE
