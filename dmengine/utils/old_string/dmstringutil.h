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

#ifndef DMSTRINGUTIL_H
#define DMSTRINGUTIL_H

#include <string>
#include "dmnamespace.h"
#include "dmvector.h"

DM_BEGIN_NAMESPACE
class UtilString;
class DM_DLL_EXPORT UtilStringList: public Vector<UtilString>
{
public:
    inline UtilStringList() { }
    inline explicit UtilStringList(const UtilString &i) { append(i); }
    inline UtilStringList(const UtilStringList &l) : Vector<UtilString>(l) { }
    inline UtilStringList(const Vector<UtilString> &l) : Vector<UtilString>(l) { }
};

struct DM_DLL_EXPORT StringData {
    struct Data {
        BasicAtomicInt ref;
        duint32 sharable : 1;
        std::string* s;
    };

    Data *create(const char *s, size_t n = 0);
    Data *create(const std::string &s);
    Data *detach();
    Data *detach(const char *s);
    Data *detach(const std::string &s);
    bool isEmpty() const;
    static Data shared_null;
    Data *d;
};

class DM_DLL_EXPORT UtilString
{
    union { StringData p; StringData::Data *d; };

public:
    inline UtilString();
    inline UtilString(char ch);
    inline UtilString(const char *s);
    inline UtilString (const char* s, size_t n);
    inline UtilString(const std::string &s);
    inline UtilString(const UtilString &s);
    ~UtilString();

    // convert to int value
    int toInt() const;
    // convert to unsigned int value
    unsigned int toUint() const;
    // convert to float value
    float toFloat() const;
    // convert to double value
    double toDouble() const;
    //convert to bool value
    bool toBool() const;

    dlonglong toLongLong() const;

    dulonglong toULongLong() const;

    //get the C string
    const char* toCharStr() const;

    const char& at(dint index) const;
    //char& at(dint index);

    //get the length of string
    duint32 length() const;
    duint32 size() const;

    inline std::string stdString() const;
    inline std::string& asStdString();

    int indexOf(char c, dint pos = 0) const;
    int indexOf(const UtilString &str, dint pos = 0) const;
    int lastIndexOf(char c, dint pos = 0) const;
    int lastIndexOf(const UtilString &str, dint pos = 0) const;

    UtilString trimmed() const;
    UtilStringList split(char c) const;
    UtilStringList split(const UtilString &str) const;

    UtilString subStr(size_t pos = 0, size_t len = -1) const;

    UtilString &erase(dint pos = 0, dint n = -1);
    UtilString &remove(dint pos = 0, dint n = -1)
    { return erase(pos, n); }

    inline bool isEmpty() const;
    void clear();

    inline bool isDetached() const;
    inline void setSharable(dbool sharable);
    inline bool isSharedWith(const UtilString &other) const;
    inline void detach();

    inline char& operator[](dint i);
    inline const char& operator[](dint i) const;
    inline UtilString& operator=(char ch);
    inline UtilString& operator=(const char *string);
    inline UtilString& operator=(const std::string &string);
    inline UtilString& operator=(const UtilString &other);

    inline bool operator==(const char *string) const;
    inline bool operator==(const std::string &string) const;
    bool operator==(const UtilString &other) const;

    inline bool operator!=(const char *string) const;
    inline bool operator!=(const std::string &string) const;
    inline bool operator!=(const UtilString &other) const;

    inline UtilString& operator+=(char c);
    inline UtilString& operator+=(const char *string);
    inline UtilString& operator+=(const std::string &string);
    inline UtilString& operator+=(const UtilString &other);

    static UtilString fromAscii(const char *s);
    static UtilString fromStdString(const std::string &s);
    //NOTICE: limited DM_MEMORYNUM(see dmglobal.h) chars
    static UtilString sprintf(const char *cformat, ...);

private:
    void detach_helper();
    void detach_helper(const char *s);
    void detach_helper(const std::string &s);
    void free(StringData::Data *data);
};

inline UtilString::UtilString(): d(&StringData::shared_null)
{ d->ref.ref(); }

inline UtilString::UtilString(char ch):
    d(p.create(std::string(1, ch)))
{
}

inline UtilString::UtilString(const char *s):
    d(p.create(s))
{
}

inline UtilString::UtilString (const char* s, size_t n):
    d(p.create(s, n))
{

}

inline UtilString::UtilString(const std::string &s):
    d(p.create(s))
{
}

inline UtilString::UtilString(const UtilString &s): d(s.d)
{ d->ref.ref(); if (!d->sharable) detach_helper(); }

