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
#include "dmdatetime.h"

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

DM_BEGIN_NAMESPACE
enum {
        FIRST_YEAR = -4713,
        FIRST_MONTH = 1,
        FIRST_DAY = 2,  // ### Qt 5: make FIRST_DAY = 1, by support jd == 0 as valid
        SECS_PER_DAY = 86400,
        MSECS_PER_DAY = 86400000,
        SECS_PER_HOUR = 3600,
        MSECS_PER_HOUR = 3600000,
        SECS_PER_MIN = 60,
        MSECS_PER_MIN = 60000,
        JULIAN_DAY_FOR_EPOCH = 2440588 // result of julianDayFromGregorianDate(1970, 1, 1)
};

static inline duint JulianDayFromGregorianDate(dint year, dint month, dint day)
{
    // Gregorian calendar starting from October 15, 1582
    // Algorithm from Henry F. Fliegel and Thomas C. Van Flandern
    return (1461 * (year + 4800 + (month - 14) / 12)) / 4
        + (367 * (month - 2 - 12 * ((month - 14) / 12))) / 12
        - (3 * ((year + 4900 + (month - 14) / 12) / 100)) / 4
        + day - 32075;
}

static duint JulianDayFromDate(dint year, dint month, dint day)
{
    if (year < 0)
        ++year;

    if (year > 1582 || (year == 1582 && (month > 10 || (month == 10 && day >= 15)))) {
        return JulianDayFromGregorianDate(year, month, day);
    } else if (year < 1582 || (year == 1582 && (month < 10 || (month == 10 && day <= 4)))) {
        // Julian calendar until October 4, 1582
        // Algorithm from Frequently Asked Questions about Calendars by Claus Toendering
        int a = (14 - month) / 12;
        return (153 * (month + (12 * a) - 3) + 2) / 5
            + (1461 * (year + 4800 - a)) / 4
            + day - 32083;
    } else {
        // the day following October 4, 1582 is October 15, 1582
        return 0;
    }
}

static inline duint MsecsFromDecomposed(dint hour, dint minute, dint sec, dint msec = 0)
{
    return MSECS_PER_HOUR * hour + MSECS_PER_MIN * minute + 1000 * sec + msec;
}

static void GetDateFromJulianDay(duint julianDay, dint *year, dint *month, dint *day)
{
    int y, m, d;

    if (julianDay >= 2299161) {
        // Gregorian calendar starting from October 15, 1582
        // This algorithm is from Henry F. Fliegel and Thomas C. Van Flandern
        dulonglong ell, n, i, j;
        ell = dulonglong(julianDay) + 68569;
        n = (4 * ell) / 146097;
        ell = ell - (146097 * n + 3) / 4;
        i = (4000 * (ell + 1)) / 1461001;
        ell = ell - (1461 * i) / 4 + 31;
        j = (80 * ell) / 2447;
        d = ell - (2447 * j) / 80;
        ell = j / 11;
        m = j + 2 - (12 * ell);
        y = 100 * (n - 49) + i + ell;
    } else {
        // Julian calendar until October 4, 1582
        // Algorithm from Frequently Asked Questions about Calendars by Claus Toendering
        julianDay += 32082;
        int dd = (4 * julianDay + 3) / 1461;
        int ee = julianDay - (1461 * dd) / 4;
        int mm = ((5 * ee) + 2) / 153;
        d = ee - (153 * mm + 2) / 5 + 1;
        m = mm + 3 - 12 * (mm / 10);
        y = dd - 4800 + (mm / 10);
        if (y <= 0)
            --y;
    }
    if (year)
        *year = y;
    if (month)
        *month = m;
    if (day)
        *day = d;
}

class DatePrivate
{
public:
    duint jd;
};

class TimePrivate
{
public:
    enum { eInvalidTime = -1 };
    inline dint ms() const { return mc == eInvalidTime ? 0 : mc; }
    dint mc;
};


class DateTimePrivate
{
public:
    Date d;
    Time t;
};

Date::Date():
    C_D(Date)
{
    pdm->jd = 0;
}

Date::Date(const Date &other):
    C_D(Date)
{
    pdm->jd = other.pdm->jd;
}

Date::~Date()
{
    D_D(Date);
}

dint Date::year() const
{
    dint y;
    GetDateFromJulianDay(pdm->jd, &y, 0, 0);
    return y;
}

dint Date::month() const
{
    dint m;
    GetDateFromJulianDay(pdm->jd, 0, &m, 0);
    return m;
}

dint Date::day() const
{
    dint d;
    GetDateFromJulianDay(pdm->jd, 0, 0, &d);
    return d;
}

