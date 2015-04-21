/*
   Copyright (C) 2012-2014 Xiongfa Li
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

#include "dmtimer_test.h"
#include "dmtimer.h"
#include "dmlogger.h"

DM_BEGIN_NAMESPACE

int g_count = 0;
Timer *g_timer;

class TimerTmp
{
    
public:
    void onTimeout(TimeEvent *event)
    {
        ++g_count;
        if (g_count > 10)
        {
            g_timer->stop();
        }
        DM_LOGI("time out ! id:%d time:%g", event->id, event->interval);
    }
};

TimerTmp *g_tmp;

dbool TimerTest::init()
{
    g_tmp = new TimerTmp();
    g_timer = new Timer(TIMER_CLASS_CALLBACK(TimerTmp, onTimeout), g_tmp);
    return true;
}

void TimerTest::cleanup()
{
    delete g_tmp;
    delete g_timer;
}

void TimerTest::run()
{
    g_timer->start(1, true);
}
DM_END_NAMESPACE

