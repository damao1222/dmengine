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

#ifndef DMSTRINGUTIL_H
#define DMSTRINGUTIL_H

#include <string>
#include "dmnamespace.h"
#include "dmvector.h"
#include "private/dmutilstring_p.h"
#include "dmchar.h"

#define DM_NO_STRING_SHARING//implicit sharing

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
#ifdef DM_NO_STRING_SHARING
#else
        BasicAtomicInt ref;
        duint32 sharable : 1;
#endif
        std::string s;
    };

    Data *create(const char *s, size_t n = 0);
    Data *create(const std::string &s);
    Data *detach();
    Data *detach(const char *s);
    Data *detach(const std::string &s);
    dbool isEmpty() const;
    static Data shared_null;
    Data *d;
};

class DM_DLL_EXPORT UtilString
{
    union { StringData p; StringData::Data *d; };

public:
    /** 
     * 默认构造函数
     */
    inline UtilString();

    /** 
     * 使用一个字符构造
     * @param ch  字符
     */
    inline UtilString(char ch);

    /** 
     * 使用一个字符串构造
     * @param s  字符串
     */
    inline UtilString(const char *s);

    /** 
     * 使用一个字符串构造
     * @param s  字符串
     * @param n  字符串长度
     */
    inline UtilString (const char* s, size_t n);

    /** 
     * 使用一个c++ string构造
     * @param s  std string
     */
    inline UtilString(const std::string &s);

    /** 
     * 拷贝构造函数
     * @param s  拷贝对象
     */
    inline UtilString(const UtilString &s);

    /** 
     * 析构函数
     */
    ~UtilString();

    /** 
     * 转为int值
     * @return int值
     */
    int toInt() const;
    
    /** 
     * 转为unsigned int值
     * @return unsigned int值
     */
    unsigned int toUint() const;
    
    /** 
     * 转为float值
     * @return float值
     */
    float toFloat() const;
    
    /** 
     * 转为double值
     * @return double值
     */
    double toDouble() const;
    
    /** 
     * 转为bool值
     * @return bool值
     */
    bool toBool() const;

    /** 
     * 转为longlong值
     * @return longlong值
     */
    dlonglong toLongLong() const;

    /** 
     * 转为unsigned longlong值
     * @return unsigned longlong值
     */
    dulonglong toULongLong() const;

    /** 
     * 转为字符串
     * @return 字符串
     */
    const char* toCharStr() const;

    /** 
     * 获得索引上的字符
     * @param index  索引
     * @return 字符
     */
    const char& at(dint index) const;

    /** 
     * 获得字串长度
     * @return 长度
     */
    duint32 length() const;

    /** 
     * 获得字串长度
     * @return 长度
     */
    duint32 size() const;

    /** 
     * 转换为std string
     * @return std string
     */
    inline std::string stdString() const;

    /** 
     * 获得内部std string
     * @return std string引用
     */
    inline std::string& asStdString();

    /** 
     * 查找字符在字串中的位置
     * @param c  字符
     * @param pos  起始位置
     * @return 字符在字串中的位置，没有查找到返回-1
     */
    dint indexOf(char c, dint pos = 0) const;

    /** 
     * 查找字串在字串中的位置
     * @param str  字串
     * @param pos  起始位置
     * @return 字串在字串中的位置，没有查找到返回-1
     */
    dint indexOf(const UtilString &str, dint pos = 0) const;

    /** 
     * 从末尾开始查找字符在字串中的位置
     * @param c  字符
     * @param pos  起始位置
     * @return 字符在字串中的位置，没有查找到返回-1
     */
    dint lastIndexOf(char c, dint pos = -1) const;

    /** 
     * 从末尾开始查找字串在字串中的位置
     * @param str  字串
     * @param pos  起始位置
     * @return 字串在字串中的位置，没有查找到返回-1
     */
    dint lastIndexOf(const UtilString &str, dint pos = -1) const;

