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

#include "dmutilstring_p.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "dmlogger.h"
#ifndef DM_CC_MSVC
#include <stdio.h>
#include <sstream>
#endif

DM_BEGIN_NAMESPACE
static int std_index_of(const StdStr &str, StdChar c, dint pos = 0)
{
    size_t found = str.find(c, pos);
    if (found != StdStr::npos)
        return static_cast<int>(found);
    else
        return -1;
}

static int std_index_of(const StdStr &str, const StdStr &s, dint pos = 0)
{
    size_t found = str.find(s, pos);
    if (found != StdStr::npos)
        return static_cast<int>(found);
    else
        return -1;
}
#ifdef DM_NO_STRING_SHARING
WStringData::Data WStringData::shared_null = {  };
#else
WStringData::Data WStringData::shared_null = { DM_BASIC_ATOMIC_INITIALIZER(1), true, };
#endif

WStringData::Data *WStringData::create(const StdChar *s, size_t n /*= 0*/)
{
#ifdef DM_NO_STRING_SHARING
    Data* t = new Data;

    if (s != NULL)
#ifdef DM_CC_MSVC
        t->s = (n == 0 ? s : StdStr(s, n));
#else
    	t->s = (n == 0 ? StdStr(s) : StdStr(s, n));
#endif
    return t;
#else
    DM_ASSERT(s);

    Data* t = NULL;
    DM_TRY
    {
        t = new Data;
        t->s = (n == 0 ? s : StdStr(s, n));
        t->ref = 1;
        t->sharable = true;
    }
    DM_CATCH (const std::bad_alloc& ba)
    {
        DM_FATAL("%s", ba.what());
        delete t;
    }
    return t;
#endif
}

WStringData::Data *WStringData::create(const StdStr &s)
{
#ifdef DM_NO_STRING_SHARING
    Data* t = new Data;
    t->s = s;
    return t;
#else
    Data* t = NULL;
    DM_TRY
    {
        t = new Data;
        t->s = s;
        t->ref = 1;
        t->sharable = true;
    }
    DM_CATCH (const std::bad_alloc& ba)
    {
        DM_FATAL("%s", ba.what());
        delete t;
    }
    return t;
#endif
}

WStringData::Data *WStringData::detach()
{
#ifdef DM_NO_STRING_SHARING
    return NULL;
#else
    Data *x = d;
    Data* t = NULL;
    DM_TRY
    {
        t = new Data;
        t->s = d->s;

        t->ref = 1;
        t->sharable = true;

        d = t;
    }
    DM_CATCH (const std::bad_alloc& ba)
    {
        DM_FATAL("%s", ba.what());
        delete t;
    }
    return x;
#endif
}

WStringData::Data *WStringData::detach(const StdChar *s)
{
#ifdef DM_NO_STRING_SHARING
    d->s = s;
    return NULL;
#else
    Data *x = d;

    Data* t = NULL;
    DM_TRY
    {
        t = new Data;
        t->s = s;

        t->ref = 1;
        t->sharable = true;

        d = t;
    }
    DM_CATCH (const std::bad_alloc& ba)
    {
        DM_FATAL("%s", ba.what());
        delete t;
    }
    return x;
#endif
}

WStringData::Data *WStringData::detach(const StdStr &s)
{
#ifdef DM_NO_STRING_SHARING
    d->s = s;
    return NULL;
#else
    Data *x = d;
    Data* t = NULL;
    DM_TRY
    {
        t = new Data;
        t->s = s;

        t->ref = 1;
        t->sharable = true;

        d = t;
    }
    DM_CATCH (const std::bad_alloc& ba)
    {
        DM_FATAL("%s", ba.what());
        delete t;
    }
    return x;
#endif
}

bool WStringData::isEmpty() const
{
    return d->s.empty();
}

WString::~WString()
{
#ifdef DM_NO_STRING_SHARING
    free(d);
#else
    if (!d->ref.deref())
        free(d);
#endif
}


dint WString::toInt() const
{
    if (length() == 0)
    {
        return 0;
    }

#ifdef DM_OS_WIN32
    return std::stoi(d->s);
#else
    std::basic_stringstream<StdChar> strStream;
    strStream << d->s;
    int value;
    strStream >> value;
    return value;
#endif
}

unsigned int WString::toUint() const
{
    if (length() == 0)
    {
        return 0;
    }

#ifdef DM_OS_WIN32
    return (unsigned int)std::stoi(d->s);
#else
    std::basic_stringstream<StdChar> strStream;
    strStream << d->s;
    unsigned int value;
    strStream >> value;
    return value;
#endif
}

