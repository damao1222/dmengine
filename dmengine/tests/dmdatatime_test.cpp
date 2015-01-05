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

#include "dmdatatime_test.h"
#include "dmdatetime.h"
#include "dmlogger.h"

DM_BEGIN_NAMESPACE

dbool DateTimeTest::init()
{
   return true;
}

void DateTimeTest::cleanup()
{

}

void DateTimeTest::run()
{
	DateTime dt = DateTime::currentDateTime();
    Date d = dt.date();
    Time t = dt.time();
    DM_LOGI("date : %02d - %02d - %02d", d.year(), d.month(), d.day());
    DM_LOGI("time : %02d : %02d : %02d", t.hour(), t.minute(), t.second());

    d = Date::currentDate();
    t = Time::currentTime();
    DM_LOGI("date : %02d - %02d - %02d", d.year(), d.month(), d.day());
    DM_LOGI("time : %02d : %02d : %02d", t.hour(), t.minute(), t.second());
}
DM_END_NAMESPACE