Date& Date::operator=(const Date &other)
{
    if (this == &other)
        return *this;

    pdm->jd = other.pdm->jd;
    return *this;
}

dbool Date::operator==(const Date &other) const
{
    if (this == &other)
        return true;

    return pdm->jd == other.pdm->jd;
}

dbool Date::operator!=(const Date &other) const
{
    return !(*this == other);
}

Time::Time():
    C_D(Time)
{
    pdm->mc = TimePrivate::eInvalidTime;
}

Time::Time(const Time &other):
    C_D(Time)
{
    pdm->mc = other.pdm->mc;
}

Time::~Time()
{
    D_D(Time);
}

dint Time::hour() const
{
    return pdm->ms() / MSECS_PER_HOUR;
}

dint Time::minute() const
{
    return (pdm->ms() % MSECS_PER_HOUR) / MSECS_PER_MIN;
}

dint Time::second() const
{
    return (pdm->ms() / 1000)%SECS_PER_MIN;
}

dint Time::msec() const
{
    return pdm->ms() % 1000;
}

Time& Time::operator=(const Time &other)
{
    if (this == &other)
        return *this;

    pdm->mc = other.pdm->mc;
    return *this;
}

dbool Time::operator==(const Time &other) const
{
    if (this == &other)
        return true;

    return pdm->mc == other.pdm->mc;
}

dbool Time::operator!=(const Time &other) const
{
    return !(*this == other);
}

DateTime::DateTime():
    C_D(DateTime)
{
   
}

DateTime::DateTime(const Date &date, const Time &time):
    C_D(DateTime)
{
    pdm->d = date;
    pdm->t = time;
}

DateTime::DateTime(const DateTime &other):
    C_D(DateTime)
{
    pdm->d = other.pdm->d;
    pdm->t = other.pdm->t;
}

DateTime::~DateTime()
{
    D_D(DateTime);
}

Date DateTime::date() const
{
    return pdm->d;
}

Time DateTime::time() const
{
    return pdm->t;
}

#if defined (DM_OS_WIN)
Date Date::currentDate()
{
    Date d;
    SYSTEMTIME st;
    memset(&st, 0, sizeof(SYSTEMTIME));
    GetLocalTime(&st);
    d.d_func()->jd = JulianDayFromDate(st.wYear, st.wMonth, st.wDay);
    return d;
}

Time Time::currentTime()
{
    Time ct;
    SYSTEMTIME st;
    memset(&st, 0, sizeof(SYSTEMTIME));
    GetLocalTime(&st);
    ct.d_func()->mc = MsecsFromDecomposed(st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    return ct;
}

DateTime DateTime::currentDateTime()
{
    Date d;
    Time t;
    SYSTEMTIME st;
    memset(&st, 0, sizeof(SYSTEMTIME));
    GetLocalTime(&st);
    d.d_func()->jd = JulianDayFromDate(st.wYear, st.wMonth, st.wDay);
    t.d_func()->mc = MsecsFromDecomposed(st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    return DateTime(d, t);
}

DateTime& DateTime::operator=(const DateTime &other)
{
    if (this == &other)
        return *this;

    pdm->d = other.pdm->d;
    pdm->t = other.pdm->t;

    return *this;
}

dbool DateTime::operator==(const DateTime &other) const
{
    if (this == &other)
        return true;

    return pdm->d == other.pdm->d && pdm->t == other.pdm->t;
}

dbool DateTime::operator!=(const DateTime &other) const
{
    return !(*this == other);
}

#elif defined (DM_OS_UNIX)
Date Date::currentDate()
{
    Date d;
    // posix compliant system
    time_t ltime;
    time(&ltime);
    struct tm *t = 0;

    t = localtime(&ltime);
    d.jd = JulianDayFromDate(t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
    return d;
}

Time Time::currentTime()
{
    Time ct;
    // posix compliant system
    struct timeval tv;
    gettimeofday(&tv, 0);
    time_t ltime = tv.tv_sec;
    struct tm *t = 0;

    t = localtime(&ltime);
    ct.mc = MsecsFromDecomposed(t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec / 1000);
    return ct;
}

DateTime DateTime::currentDateTime()
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    time_t ltime = tv.tv_sec;
    struct tm *t = 0;

    t = localtime(&ltime);

    DateTime dt;
    dt.pdm->t.mc = MsecsFromDecomposed(t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec / 1000);
    dt.pdm->d.jd = JulianDayFromDate(t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
    return dt;
}
#endif //OS ARCH
DM_END_NAMESPACE