float WString::toFloat() const
{
    if (length() == 0)
    {
        return 0.0f;
    }

#ifdef DM_OS_WIN32
    return std::stof(d->s);
#else
    std::basic_stringstream<StdChar> strStream;
    strStream << d->s;
    float value;
    strStream >> value;
    return value;
#endif
}

double WString::toDouble() const
{
    if (length() == 0)
    {
        return 0.0;
    }

#ifdef DM_OS_WIN32
    return std::stod(d->s);
#else
    std::basic_stringstream<StdChar> strStream;
    strStream << d->s;
    double value;
    strStream >> value;
    return value;
#endif
}

bool WString::toBool() const
{
    if (length() == 0)
    {
        return false;
    }

    if (d->s.Equals(StdStr("0")) || d->s.Equals(StdStr("false")))
    {
        return false;
    }
    return true;
}

dlonglong WString::toLongLong() const
{
#ifdef DM_OS_WIN32
    return std::stoll(d->s);
#else
    std::basic_stringstream<StdChar> strStream;
    strStream << d->s;
    dlonglong value;
    strStream >> value;
    return value;
#endif
}

dulonglong WString::toULongLong() const
{
#ifdef DM_OS_WIN32
    return std::stoull(d->s);
#else
    std::basic_stringstream<StdChar> strStream;
    strStream << d->s;
    dulonglong value;
    strStream >> value;
    return value;
#endif
}

const StdChar* WString::toCharStr() const
{
    return d->s.c_str();
}

const StdChar& WString::at(dint index) const
{
    return d->s.at(index);
}
/*
StdChar& WString::at(dint index)
{
    return d->s->at(index);
}
*/
duint32 WString::length() const
{
    return d->s.length();
}

int WString::indexOf(StdChar c, dint pos /*= 0*/) const
{
    return std_index_of(d->s, c, pos);
}

int WString::indexOf(const WString &s, dint pos /*= 0*/) const
{
    return std_index_of(d->s, s.d->s, pos);
}

int WString::lastIndexOf(StdChar c, dint pos /*= 0*/) const
{
    size_t found = d->s.rfind(c, pos == 0 ? StdStr::npos : pos);
    if (found!=StdStr::npos)
        return static_cast<int>(found);
    else
        return -1;
}

int WString::lastIndexOf(const WString &s, dint pos /*= 0*/) const
{
    size_t found = d->s.rfind(s.d->s, pos == 0 ? StdStr::npos : pos);
    if (found!=StdStr::npos)
        return static_cast<int>(found);
    else
        return -1;
}

dbool WString::beginWith(StdChar c) const
{
	return indexOf(c) == 0;
}

dbool WString::beginWith(const WString &str) const
{
	return indexOf(str) == 0;
}

dbool WString::endWith(StdChar c) const
{
	return lastIndexOf(c) == this->length() - 1;
}

dbool WString::endWith(const WString &str) const
{
	return lastIndexOf(str) == this->length() - 1;
}

WString WString::trimmed() const
{
    if (isEmpty()) return (*this);

    size_t s = d->s.find_first_not_of(StdStr(" \t"));
    size_t e = d->s.find_last_not_of(StdStr(" \t"));

    if (s == 0 && e == length() - 1)
    {
        return *this;
    }
#ifdef DM_CC_MSVC
    WString str = d->s.substr(s,e-s+1);
#else
    WString str;
    str.d->s = d->s.substr(s,e-s+1);
#endif
    return str;
}

WStringList WString::split(StdChar c) const
{
    WStringList l;
    if (isEmpty()) return l;

    WString s;
    
    const StdStr &left = d->s;

    dint pos = std_index_of(left, c);
    //Not found, return String itself
    if (pos < 0 && !left.empty())
    {
        l.append(*this);
        return l;
    }

    dint start = 0;
    while (pos > -1)
    {
        if (start != pos)
        {
            s = left.substr(start, pos - start);
            if (!s.isEmpty())
                l.append(s);
        }
        start = pos + 1;
        pos = std_index_of(left, c, start);
        if (pos < 0 && left.length() > start)
        {
            l.append(WString(left.substr(start)));
        }
    }

    return l;
}

WStringList WString::split(const WString &str) const
{
    WStringList l;
    if (isEmpty()) return l;

    WString s;

    const StdStr &left = d->s;

    dint pos = std_index_of(left, str.d->s);
    //Not found, return String itself
    if (pos < 0 && !left.empty())
    {
        l.append(*this);
        return l;
    }

    dint start = 0;
    while (pos > -1)
    {
        if (start != pos)
        {
            s = left.substr(start, pos - start);
            if (!s.isEmpty())
                l.append(s);
        }
        start = pos + str.length();
        pos = std_index_of(left, str.d->s, start);
        if (pos < 0 && left.length() > start)
        {
            l.append(WString(left.substr(start)));
        }
    }

    return l;
}

