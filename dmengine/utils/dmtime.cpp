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
#include "dmtime.h"

#include <time.h>
#include <sys/timeb.h>
#include <stdint.h>

#ifdef DM_OS_WIN
#ifndef DM_OS_WP8
#include <winsock2.h>
#endif
#else
#include <sys/select.h>
#endif

#ifdef DM_OS_WIN
#   include <windows.h>
#else
#   include <sys/time.h>
#endif

#ifdef DM_OS_WIN
#if 0
int
gettimeofday(struct timeval *tp, void *tzp)
{
    time_t clock;
    struct tm tm;
    SYSTEMTIME wtm;

    GetLocalTime(&wtm);
    tm.tm_year     = wtm.wYear - 1900;
    tm.tm_mon     = wtm.wMonth - 1;
    tm.tm_mday     = wtm.wDay;
    tm.tm_hour     = wtm.wHour;
    tm.tm_min     = wtm.wMinute;
    tm.tm_sec     = wtm.wSecond;
    tm. tm_isdst    = -1;
    clock = mktime(&tm);
    tp->tv_sec = clock;
    tp->tv_usec = wtm.wMilliseconds * 1000;

    return (0);
}
#endif
#endif

DM_USING_NAMESPACE;

duint32 Time::getSecond()
{
    return (duint32)::time(0);
}

duint32 Time::getMillisecond()
{
    timeb timebTmp;
    ::ftime(&timebTmp);
    return (duint32)timebTmp.time*1000 + timebTmp.millitm;
}

duint32 Time::getMicrosecond()
{
#ifdef DM_OS_WIN
    SYSTEMTIME wtm;
    ::GetLocalTime(&wtm);
    return wtm.wMilliseconds * 1000;
#else
    struct timeval tpTime;
    ::gettimeofday(&tpTime,NULL);
    return tpTime.tv_usec;
#endif
}

#ifdef DM_OS_WIN
#pragma comment(lib, "winmm.lib ")
#endif
duint Time::getAppClockMillis()
{
    uint64_t now_time;
    static uint64_t start_time = 0;
    static dbool start_time_set = false;
#if defined(DM_OS_DARWIN)
    now_time = CVGetCurrentHostTime() *  1000 / CVGetHostClockFrequency();
#elif defined(DM_OS_WIN)
    now_time = (uint64_t)timeGetTime();
#else
    struct timespec ts = {};
    clock_gettime(CLOCK_MONOTONIC, &ts);
    now_time = (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
#endif
    if (!start_time_set)
    {
        start_time = now_time;
        start_time_set = true;
    }

    return (duint)(now_time - start_time);
}

