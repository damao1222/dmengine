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
#ifdef DM_NO_STRING_SHARING
StringData::Data StringData::shared_null = {  };
#else
StringData::Data StringData::shared_null = { DM_BASIC_ATOMIC_INITIALIZER(1), true, };
#endif

StringData::Data *StringData::create(const char *s, size_t n /*= 0*/)
{
#ifdef DM_NO_STRING_SHARING
    Data* t = new Data;
    if (s != NULL)
        t->s = (n == 0 ? s : std::string(s, n));
    return t;
#else
    DM_ASSERT(s);

    Data* t = NULL;
    DM_TRY
    {
        t = new Data;
        t->s = (n == 0 ? s : std::string(s, n));
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

StringData::Data *StringData::create(const std::string &s)
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

StringData::Data *StringData::detach()
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

StringData::Data *StringData::detach(const char *s)
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

StringData::Data *StringData::detach(const std::string &s)
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

bool StringData::isEmpty() const
{
    return d->s.empty();
}

UtilString::~UtilString()
{
#ifdef DM_NO_STRING_SHARING
    free(d);
#else
    if (!d->ref.deref())
        free(d);
#endif
}

dint UtilString::toInt() const
{
    if (length() == 0)
    {
        return 0;
    }

#ifdef DM_OS_WIN32
    return std::stoi(d->s);
#else
    return atoi(d->s.c_str());
#endif
}

unsigned int UtilString::toUint() const
{
    if (length() == 0)
    {
        return 0;
    }

#ifdef DM_OS_WIN32
    return (unsigned int)std::stoi(d->s);
#else
    return (unsigned dint)atoi(d->s.c_str());
#endif
}

float UtilString::toFloat() const
{
    if (length() == 0)
    {
        return 0.0f;
    }

#ifdef DM_OS_WIN32
    return std::stof(d->s);
#else
    return (float)atof(d->s.c_str());
#endif
}

double UtilString::toDouble() const
{
    if (length() == 0)
    {
        return 0.0;
    }

#ifdef DM_OS_WIN32
    return std::stod(d->s);
#else
    return atof(d->s.c_str());
#endif
}

bool UtilString::toBool() const
{
    if (length() == 0)
    {
        return false;
    }

    if (0 == strcmp(d->s.c_str(), "0") || 0 == strcmp(d->s.c_str(), "false"))
    {
        return false;
    }
    return true;
}

dlonglong UtilString::toLongLong() const
{
#ifdef DM_OS_WIN32
    return std::stoll(d->s);
#else
    std::stringstream strStream;
    strStream << d->s.c_str();
    dlonglong value;
    strStream >> value;
    return value;
#endif
}

dulonglong UtilString::toULongLong() const
{
#ifdef DM_OS_WIN32
    return std::stoull(d->s);
#else
    std::stringstream strStream;
    strStream << d->s.c_str();
    dulonglong value;
    strStream >> value;
    return value;
#endif
}

const char* UtilString::toCharStr() const
{
    return d->s.c_str();
}

const char& UtilString::at(dint index) const
{
    return d->s.at(index);
}
/*
char& UtilString::at(dint index)
{
    return d->s->at(index);
}
*/
duint32 UtilString::length() const
{
    return d->s.length();
}

dint UtilString::indexOf(char c, dint pos /*= 0*/) const
{
    return std_index_of(d->s, c, pos);
}

dint UtilString::indexOf(const UtilString &s, dint pos /*= 0*/) const
{
    return std_index_of(d->s, s.d->s, pos);
}

dint UtilString::lastIndexOf(char c, dint pos /*= -1*/) const
{
    size_t found = d->s.rfind(c, pos == -1 ? std::string::npos : pos);
    if (found!=std::string::npos)
        return static_cast<dint>(found);
    else
        return -1;
}

dint UtilString::lastIndexOf(const UtilString &s, dint pos /*= -1*/) const
{
    size_t found = d->s.rfind(s.d->s, pos == -1 ? std::string::npos : pos);
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

    size_t s = d->s.find_first_not_of(" \t");
    size_t e = d->s.find_last_not_of(" \t");

    if (s == 0 && e == length() - 1)
    {
        return *this;
    }

    UtilString str = d->s.substr(s,e-s+1);
    return str;
}

UtilStringList UtilString::split(char c) const
{
    UtilStringList l;
    if (isEmpty()) return l;

    UtilString s;
    
    const std::string &left = d->s;

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

    const std::string &left = d->s;

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
    std::transform( d->s.begin(),
                    d->s.end(),
                    d->s.begin(),
                    ToLower);
    return *this;
}

UtilString& UtilString::upper()
{
    std::transform( d->s.begin(),
                    d->s.end(),
                    d->s.begin(),
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

    return len == -1 ? d->s.substr(pos) : d->s.substr(pos, len);
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

    n == -1 ? d->s.erase(pos) : d->s.erase(pos, n);

    return *this;
}

UtilString &UtilString::replace(char from, char to)
{
    for (dint i=0; i<d->s.length(); ++i)
    {
        if (d->s[i] == from)
            d->s[i] = to;
    }

    return *this;
}

UtilString &UtilString::replace(const UtilString &from, const UtilString &to)
{
    std::string &left = d->s;
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

void UtilString::clear()
{
    *this = UtilString();
}

void UtilString::reserve (duint n /*= 0*/)
{
    d->s.reserve(n);
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
    return WString(d->s);
}

char& UtilString::operator[](dint i) 
{ 
    detach();
    return d->s.at(i); 
}

const char& UtilString::operator[](dint i) const { return d->s.at(i); }

dbool UtilString::operator==(const char *s) const
{
    if (d->s.empty()) return s == NULL ? true : false;
    else return d->s == s;
}

dbool UtilString::operator==(const std::string &s) const
{
    return d->s == s;
}

bool UtilString::operator==(const UtilString &s) const
{
    if (d == s.d)
        return true;

    if (d->s == s.d->s) 
    {
        return true;
    }
    return false;
}

UtilString& UtilString::operator+=(char c)
{
    detach();

    d->s += c;
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
            detach();

            d->s += string;
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
            detach();

            d->s += string;
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
            detach();

            d->s += string.d->s;
        }
    }
    return (*this);
}

void UtilString::detach_helper()
{
#ifdef DM_NO_STRING_SHARING

#else
    if (d->sharable)
    {
        StringData::Data *x = p.detach();
        if (!x->ref.deref())
            free(x);
    }
#endif
}

void UtilString::detach_helper(const char *s)
{
#ifdef DM_NO_STRING_SHARING

#else
    StringData::Data *x = p.detach(s);
    if (!x->ref.deref())
        free(x);
#endif
}

void UtilString::detach_helper(const std::string &s)
{
#ifdef DM_NO_STRING_SHARING

#else
    StringData::Data *x = p.detach(s);
    if (!x->ref.deref())
        free(x);
#endif
}

void UtilString::free(StringData::Data *data)
{
    if (data != &StringData::shared_null)
        delete data;
}

DM_END_NAMESPACE
