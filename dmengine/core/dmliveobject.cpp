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

#include "dmliveobject.h"
#include "private/dmliveobject_p.h"
#include "dmapplication.h"
#include "dmlogger.h"

DM_BEGIN_NAMESPACE
LiveObject::LiveObject():
    C_D(LiveObject)
{
}

LiveObject::LiveObject(LiveObjectPrivate *p):
    pdm(p)
{

}

LiveObject::~LiveObject()
{
    D_D(LiveObject);
}

dbool LiveObject::isAcceptInput() const
{
    return pdm->acceptInput;
}


DM_END_NAMESPACE