    /** 
     * 判断字串是否以字符起始
     * @param c  起始字符
     * @return 以c起始返回true，否则返回false
     */
    dbool beginWith(char c) const;

    /** 
     * 判断字串是否以str起始
     * @param str  起始字串
     * @return 以str起始返回true，否则返回false
     */
    dbool beginWith(const UtilString &str) const;

    /** 
     * 判断字串是否以字符结尾
     * @param c  结尾字符
     * @return 以c结尾返回true，否则返回false
     */
    dbool endWith(char c) const;

    /** 
     * 判断字串是否以str结尾
     * @param str  结尾字串
     * @return 以str结尾返回true，否则返回false
     */
    dbool endWith(const UtilString &str) const;

    /** 
     * 获得去掉开头和结尾包含的所有空格和制表符的字串
     * @return 过滤后的字串
     */
    UtilString trimmed() const;

    /** 
     * 用字符c分割字串，组成一个字串组，如果无分割返回自己
     * @param c  分割字符
     * @return 分割后的字串组
     */
    UtilStringList split(char c) const;

    /** 
     * 用字串str分割字串，组成一个字串组，如果无分割返回自己
     * @param str  分割字串
     * @return 分割后的字串组
     */
    UtilStringList split(const UtilString &str) const;

    /** 
     * 转换为小写字串
     * @return 转换后自身引用
     */
    UtilString& lower();

    /** 
     * 转换为大写字串
     * @return 转换后自身引用
     */
    UtilString& upper();

    /** 
     * 获得当前字串的小写拷贝
     * @return 小写字串
     */
    UtilString toLower() const;

    /** 
     * 获得当前字串的大写拷贝
     * @return 大写字串
     */
    UtilString toUpper() const;

    /** 
     * 判断字串是否与str相等
     * @param str  比较字串
     * @param sensitive  是否大小写敏感
     * @return 相等返回true，否则返回false
     */
    dbool equals(const UtilString &str, dbool sensitive = false) const;

    /** 
     * 判断字串是否与str相等
     * @param str  比较字串
     * @param sensitive  是否大小写敏感
     * @return 相等返回true，否则返回false
     */
    dbool compare(const UtilString &str, dbool sensitive = false) const;

    /** 
     * 获得pos起始，长度为len的子字串
     * @param pos  起始位置
     * @param len  截取长度
     * @return 子字串
     */
    UtilString subStr(size_t pos = 0, size_t len = -1) const;

	/** 
     * 获得从0开始右数size大小的子字串
     * @param size  起始位置
     * @return 子字串
     */
    UtilString left(duint size) const;

	/** 
     * 获得从末尾开始左数size大小的子字串
     * @param size  起始位置
     * @return 子字串
     */
    UtilString right(duint size) const;

    /** 
     * 删除pos起始，长度为n的字符
     * @param pos  起始位置
     * @param n  删除字符个数
     * @return 删除后的当前引用
     */
    UtilString &erase(dint pos = 0, dint n = -1);

    /** 
     * 删除pos起始，长度为n的字符
     * @param pos  起始位置
     * @param n  删除字符个数
     * @return 删除后的当前引用
     */
    UtilString &remove(dint pos = 0, dint n = -1)
    { return erase(pos, n); }

    /** 
     * 使用to字符替换所有的from字符
     * @param from  被替换的字符
     * @param to  替换的目标字符
     * @return 替换后的当前引用
     */
    UtilString &replace(char from, char to);

    /** 
     * 使用to字符串替换所有的from字符串
     * @param from  被替换的字符串
     * @param to  替换的目标字符串
     * @return 替换后的当前引用
     */
    UtilString &replace(const UtilString &from, const UtilString &to);

    /** 
     * 判断字串是否为空
     * @return 为空返回true，否则返回false
     */
    inline dbool isEmpty() const;

    /** 
     * 清除字串内容
     */
    void clear();

