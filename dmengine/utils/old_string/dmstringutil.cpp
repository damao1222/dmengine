/*
   Copyright (C) 2012-2013 Xiongfa Li
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

#include "dmstringutil.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "dmlogger.h"
#ifndef DM_CC_MSVC
#include <stdio.h>
#endif
#include <sstream>

DM_BEGIN_NAMESPACE

static int std_index_of(const std::string &str, char c, dint pos = 0)
{
    size_t found = str.find(c, pos);
    if (found != std::string::npos)
        return static_cast<int>(found);
    else
        return -1;
}

static int std_index_of(const std::string &str, const std::string &s, dint pos = 0)
{
    size_t found = str.find(s, pos);
    if (found != std::string::npos)
        return static_cast<int>(found);
    else
        return -1;
}

StringData::Data StringData::shared_null = { DM_BASIC_ATOMIC_INITIALIZER(1), true, new std::string() };

StringData::Data *StringData::create(const char *s, size_t n /*= 0*/)
{
    DM_ASSERT(s);

    Data* t = NULL;
    try
    {
        t = new Data;
        t->s = n == 0 ? new std::string(s) : new std::string(s, n);
        t->ref = 1;
        t->sharable = true;
    }
    catch (const std::bad_alloc& ba)
    {
        DM_FATAL("%s", ba.what());
        delete t->s;
        delete t;
    }
    return t;
}

StringData::Data *StringData::create(const std::string &s)
{
    Data* t = NULL;
    try
    {
        t = new Data;
        t->s = new std::string(s);
        t->ref = 1;
        t->sharable = true;
    }
    catch (const std::bad_alloc& ba)
    {
        DM_FATAL("%s", ba.what());
        delete t->s;
        delete t;
    }
    return t;
}

StringData::Data *StringData::detach()
{
    Data *x = d;
    Data* t = NULL;
    try
    {
        t = new Data;
        t->s = new std::string(*d->s);

        t->ref = 1;
        t->sharable = true;

        d = t;
    }
    catch (const std::bad_alloc& ba)
    {
        DM_FATAL("%s", ba.what());
        delete t->s;
        delete t;
    }
    return x;
}

StringData::Data *StringData::detach(const char *s)
{
    Data *x = d;

    Data* t = NULL;
    try
    {
        t = new Data;
        t->s = new std::string(s);

        t->ref = 1;
        t->sharable = true;

        d = t;
    }
    catch (const std::bad_alloc& ba)
    {
        DM_FATAL("%s", ba.what());
        delete t->s;
        delete t;
    }
    return x;
}

StringData::Data *StringData::detach(const std::string &s)
{
    Data *x = d;
    Data* t = NULL;
    try
    {
        t = new Data;
        t->s = new std::string(s);

        t->ref = 1;
        t->sharable = true;

        d = t;
    }
    catch (const std::bad_alloc& ba)
    {
        DM_FATAL("%s", ba.what());
        delete t->s;
        delete t;
    }
    return x;
}

bool StringData::isEmpty() const
{
    return d->s->empty();
}

UtilString::~UtilString()
{
    if (!d->ref.deref())
        free(d);
}

int UtilString::toInt() const
{
    if (length() == 0)
    {
        return 0;
    }
    return atoi(d->s->c_str());
}

unsigned int UtilString::toUint() const
{
    if (length() == 0)
    {
        return 0;
    }
    return (unsigned int)atoi(d->s->c_str());
}

float UtilString::toFloat() const
{
    if (length() == 0)
    {
        return 0.0f;
    }
    return (float)atof(d->s->c_str());
}

double UtilString::toDouble() const
{
    if (length() == 0)
    {
        return 0.0;
    }
    return atof(d->s->c_str());
}

bool UtilString::toBool() const
{
    if (length() == 0)
    {
        return false;
    }

    if (0 == strcmp(d->s->c_str(), "0") || 0 == strcmp(d->s->c_str(), "false"))
    {
        return false;
    }
    return true;
}

dlonglong UtilString::toLongLong() const
{
    std::stringstream strStream;
    strStream << d->s->c_str();
    dlonglong value;
    strStream >> value;
    return value;
}

dulonglong UtilString::toULongLong() const
{
    std::stringstream strStream;
    strStream << d->s->c_str();
    dulonglong value;
    strStream >> value;
    return value;
}