inline duint32 UtilString::size() const
{ return d->s->length(); }

inline std::string UtilString::stdString() const
{
    return std::string(*(d->s));
}

inline std::string& UtilString::asStdString()
{
    return *(d->s);
}

inline bool UtilString::isDetached() const { return d->ref == 1; }
inline void UtilString::setSharable(dbool sharable) { if (!sharable) detach(); d->sharable = sharable; }
inline bool UtilString::isSharedWith(const UtilString &other) const { return d == other.d; }
inline void UtilString::detach() { if (d->ref != 1) detach_helper(); }

inline bool UtilString::isEmpty() const { return p.isEmpty(); }

inline char& UtilString::operator[](dint i) { return d->s->at(i); }
inline const char& UtilString::operator[](dint i) const { return d->s->at(i); }

inline UtilString& UtilString::operator=(char ch)
{
    return operator=(std::string(1, ch));
}

inline UtilString& UtilString::operator=(const char *s)
{
    detach_helper(s);
    return (*this);
}

inline UtilString& UtilString::operator=(const std::string &s)
{
    detach_helper(s);
    return (*this);
}

inline UtilString& UtilString::operator=(const UtilString &s)
{
    if (this == &s)
        return *this;

    if (d != s.d) {
        StringData::Data *o = s.d;
        o->ref.ref();
        if (!d->ref.deref())
            free(d);
        d = o;
        if (!d->sharable)
            detach_helper();
    }
    return (*this);
}

inline bool UtilString::operator==(const char *s) const
{
    if (d->s == NULL) return s == NULL ? true : false;
    else return *(d->s) == s;
}

inline bool UtilString::operator==(const std::string &s) const
{
    if (d->s == NULL) return s == "" ? true : false;
    else return *(d->s) == s;
}

inline bool UtilString::operator!=(const char *s) const
{
    return !operator==(s);
}

inline bool UtilString::operator!=(const std::string &s) const
{
    return !operator==(s);
}

inline bool UtilString::operator!=(const UtilString &s) const
{
    return !operator==(s);
}

inline UtilString& UtilString::operator+=(char c)
{
    detach();

    (*d->s) += c;
    return (*this);
}

inline UtilString& UtilString::operator+=(const char *string)
{
    if (string != NULL)
    {
        if (isEmpty())
            *this = string;
        else
        {
            detach();

            (*d->s) += string;
        }
    }

    return (*this);
}

inline UtilString& UtilString::operator+=(const std::string &string)
{
    if (!string.empty())
    {
        if (isEmpty())
            *this = string;
        else
        {
            detach();

            (*d->s) += string;
        }
    }

    return (*this);
}

inline UtilString& UtilString::operator+=(const UtilString &string)
{
    if (!string.isEmpty())
    {
        if (isEmpty())
            *this = string;
        else
        {
            detach();

            (*d->s) += *string.d->s;
        }
    }
    return (*this);
}

inline const UtilString operator+(const UtilString &s1, const UtilString &s2)
{ UtilString t(s1); t+=s2; return t; }

inline const UtilString operator+(const UtilString &s1, const char *s2)
{ UtilString t(s1); t+=s2; return t; }

inline const UtilString operator+(const char *s1, const UtilString &s2)
{ UtilString t(s1); t+=s2; return t; }

inline const UtilString operator+(const UtilString &s1, const std::string &s2)
{ UtilString t(s1); t+=s2; return t; }

inline const UtilString operator+(const std::string &s1, const UtilString &s2)
{ UtilString t(s1); t+=s2; return t; }

#if 0
inline bool operator==(const UtilString &s1, const UtilString &s2)
{ return (s1 == s2); }

inline bool operator==(const UtilString &s1, const char *s2)
{ return s1 == s2; }

inline bool operator==(const UtilString &s1, const std::string &s2)
{ return s1 == s2; }

inline bool operator==(const char *s1, const UtilString &s2)
{ return s2 == s1; }

inline bool operator==(const std::string &s1, const UtilString &s2)
{ return s2 == s1; }

inline bool operator!=(const UtilString &s1, const UtilString &s2)
{ return s1 != s2; }

inline bool operator!=(const UtilString &s1, const char *s2)
{ return s1 != s2; }

inline bool operator!=(const UtilString &s1, const std::string &s2)
{ return s1 != s2; }

inline bool operator!=(const char *s1, const UtilString &s2)
{ return s2 != s1; }

inline bool operator!=(const std::string &s1, const UtilString &s2)
{ return s2 != s1; }
#endif
DM_END_NAMESPACE
#endif // DMSTRINGUTIL_H
