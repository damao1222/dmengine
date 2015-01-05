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
#include "dmendtime.h"
#include "dmtime.h"
#include <limits.h>

DM_BEGIN_NAMESPACE
class EndTimePrivate
{
public:
#ifdef DM_NO_THREAD
    duint totleTime;
#else
    volatile duint totleTime;
#endif
    duint beginTime;
};

EndTime::EndTime(duint totleTime):
    C_D(EndTime)
{
    pdm->totleTime = totleTime;
    pdm->beginTime = Time::getAppClockMillis();
}

EndTime::~EndTime()
{
    D_D(EndTime);
}

void EndTime::resetTime()
{
    pdm->beginTime = Time::getAppClockMillis();
}

void EndTime::resetTime(duint totleTime)
{
    pdm->totleTime = totleTime;
    pdm->beginTime = Time::getAppClockMillis();
}

void EndTime::setExpire()
{
    pdm->totleTime = 0;
}

void EndTime::setInfinite()
{
    pdm->totleTime = UINT_MAX;//std::numeric_limits<unsigned int>::max();
}

dbool EndTime::isExpired() const
{
    if (pdm->totleTime == 0)
        return true;
    else if (isInfinite())
        return false;
    else if (pastTime() >= pdm->totleTime)
        return true;
   
    return false;
}

dbool EndTime::isInfinite() const
{
    return pdm->totleTime == UINT_MAX;
}

duint EndTime::leftTime() const
{
    if (isInfinite())
        return UINT_MAX;
    
    duint past = pastTime();
    return past >= pdm->totleTime ? 0 : pdm->totleTime - past;
}

duint EndTime::pastTime() const
{
    return Time::getAppClockMillis() - pdm->beginTime;
}
DM_END_NAMESPACE
