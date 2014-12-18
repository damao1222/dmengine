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

#include "dmutilstring.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "dmlogger.h"
#ifndef DM_CC_MSVC
#include <stdio.h>
#include <sstream>
#endif

DM_BEGIN_NAMESPACE

static dint std_index_of(const std::string &str, char c, dint pos = 0)
{
    size_t found = str.find(c, pos);
    if (found != std::string::npos)
        return static_cast<dint>(found);
    else
        return -1;
}

static dint std_index_of(const std::string &str, const std::string &s, dint pos = 0)
{
    size_t found = str.find(s, pos);
    if (found != std::string::npos)
        return static_cast<dint>(found);
    else
        return -1;
}

UtilString::~UtilString()
{
    _str.clear();
}

dint UtilString::toInt() const
{
    if (length() == 0)
    {
        return 0;
    }

#ifdef DM_OS_WIN32
    return std::stoi(_str);
#else
    return atoi(_str.c_str());
#endif
}

unsigned int UtilString::toUint() const
{
    if (length() == 0)
    {
        return 0;
    }

#ifdef DM_OS_WIN32
    return (unsigned int)std::stoi(_str);
#else
    return (unsigned dint)atoi(_str.c_str());
#endif
}

float UtilString::toFloat() const
{
    if (length() == 0)
    {
        return 0.0f;
    }

#ifdef DM_OS_WIN32
    return std::stof(_str);
#else
    return (float)atof(_str.c_str());
#endif
}

double UtilString::toDouble() const
{
    if (length() == 0)
    {
        return 0.0;
    }

#ifdef DM_OS_WIN32
    return std::stod(_str);
#else
    return atof(_str.c_str());
#endif
}

bool UtilString::toBool() const
{
    if (length() == 0)
    {
        return false;
    }

    if (0 == strcmp(_str.c_str(), "0") || 0 == strcmp(_str.c_str(), "false"))
    {
        return false;
    }
    return true;
}

dlonglong UtilString::toLongLong() const
{
#ifdef DM_OS_WIN32
    return std::stoll(_str);
#else
    std::stringstream strStream;
    strStream << _str.c_str();
    dlonglong value;
    strStream >> value;
    return value;
#endif
}

dulonglong UtilString::toULongLong() const
{
#ifdef DM_OS_WIN32
    return std::stoull(_str);
#else
    std::stringstream strStream;
    strStream << _str.c_str();
    dulonglong value;
    strStream >> value;
    return value;
#endif
}

const char* UtilString::toCharStr() const
{
    return _str.c_str();
}

const char& UtilString::at(dint index) const
{
    return _str.at(index);
}
/*
char& UtilString::at(dint index)
{
    return _str->at(index);
}
*/
duint32 UtilString::length() const
{
    return _str.length();
}

dint UtilString::indexOf(char c, dint pos /*= 0*/) const
{
    return std_index_of(_str, c, pos);
}

dint UtilString::indexOf(const UtilString &s, dint pos /*= 0*/) const
{
    return std_index_of(_str, s._str, pos);
}

dint UtilString::lastIndexOf(char c, dint pos /*= -1*/) const
{
    size_t found = _str.rfind(c, pos == -1 ? std::string::npos : pos);
    if (found!=std::string::npos)
        return static_cast<dint>(found);
    else
        return -1;
}

dint UtilString::lastIndexOf(const UtilString &s, dint pos /*= -1*/) const
{
    size_t found = _str.rfind(s._str, pos == -1 ? std::string::npos : pos);
    if (found!=std::string::npos)
        return static_cast<dint>(found);
    else
        return -1;
}

dbool UtilString::beginWith(char c) const
{
    return indexOf(c) == 0;
}

dbool UtilString::beginWith(const UtilString &str) const
{
    return indexOf(str) == 0;
}

dbool UtilString::endWith(char c) const
{
    return lastIndexOf(c) == this->length() - 1;
}

dbool UtilString::endWith(const UtilString &str) const
{
    return lastIndexOf(str) == this->length() - str.length();
}

UtilString UtilString::trimmed() const
{
    if (isEmpty()) return (*this);

    size_t s = _str.find_first_not_of(" \t");
    size_t e = _str.find_last_not_of(" \t");

    if (s == 0 && e == length() - 1)
    {
        return *this;
    }

    UtilString str = _str.substr(s,e-s+1);
    return str;
}