WString WString::subStr(size_t pos /*= 0*/, size_t len /*= -1*/) const
{
    if (isEmpty()) return WString();

#ifdef DM_CC_MSVC
    return len == -1 ? d->s.substr(pos) : d->s.substr(pos, len);
#else
    WString str;
    str.d->s = len == -1 ? d->s.substr(pos) : d->s.substr(pos, len);
    return str;
#endif
}

WString WString::left(duint size) const
{
	if (size >= length())
		return *this;

	return subStr(0, size);
}

WString WString::right(duint size) const
{
	if (size >= length())
		return *this;

	return subStr(length() - size);
}

WString &WString::erase(dint pos /*= 0*/, dint n /*= -1*/)
{
    if (isEmpty()) return *this;

    n == -1 ? d->s.erase(pos) : d->s.erase(pos, n);

    return *this;
}

WString &WString::replace(StdChar from, StdChar to)
{
    for (int i=0; i<d->s.length(); ++i)
    {
        if (d->s[i] == from)
            d->s[i] = to;
    }

    return *this;
}

WString &WString::replace(const WString &from, const WString &to)
{
    StdStr &left = d->s;
    dint oldLen = from.length();
    dint newLen = to.length();
    dint pos = std_index_of(left, from.d->s);
    //Not found, return String itself
    if (pos < 0)
    {
        return *this;
    }

    dint start = 0;
    while (pos > -1)
    {
        if (start != pos)
        {
            left.replace(pos, oldLen, to.d->s);
        }
        start = pos + newLen;
        pos = std_index_of(left, from.d->s, start);
    }
    return *this;
}

void WString::clear()
{
    *this = WString();
}

void WString::reserve (duint n /*= 0*/)
{
    d->s.reserve(n);
}

WString& WString::lower()
{
    d->s.ToLower();
    return *this;
}

WString& WString::upper()
{
    d->s.ToUpper();
    return *this;
}

WString WString::toLower() const
{
    return WString(*this).lower();
}

WString WString::toUpper() const
{
    return WString(*this).upper();
}

WString WString::fromAscii(const StdChar *s)
{
    return WString(s);
}

WString WString::fromStdString(const StdStr &s)
{
    return WString(s);
}

WString WString::sprintf(const StdChar *cformat, ...)
{
    StdStr s;
    va_list argList;
    va_start(argList, cformat);
    s.FormatV(cformat, argList);
    va_end(argList);

    return WString(s);
}

StdChar& WString::operator[](dint i) 
{ 
    detach();
    return d->s.at(i); 
}

const StdChar& WString::operator[](dint i) const { return d->s.at(i); }

dbool WString::operator==(const char *s) const
{
    if (d->s.empty()) return s == NULL ? true : false;
    else return d->s.Equals(StdStr(s), true);
}

dbool WString::operator==(const StdChar *s) const
{
    if (d->s.empty()) return s == NULL ? true : false;
    else return d->s == s;
}

dbool WString::operator==(const StdStr &s) const
{
    return d->s == s;
}

bool WString::operator==(const WString &s) const
{
    if (d == s.d)
        return true;

    if (d->s == s.d->s) 
    {
        return true;
    }
    return false;
}

WString& WString::operator+=(StdChar c)
{
    detach();

    d->s += c;
    return (*this);
}

WString& WString::operator+=(const StdChar *string)
{
    if (string != NULL)
    {
        if (isEmpty())
            *this = string;
        else
        {
            detach();

            d->s += string;
        }
    }

    return (*this);
}

WString& WString::operator+=(const StdStr &string)
{
    if (!string.empty())
    {
        if (isEmpty())
            *this = string;
        else
        {
            detach();

            d->s += string;
        }
    }

    return (*this);
}

WString& WString::operator+=(const WString &string)
{
    if (!string.isEmpty())
    {
        if (isEmpty())
            *this = string;
        else
        {
            detach();

            d->s += string.d->s;
        }
    }
    return (*this);
}

void WString::detach_helper()
{
#ifdef DM_NO_STRING_SHARING

#else
    if (d->sharable)
    {
        WStringData::Data *x = p.detach();
        if (!x->ref.deref())
            free(x);
    }
#endif
}

void WString::detach_helper(const StdChar *s)
{
#ifdef DM_NO_STRING_SHARING

#else
    WStringData::Data *x = p.detach(s);
    if (!x->ref.deref())
        free(x);
#endif
}

void WString::detach_helper(const StdStr &s)
{
#ifdef DM_NO_STRING_SHARING

#else
    WStringData::Data *x = p.detach(s);
    if (!x->ref.deref())
        free(x);
#endif
}

void WString::free(WStringData::Data *data)
{
    if (data != &WStringData::shared_null)
        delete data;
}

DM_END_NAMESPACE
