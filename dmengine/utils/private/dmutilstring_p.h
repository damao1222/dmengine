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

#ifndef DMWSTRING_H
#define DMWSTRING_H

#include "dmvector.h"
#include "StdString.h"

#define DM_NO_STRING_SHARING//implicit sharing

DM_BEGIN_NAMESPACE
typedef CStdStringW StdStr;
typedef wchar_t StdChar;
class WString;
class DM_DLL_EXPORT WStringList: public Vector<WString>
{
public:
    inline WStringList() { }
    inline explicit WStringList(const WString &i) { append(i); }
    inline WStringList(const WStringList &l) : Vector<WString>(l) { }
    inline WStringList(const Vector<WString> &l) : Vector<WString>(l) { }
};

struct DM_DLL_EXPORT WStringData {
    struct Data {
#ifdef DM_NO_STRING_SHARING
#else
        BasicAtomicInt ref;
        duint32 sharable : 1;
#endif
        StdStr s;
    };

    Data *create(const StdChar *s, size_t n = 0);
    Data *create(const StdStr &s);
    Data *detach();
    Data *detach(const StdChar *s);
    Data *detach(const StdStr &s);
    dbool isEmpty() const;
    static Data shared_null;
    Data *d;
};

class DM_DLL_EXPORT WString
{
    union { WStringData p; WStringData::Data *d; };

public:
    inline WString();
    inline WString(StdChar ch);
    inline WString(const char *s);
    inline WString(const StdChar *s);
    inline WString (const StdChar* s, size_t n);
    inline WString(const StdStr &s);
    inline WString(const WString &s);
    ~WString();

    // convert to int value
    int toInt() const;
    // convert to unsigned int value
    unsigned int toUint() const;
    // convert to float value
    float toFloat() const;
    // convert to double value
    double toDouble() const;
    //convert to dbool value
    bool toBool() const;

    dlonglong toLongLong() const;

    dulonglong toULongLong() const;

    //get the C string
    const StdChar* toCharStr() const;

    const StdChar& at(dint index) const;
    //StdChar& at(dint index);

    //get the length of string
    duint32 length() const;
    duint32 size() const;

    inline StdStr stdString() const;
    inline StdStr& asStdString();

    int indexOf(StdChar c, dint pos = 0) const;
    int indexOf(const WString &str, dint pos = 0) const;
    int lastIndexOf(StdChar c, dint pos = 0) const;
    int lastIndexOf(const WString &str, dint pos = 0) const;

	dbool beginWith(StdChar c) const;
	dbool beginWith(const WString &str) const;
	dbool endWith(StdChar c) const;
    dbool endWith(const WString &str) const;

    WString trimmed() const;
    WStringList split(StdChar c) const;
    WStringList split(const WString &str) const;

    WString subStr(size_t pos = 0, size_t len = -1) const;
	WString left(duint size) const;
	WString right(duint size) const;

    WString &erase(dint pos = 0, dint n = -1);
    WString &remove(dint pos = 0, dint n = -1)
    { return erase(pos, n); }

    WString &replace(StdChar from, StdChar to);
    WString &replace(const WString &from, const WString &to);

    inline dbool isEmpty() const;
    void clear();
    void reserve (duint n = 0);

    WString& lower();
    WString& upper();
    WString toLower() const;
    WString toUpper() const;

    inline dbool isDetached() const;
    inline void setSharable(dbool sharable);
    inline dbool isSharedWith(const WString &other) const;
    inline void detach();

    inline StdChar& operator[](dint i);
    inline const StdChar& operator[](dint i) const;
    inline WString& operator=(StdChar ch);
    inline WString& operator=(const char* s);
    inline WString& operator=(const StdChar *string);
    inline WString& operator=(const StdStr &string);
    inline WString& operator=(const WString &other);

    inline dbool operator==(const char *string) const;
    inline dbool operator==(const StdChar *string) const;
    inline dbool operator==(const StdStr &string) const;
    dbool operator==(const WString &other) const;

    inline dbool operator!=(const char *string) const;
    inline dbool operator!=(const StdChar *string) const;
    inline dbool operator!=(const StdStr &string) const;
    inline dbool operator!=(const WString &other) const;

    WString& operator+=(StdChar c);
    WString& operator+=(const StdChar *string);
    WString& operator+=(const StdStr &string);
    WString& operator+=(const WString &other);