const char* UtilString::toCharStr() const
{
    return d->s->c_str();
}

const char& UtilString::at(dint index) const
{
    return d->s->at(index);
}
/*
char& UtilString::at(dint index)
{
    return d->s->at(index);
}
*/
duint32 UtilString::length() const
{
    return d->s->length();
}

int UtilString::indexOf(char c, dint pos /*= 0*/) const
{
    return std_index_of(*d->s, c, pos);
}

int UtilString::indexOf(const UtilString &s, dint pos /*= 0*/) const
{
    return std_index_of(*d->s, *s.d->s, pos);
}

int UtilString::lastIndexOf(char c, dint pos /*= 0*/) const
{
    size_t found = d->s->rfind(c, pos == 0 ? std::string::npos : pos);
    if (found!=std::string::npos)
        return static_cast<int>(found);
    else
        return -1;
}

int UtilString::lastIndexOf(const UtilString &s, dint pos /*= 0*/) const
{
    size_t found = d->s->rfind(*(s.d->s), pos == 0 ? std::string::npos : pos);
    if (found!=std::string::npos)
        return static_cast<int>(found);
    else
        return -1;
}

UtilString UtilString::trimmed() const
{
    if (isEmpty()) return (*this);

    size_t s = d->s->find_first_not_of(" \t");
    size_t e = d->s->find_last_not_of(" \t");

    if (s == 0 && e == length() - 1)
    {
        return *this;
    }

    UtilString str = d->s->substr(s,e-s+1);
    return str;
}

UtilStringList UtilString::split(char c) const
{
    UtilStringList l;
    if (isEmpty()) return l;

    UtilString s;
    std::string left = *d->s;
    int pos = std_index_of(left, c);
    //Not found, return String itself
    if (pos < 0 && !left.empty())
    {
        l.append(*this);
        return l;
    }

    while (pos > -1)
    {
        s = left.substr(0, pos);
        if (!s.isEmpty())
            l.append(s);
        left = left.substr(pos+1);
        pos = std_index_of(left, c);
        if (pos < 0 && left.length() > 0)
        {
            l.append(left);
        }
    }

    return l;
}

UtilStringList UtilString::split(const UtilString &str) const
{
    UtilStringList l;
    if (isEmpty()) return l;

    UtilString s;
    std::string left = *d->s;
    int pos = std_index_of(left, *str.d->s);
    //Not found, return String itself
    if (pos < 0 && !left.empty())
    {
        l.append(*this);
        return l;
    }

    while (pos > -1)
    {
        s = left.substr(0, pos);
        if (!s.isEmpty())
            l.append(s);
        left = left.substr(pos+str.length());
        pos = std_index_of(left, *str.d->s);
        if (pos < 0 && left.length() > 0)
        {
            l.append(left);
        }
    }

    return l;
}

UtilString UtilString::subStr(size_t pos /*= 0*/, size_t len /*= -1*/) const
{
    if (isEmpty()) return UtilString();

    return len == -1 ? d->s->substr(pos) : d->s->substr(pos, len);
}

UtilString &UtilString::erase(dint pos /*= 0*/, dint n /*= -1*/)
{
    if (isEmpty()) return *this;

    n == -1 ? d->s->erase(pos) : d->s->erase(pos, n);

    return *this;
}

void UtilString::clear()
{
    *this = UtilString();
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

bool UtilString::operator==(const UtilString &s) const
{
    if (d == s.d)
        return true;

    if (d->s == NULL)
    {
        return s.d->s == NULL ? true : false;
    }
    else
    {
        if ((*(d->s)) == (*(s.d->s))) {
            return true;
        }
    } 
    return false;
}

void UtilString::detach_helper()
{
    if (d->sharable)
    {
        StringData::Data *x = p.detach();
        if (!x->ref.deref())
            free(x);
    }
}

void UtilString::detach_helper(const char *s)
{
    StringData::Data *x = p.detach(s);
    if (!x->ref.deref())
        free(x);
}

void UtilString::detach_helper(const std::string &s)
{
    StringData::Data *x = p.detach(s);
    if (!x->ref.deref())
        free(x);
}

void UtilString::free(StringData::Data *data)
{
    delete data->s;
    delete data;
}

DM_END_NAMESPACE