    /** 
     * 保留一定量内存以容纳一定数量的字符
     * @param n  保留内存大小
     */
    void reserve (duint n = 0);

    inline dbool isDetached() const;
    inline void setSharable(dbool sharable);
    inline dbool isSharedWith(const UtilString &other) const;
    inline void detach();

    /** 
     * []运算符重载，获得索引位置的字符
     * @param i  索引
     * @return 索引位置的字符引用
     */
    inline char& operator[](dint i);

    /** 
     * []运算符重载，获得索引位置的字符
     * @param i  索引
     * @return 索引位置的字符
     */
    inline const char& operator[](dint i) const;

    /** 
     * 赋值运算符，使用字符ch赋值
     * @param ch  字符
     * @return 赋值后的当前引用
     */
    inline UtilString& operator=(char ch);

    /** 
     * 赋值运算符，使用字串string赋值
     * @param string  字符串
     * @return 赋值后的当前引用
     */
    inline UtilString& operator=(const char *string);

    /** 
     * 赋值运算符，使用std string赋值
     * @param string  字串
     * @return 赋值后的当前引用
     */
    inline UtilString& operator=(const std::string &string);

    /** 
     * 赋值运算符，使用UtilString赋值
     * @param other  赋值对象
     * @return 赋值后的当前引用
     */
    inline UtilString& operator=(const UtilString &other);

    /** 
     * 等于运算符重载，大小写敏感
     * @param string  比较字串
     * @return 相等返回true，否则返回false
     */
    inline dbool operator==(const char *string) const;

    /** 
     * 等于运算符重载，大小写敏感
     * @param string  比较字串
     * @return 相等返回true，否则返回false
     */
    inline dbool operator==(const std::string &string) const;

    /** 
     * 等于运算符重载，大小写敏感
     * @param other  比较字串
     * @return 相等返回true，否则返回false
     */
    dbool operator==(const UtilString &other) const;

    /** 
     * 不等运算符重载，大小写敏感
     * @param string  比较字串
     * @return 不相等返回true，否则返回false
     */
    inline dbool operator!=(const char *string) const;

    /** 
     * 不等运算符重载，大小写敏感
     * @param string  比较字串
     * @return 不相等返回true，否则返回false
     */
    inline dbool operator!=(const std::string &string) const;

    /** 
     * 不等运算符重载，大小写敏感
     * @param other  比较字串
     * @return 不相等返回true，否则返回false
     */
    inline dbool operator!=(const UtilString &other) const;

    /** 
     * 加等运算符重载，在字串末尾追加字符
     * @param c  追加的字符
     * @return 当前实例的引用
     */
    UtilString& operator+=(char c);

    /** 
     * 加等运算符重载，在字串末尾追加字串
     * @param string  追加的字串
     * @return 当前实例的引用
     */
    UtilString& operator+=(const char *string);

    /** 
     * 加等运算符重载，在字串末尾追加字串
     * @param string  追加的字串
     * @return 当前实例的引用
     */
    UtilString& operator+=(const std::string &string);

    /** 
     * 加等运算符重载，在字串末尾追加字串
     * @param other  追加的字串
     * @return 当前实例的引用
     */
    UtilString& operator+=(const UtilString &other);

    /** 
     * 使用字串构造
     * @param s  构造UtilString的字串
     * @return 构造后的字串
     */
    static UtilString fromAscii(const char *s);

    /** 
     * 使用std string构造
     * @param s  构造UtilString的字串
     * @return 构造后的字串
     */
    static UtilString fromStdString(const std::string &s);

    /** 
     * 使用格式构造
     * @param cformat  构造格式
     * @return 构造后的字串
     */
    static UtilString sprintf(const char *cformat, ...);

    /** 
     * 获得WString
     * @return 当前实例的WString
     */
    WString toWString() const;
private:
    void detach_helper();
    void detach_helper(const char *s);
    void detach_helper(const std::string &s);
    void free(StringData::Data *data);
};

