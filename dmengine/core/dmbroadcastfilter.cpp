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
#include "dmbroadcastfilter.h"
#include "dmvector.h"
#include "dmutilstring.h"
#include "private/dmbroadcast_p.h"
#include <stdarg.h>
DM_BEGIN_NAMESPACE
BroadcastFilter::BroadcastFilter():
    C_D(BroadcastFilter)
{
}

BroadcastFilter::~BroadcastFilter()
{
    D_D(BroadcastFilter);
}

void BroadcastFilter::addAction(const UtilString &action)
{
    if (!action.isEmpty() && pdm->actions.indexOf(action) == -1)
    {
        pdm->actions.append(action);
    }
}

dint BroadcastFilter::actionSize() const
{
    return pdm->actions.size();
}

const UtilString& BroadcastFilter::actionAt(dint index) const
{
    return pdm->actions.at(index);
}

BroadcastFilter &BroadcastFilter::operator=(const BroadcastFilter &other)
{
    pdm->actions = other.pdm->actions;
    return *this;
}

BroadcastFilter* BroadcastFilter::create(const char *action, ...)
{
    BroadcastFilter *filter = new BroadcastFilter;
    va_list params;
    va_start(params, action);

    const char *tmp = action;
    while (tmp)
    {
        filter->addAction(tmp);
        tmp = va_arg(params, const char*);
    }

    va_end(params);
    
    return filter;
}

DM_END_NAMESPACE