UtilStringList UtilString::split(char c) const
{
    UtilStringList l;
    if (isEmpty()) return l;

    UtilString s;
    
    const std::string &left = _str;

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
            l.append(left.substr(start));
        }
    }

    return l;
}

UtilStringList UtilString::split(const UtilString &str) const
{
    UtilStringList l;
    if (isEmpty()) return l;

    UtilString s;

    const std::string &left = _str;

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
            l.append(left.substr(start));
        }
    }

    return l;
}

static char ToLower(const char c)
{
    return std::tolower(c, std::locale());
}

static char ToUpper(const char c)
{
    return std::toupper(c, std::locale());
}

UtilString& UtilString::lower()
{
    std::transform( _str.begin(),
                    _str.end(),
                    _str.begin(),
                    ToLower);
    return *this;
}

UtilString& UtilString::upper()
{
    std::transform( _str.begin(),
                    _str.end(),
                    _str.begin(),
                    ToUpper);
    return *this;
}

UtilString UtilString::toLower() const
{
    return UtilString(*this).lower();
}

UtilString UtilString::toUpper() const
{
    return UtilString(*this).upper();
}

dbool UtilString::equals(const UtilString &s, dbool sensitive /*= false*/) const
{
    if (sensitive)
        return *this == s;
    else
    {
        dint len = size(), i = 0;
        if (len == s.size())
        {
            while (i != len)
            {
                char f = ToLower(at(i));
                char t = ToLower(s.at(i));
                if (f != t)
                    return false;
                ++i;
            }
            return true;
        }
    }
    return false;
}

dbool UtilString::compare(const UtilString &s, dbool sensitive /*= false*/) const
{
    return equals(s, sensitive);
}

UtilString UtilString::subStr(size_t pos /*= 0*/, size_t len /*= -1*/) const
{
    if (isEmpty()) return UtilString();

    return len == -1 ? _str.substr(pos) : _str.substr(pos, len);
}

UtilString UtilString::left(duint size) const
{
	if (size >= length())
		return *this;

	return subStr(0, size);
}

UtilString UtilString::right(duint size) const
{
	if (size >= length())
		return *this;

	return subStr(length() - size);
}

UtilString &UtilString::erase(dint pos /*= 0*/, dint n /*= -1*/)
{
    if (isEmpty()) return *this;

    n == -1 ? _str.erase(pos) : _str.erase(pos, n);

    return *this;
}

UtilString &UtilString::replace(char from, char to)
{
    for (dint i=0; i<_str.length(); ++i)
    {
        if (_str[i] == from)
            _str[i] = to;
    }

    return *this;
}

UtilString &UtilString::replace(const UtilString &from, const UtilString &to)
{
    std::string &left = _str;
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

void UtilString::clear()
{
    *this = UtilString();
}

void UtilString::reserve (duint n /*= 0*/)
{
    _str.reserve(n);
}

UtilString UtilString::fromAscii(const char *s)
{
    return UtilString(s);
}

UtilString UtilString::fromStdString(const std::string &s)
{
    return UtilString(s);
}

UtilString UtilString::sprintf(const char *cformat, ...)
{
    char buff[DM_MEMORYNUM];
    memset(buff, 0, sizeof(buff));
    va_list ap;
    va_start(ap, cformat);
#ifdef DM_CC_MSVC
    vsprintf_s(buff, DM_MEMORYNUM, cformat, ap);
#else
    vsnprintf(buff, DM_MEMORYNUM, cformat, ap);
#endif
    va_end(ap);
    UtilString s = buff;
    return s;
}

WString UtilString::toWString() const
{
    return WString(_str);
}

char& UtilString::operator[](dint i) 
{ 
    return _str.at(i); 
}

const char& UtilString::operator[](dint i) const { return _str.at(i); }

dbool UtilString::operator==(const char *s) const
{
    if (_str.empty()) return s == NULL ? true : false;
    else return _str == s;
}

dbool UtilString::operator==(const std::string &s) const
{
    return _str == s;
}

bool UtilString::operator==(const UtilString &s) const
{
    if (_str == s._str) 
    {
        return true;
    }
    return false;
}

UtilString& UtilString::operator+=(char c)
{
    _str += c;
    return (*this);
}

UtilString& UtilString::operator+=(const char *string)
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

UtilString& UtilString::operator+=(const std::string &string)
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

UtilString& UtilString::operator+=(const UtilString &string)
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
