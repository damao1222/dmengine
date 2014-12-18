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

#ifndef DMWSTRING_H
#define DMWSTRING_H

#include "dmvector.h"
#include "StdString.h"

#ifndef DM_NO_STRING_SHARING
#define DM_NO_STRING_SHARING//implicit sharing
#endif

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

class DM_DLL_EXPORT WString
{
    StdStr _str;

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

    inline StdChar& operator[](dint i);
    inline const StdChar& operator[](dint i) const;
    inline WString& operator=(StdChar ch);
    inline WString& operator=(const char* s);
    inline WString& operator=(const StdChar *string);
    inline WString& operator=(const StdStr &string);
    inline WString& operator=(const WString &other);

    dbool operator==(const char *string) const;
    dbool operator==(const StdChar *string) const;
    dbool operator==(const StdStr &string) const;
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

    void setSharable(dbool share) { DM_UNUSED(share); }
};

inline WString::WString()
{  }

inline WString::WString(StdChar ch):
    _str(ch, 1)
{
}

inline WString::WString(const char *s):
    _str(s)
{
}

inline WString::WString(const StdChar *s):
    _str(s)
{
}

inline WString::WString (const StdChar* s, size_t n):
    _str(StdStr(s, n))
{

}

inline WString::WString(const StdStr &s):
    _str(s)
{
}

inline WString::WString(const WString &s):
    _str(s._str)
{  }

inline duint32 WString::size() const
{ return _str.length(); }

inline StdStr WString::stdString() const
{
    return _str;
}

inline StdStr& WString::asStdString()
{
    return _str;
}

inline dbool WString::isEmpty() const { return _str.empty(); }

inline WString& WString::operator=(StdChar ch)
{
    _str = ch;
    return *this;
}

inline WString& WString::operator=(const char* s)
{
    _str = s;
    return *this;
}

inline WString& WString::operator=(const StdChar *s)
{
    _str = s;
    return *this;
}

inline WString& WString::operator=(const StdStr &s)
{
    _str = s;
    return *this;
}

inline WString& WString::operator=(const WString &s)
{
    _str = s._str;
    return *this;
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