    static WString fromAscii(const StdChar *s);
    static WString fromStdString(const StdStr &s);
    //NOTICE: limited DM_MEMORYNUM(see dmglobal.h) chars
    static WString sprintf(const StdChar *cformat, ...);

private:
    void detach_helper();
    void detach_helper(const StdChar *s);
    void detach_helper(const StdStr &s);
    void free(WStringData::Data *data);
};

#ifdef DM_NO_STRING_SHARING
inline WString::WString(): d(p.create(NULL))
{  }
#else
inline WString::WString(): d(&WStringData::shared_null)
{ d->ref.ref(); }
#endif

inline WString::WString(StdChar ch):
    d(p.create(StdStr(1, ch)))
{
}

inline WString::WString(const char *s):
    d(p.create(StdStr(s)))
{
}

inline WString::WString(const StdChar *s):
    d(p.create(s))
{
}

inline WString::WString (const StdChar* s, size_t n):
    d(p.create(s, n))
{

}

inline WString::WString(const StdStr &s):
    d(p.create(s))
{
}

#ifdef DM_NO_STRING_SHARING
inline WString::WString(const WString &s):
    d(p.create(s.d->s))
{  }
#else
inline WString::WString(const WString &s): d(s.d)
{ d->ref.ref(); if (!d->sharable) detach_helper(); }
#endif

inline duint32 WString::size() const
{ return d->s.length(); }

inline StdStr WString::stdString() const
{
    return d->s;
}

inline StdStr& WString::asStdString()
{
    return d->s;
}

#ifdef DM_NO_STRING_SHARING
inline dbool WString::isDetached() const { return true; }
inline void WString::setSharable(dbool sharable) { }
inline dbool WString::isSharedWith(const WString &other) const { return false; }
inline void WString::detach() { }
#else
inline dbool WString::isDetached() const { return d->ref == 1; }
inline void WString::setSharable(dbool sharable) { if (!sharable) detach(); d->sharable = sharable; }
inline dbool WString::isSharedWith(const WString &other) const { return d == other.d; }
inline void WString::detach() { if (d->ref != 1) detach_helper(); }
#endif

inline dbool WString::isEmpty() const { return p.isEmpty(); }

inline WString& WString::operator=(StdChar ch)
{
#ifdef DM_NO_STRING_SHARING
    d->s = ch;
    return *this;
#else
    return operator=(StdStr(1, ch));
#endif
}

inline WString& WString::operator=(const char* s)
{
#ifdef DM_NO_STRING_SHARING
    d->s = s;
    return *this;
#else
    detach_helper(s);
    return (*this);
#endif
}

inline WString& WString::operator=(const StdChar *s)
{
#ifdef DM_NO_STRING_SHARING
    d->s = s;
    return *this;
#else
    detach_helper(s);
    return (*this);
#endif
}

inline WString& WString::operator=(const StdStr &s)
{
#ifdef DM_NO_STRING_SHARING
    d->s = s;
    return *this;
#else
    detach_helper(s);
    return (*this);
#endif
}

inline WString& WString::operator=(const WString &s)
{
#ifdef DM_NO_STRING_SHARING
    d->s = s.d->s;
    return *this;
#else
    if (this == &s)
        return *this;

    if (d != s.d) {
        WStringData::Data *o = s.d;
        o->ref.ref();
        if (!d->ref.deref())
            free(d);
        d = o;
        if (!d->sharable)
            detach_helper();
    }
    return (*this);
#endif
}

inline dbool WString::operator!=(const char *s) const
{
    return !operator==(s);
}

inline dbool WString::operator!=(const StdChar *s) const
{
    return !operator==(s);
}

inline dbool WString::operator!=(const StdStr &s) const
{
    return !operator==(s);
}

inline dbool WString::operator!=(const WString &s) const
{
    return !operator==(s);
}

inline const WString operator+(const WString &s1, const WString &s2)
{ WString t(s1); t+=s2; return t; }

inline const WString operator+(const WString &s1, const StdChar *s2)
{ WString t(s1); t+=s2; return t; }

inline const WString operator+(const StdChar *s1, const WString &s2)
{ WString t(s1); t+=s2; return t; }

inline const WString operator+(const WString &s1, const StdStr &s2)
{ WString t(s1); t+=s2; return t; }

inline const WString operator+(const StdStr &s1, const WString &s2)
{ WString t(s1); t+=s2; return t; }
DM_END_NAMESPACE
#endif // DMWSTRING_H
