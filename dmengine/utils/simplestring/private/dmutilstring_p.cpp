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

WString::~WString()
{
    _str.clear();
}


dint WString::toInt() const
{
    if (length() == 0)
    {
        return 0;
    }

#ifdef DM_OS_WIN32
    return std::stoi(_str);
#else
    std::basic_stringstream<StdChar> strStream;
    strStream << _str;
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
    return (unsigned int)std::stoi(_str);
#else
    std::basic_stringstream<StdChar> strStream;
    strStream << _str;
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
    return std::stof(_str);
#else
    std::basic_stringstream<StdChar> strStream;
    strStream << _str;
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
    return std::stod(_str);
#else
    std::basic_stringstream<StdChar> strStream;
    strStream << _str;
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

    if (_str.Equals(StdStr("0")) || _str.Equals(StdStr("false")))
    {
        return false;
    }
    return true;
}

dlonglong WString::toLongLong() const
{
#ifdef DM_OS_WIN32
    return std::stoll(_str);
#else
    std::basic_stringstream<StdChar> strStream;
    strStream << _str;
    dlonglong value;
    strStream >> value;
    return value;
#endif
}

dulonglong WString::toULongLong() const
{
#ifdef DM_OS_WIN32
    return std::stoull(_str);
#else
    std::basic_stringstream<StdChar> strStream;
    strStream << _str;
    dulonglong value;
    strStream >> value;
    return value;
#endif
}

const StdChar* WString::toCharStr() const
{
    return _str.c_str();
}

const StdChar& WString::at(dint index) const
{
    return _str.at(index);
}
/*
StdChar& WString::at(dint index)
{
    return _str->at(index);
}
*/
duint32 WString::length() const
{
    return _str.length();
}

int WString::indexOf(StdChar c, dint pos /*= 0*/) const
{
    return std_index_of(_str, c, pos);
}

int WString::indexOf(const WString &s, dint pos /*= 0*/) const
{
    return std_index_of(_str, s._str, pos);
}

int WString::lastIndexOf(StdChar c, dint pos /*= 0*/) const
{
    size_t found = _str.rfind(c, pos == 0 ? StdStr::npos : pos);
    if (found!=StdStr::npos)
        return static_cast<int>(found);
    else
        return -1;
}

int WString::lastIndexOf(const WString &s, dint pos /*= 0*/) const
{
    size_t found = _str.rfind(s._str, pos == 0 ? StdStr::npos : pos);
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

    size_t s = _str.find_first_not_of(StdStr(" \t"));
    size_t e = _str.find_last_not_of(StdStr(" \t"));

    if (s == 0 && e == length() - 1)
    {
        return *this;
    }
#ifdef DM_CC_MSVC
    WString str = _str.substr(s,e-s+1);
#else
    WString str;
    str._str = _str.substr(s,e-s+1);
#endif
    return str;
}

WStringList WString::split(StdChar c) const
{
    WStringList l;
    if (isEmpty()) return l;

    WString s;
    
    const StdStr &left = _str;

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

    const StdStr &left = _str;

    dint pos = std_index_of(left, str._str);
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
        pos = std_index_of(left, str._str, start);
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
    return len == -1 ? _str.substr(pos) : _str.substr(pos, len);
#else
    WString str;
    str._str = len == -1 ? _str.substr(pos) : _str.substr(pos, len);
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

    n == -1 ? _str.erase(pos) : _str.erase(pos, n);

    return *this;
}

WString &WString::replace(StdChar from, StdChar to)
{
    for (int i=0; i<_str.length(); ++i)
    {
        if (_str[i] == from)
            _str[i] = to;
    }

    return *this;
}

WString &WString::replace(const WString &from, const WString &to)
{
    StdStr &left = _str;
    dint oldLen = from.length();
    dint newLen = to.length();
    dint pos = std_index_of(left, from._str);
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
            left.replace(pos, oldLen, to._str);
        }
        start = pos + newLen;
        pos = std_index_of(left, from._str, start);
    }
    return *this;
}

void WString::clear()
{
    *this = WString();
}

void WString::reserve (duint n /*= 0*/)
{
    _str.reserve(n);
}

WString& WString::lower()
{
    _str.ToLower();
    return *this;
}

WString& WString::upper()
{
    _str.ToUpper();
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
    return _str.at(i); 
}

const StdChar& WString::operator[](dint i) const { return _str.at(i); }

dbool WString::operator==(const char *s) const
{
    if (_str.empty()) return s == NULL ? true : false;
    else return _str.Equals(StdStr(s), true);
}

dbool WString::operator==(const StdChar *s) const
{
    if (_str.empty()) return s == NULL ? true : false;
    else return _str == s;
}

dbool WString::operator==(const StdStr &s) const
{
    return _str == s;
}

bool WString::operator==(const WString &s) const
{
    if (_str == s._str) 
    {
        return true;
    }
    return false;
}

WString& WString::operator+=(StdChar c)
{
    _str += c;
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
            _str += string;
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
            _str += string;
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
            _str += string._str;
        }
    }
    return (*this);
}
DM_END_NAMESPACE