#ifdef DM_NO_STRING_SHARING
inline UtilString::UtilString(): d(p.create(NULL))
{  }
#else
inline UtilString::UtilString(): d(&StringData::shared_null)
{ d->ref.ref(); }
#endif

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

#ifdef DM_NO_STRING_SHARING
inline UtilString::UtilString(const UtilString &s):
    d(p.create(s.d->s))
{  }
#else
inline UtilString::UtilString(const UtilString &s): d(s.d)
{ d->ref.ref(); if (!d->sharable) detach_helper(); }
#endif

inline duint32 UtilString::size() const
{ return d->s.length(); }

inline std::string UtilString::stdString() const
{
    return d->s;
}

inline std::string& UtilString::asStdString()
{
    return d->s;
}

#ifdef DM_NO_STRING_SHARING
inline dbool UtilString::isDetached() const { return true; }
inline void UtilString::setSharable(dbool sharable) { }
inline dbool UtilString::isSharedWith(const UtilString &other) const { return false; }
inline void UtilString::detach() { }
#else
inline dbool UtilString::isDetached() const { return d->ref == 1; }
inline void UtilString::setSharable(dbool sharable) { if (!sharable) detach(); d->sharable = sharable; }
inline dbool UtilString::isSharedWith(const UtilString &other) const { return d == other.d; }
inline void UtilString::detach() { if (d->ref != 1) detach_helper(); }
#endif

inline dbool UtilString::isEmpty() const { return p.isEmpty(); }

inline UtilString& UtilString::operator=(char ch)
{
#ifdef DM_NO_STRING_SHARING
    d->s = ch;
    return *this;
#else
    return operator=(std::string(1, ch));
#endif
}

inline UtilString& UtilString::operator=(const char *s)
{
#ifdef DM_NO_STRING_SHARING
    d->s = s;
    return *this;
#else
    detach_helper(s);
    return (*this);
#endif
}

inline UtilString& UtilString::operator=(const std::string &s)
{
#ifdef DM_NO_STRING_SHARING
    d->s = s;
    return *this;
#else
    detach_helper(s);
    return (*this);
#endif
}

inline UtilString& UtilString::operator=(const UtilString &s)
{
#ifdef DM_NO_STRING_SHARING
    d->s = s.d->s;
    return *this;
#else
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
#endif
}

inline dbool UtilString::operator!=(const char *s) const
{
    return !operator==(s);
}

inline dbool UtilString::operator!=(const std::string &s) const
{
    return !operator==(s);
}

inline dbool UtilString::operator!=(const UtilString &s) const
{
    return !operator==(s);
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
inline dbool operator==(const UtilString &s1, const UtilString &s2)
{ return (s1 == s2); }

inline dbool operator==(const UtilString &s1, const char *s2)
{ return s1 == s2; }

inline dbool operator==(const UtilString &s1, const std::string &s2)
{ return s1 == s2; }

inline dbool operator==(const char *s1, const UtilString &s2)
{ return s2 == s1; }

inline dbool operator==(const std::string &s1, const UtilString &s2)
{ return s2 == s1; }

inline dbool operator!=(const UtilString &s1, const UtilString &s2)
{ return s1 != s2; }

inline dbool operator!=(const UtilString &s1, const char *s2)
{ return s1 != s2; }

inline dbool operator!=(const UtilString &s1, const std::string &s2)
{ return s1 != s2; }

inline dbool operator!=(const char *s1, const UtilString &s2)
{ return s2 != s1; }

inline dbool operator!=(const std::string &s1, const UtilString &s2)
{ return s2 != s1; }
#endif

#ifdef UNICODE
typedef WString UrlString;
typedef WStringList UrlStringList;
#else
typedef UtilString UrlString;
typedef UtilStringList UrlStringList;
#endif

DM_END_NAMESPACE
#endif // DMSTRINGUTIL_H
