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

#ifndef DMSTRINGUTIL_H
#define DMSTRINGUTIL_H

#include <string>
#include "dmnamespace.h"
#include "dmvector.h"
#include "private/dmutilstring_p.h"
#include "dmchar.h"

#ifndef DM_NO_STRING_SHARING
#define DM_NO_STRING_SHARING//implicit sharing
#endif

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

class DM_DLL_EXPORT UtilString
{
    std::string _str;

public:
    /** 
     * Ĭ�Ϲ��캯��
     */
    inline UtilString();

    /** 
     * ʹ��һ���ַ�����
     * @param ch  �ַ�
     */
    inline UtilString(char ch);

    /** 
     * ʹ��һ���ַ�������
     * @param s  �ַ���
     */
    inline UtilString(const char *s);

    /** 
     * ʹ��һ���ַ�������
     * @param s  �ַ���
     * @param n  �ַ�������
     */
    inline UtilString (const char* s, size_t n);

    /** 
     * ʹ��һ��c++ string����
     * @param s  std string
     */
    inline UtilString(const std::string &s);

    /** 
     * �������캯��
     * @param s  ��������
     */
    inline UtilString(const UtilString &s);

    /** 
     * ��������
     */
    ~UtilString();

    /** 
     * תΪintֵ
     * @return intֵ
     */
    int toInt() const;
    
    /** 
     * תΪunsigned intֵ
     * @return unsigned intֵ
     */
    unsigned int toUint() const;
    
    /** 
     * תΪfloatֵ
     * @return floatֵ
     */
    float toFloat() const;
    
    /** 
     * תΪdoubleֵ
     * @return doubleֵ
     */
    double toDouble() const;
    
    /** 
     * תΪboolֵ
     * @return boolֵ
     */
    bool toBool() const;

    /** 
     * תΪlonglongֵ
     * @return longlongֵ
     */
    dlonglong toLongLong() const;

    /** 
     * תΪunsigned longlongֵ
     * @return unsigned longlongֵ
     */
    dulonglong toULongLong() const;

    /** 
     * תΪ�ַ���
     * @return �ַ���
     */
    const char* toCharStr() const;

    /** 
     * ��������ϵ��ַ�
     * @param index  ����
     * @return �ַ�
     */
    const char& at(dint index) const;

    /** 
     * ����ִ�����
     * @return ����
     */
    duint32 length() const;

    /** 
     * ����ִ�����
     * @return ����
     */
    duint32 size() const;

    /** 
     * ת��Ϊstd string
     * @return std string
     */
    inline std::string stdString() const;

    /** 
     * ����ڲ�std string
     * @return std string����
     */
    inline std::string& asStdString();

    /** 
     * �����ַ����ִ��е�λ��
     * @param c  �ַ�
     * @param pos  ��ʼλ��
     * @return �ַ����ִ��е�λ�ã�û�в��ҵ�����-1
     */
    dint indexOf(char c, dint pos = 0) const;

    /** 
     * �����ִ����ִ��е�λ��
     * @param str  �ִ�
     * @param pos  ��ʼλ��
     * @return �ִ����ִ��е�λ�ã�û�в��ҵ�����-1
     */
    dint indexOf(const UtilString &str, dint pos = 0) const;

    /** 
     * ��ĩβ��ʼ�����ַ����ִ��е�λ��
     * @param c  �ַ�
     * @param pos  ��ʼλ��
     * @return �ַ����ִ��е�λ�ã�û�в��ҵ�����-1
     */
    dint lastIndexOf(char c, dint pos = -1) const;

    /** 
     * ��ĩβ��ʼ�����ִ����ִ��е�λ��
     * @param str  �ִ�
     * @param pos  ��ʼλ��
     * @return �ִ����ִ��е�λ�ã�û�в��ҵ�����-1
     */
    dint lastIndexOf(const UtilString &str, dint pos = -1) const;

    /** 
     * �ж��ִ��Ƿ����ַ���ʼ
     * @param c  ��ʼ�ַ�
     * @return ��c��ʼ����true�����򷵻�false
     */
    dbool beginWith(char c) const;

    /** 
     * �ж��ִ��Ƿ���str��ʼ
     * @param str  ��ʼ�ִ�
     * @return ��str��ʼ����true�����򷵻�false
     */
    dbool beginWith(const UtilString &str) const;

    /** 
     * �ж��ִ��Ƿ����ַ���β
     * @param c  ��β�ַ�
     * @return ��c��β����true�����򷵻�false
     */
    dbool endWith(char c) const;

    /** 
     * �ж��ִ��Ƿ���str��β
     * @param str  ��β�ִ�
     * @return ��str��β����true�����򷵻�false
     */
    dbool endWith(const UtilString &str) const;

    /** 
     * ���ȥ����ͷ�ͽ�β���������пո���Ʊ�����ִ�
     * @return ���˺���ִ�
     */
    UtilString trimmed() const;

    /** 
     * ���ַ�c�ָ��ִ������һ���ִ��飬����޷ָ���Լ�
     * @param c  �ָ��ַ�
     * @return �ָ����ִ���
     */
    UtilStringList split(char c) const;

    /** 
     * ���ִ�str�ָ��ִ������һ���ִ��飬����޷ָ���Լ�
     * @param str  �ָ��ִ�
     * @return �ָ����ִ���
     */
    UtilStringList split(const UtilString &str) const;

    /** 
     * ת��ΪСд�ִ�
     * @return ת������������
     */
    UtilString& lower();

    /** 
     * ת��Ϊ��д�ִ�
     * @return ת������������
     */
    UtilString& upper();

    /** 
     * ��õ�ǰ�ִ���Сд����
     * @return Сд�ִ�
     */
    UtilString toLower() const;

    /** 
     * ��õ�ǰ�ִ��Ĵ�д����
     * @return ��д�ִ�
     */
    UtilString toUpper() const;

    /** 
     * �ж��ִ��Ƿ���str���
     * @param str  �Ƚ��ִ�
     * @param sensitive  �Ƿ��Сд����
     * @return ��ȷ���true�����򷵻�false
     */
    dbool equals(const UtilString &str, dbool sensitive = false) const;

    /** 
     * �ж��ִ��Ƿ���str���
     * @param str  �Ƚ��ִ�
     * @param sensitive  �Ƿ��Сд����
     * @return ��ȷ���true�����򷵻�false
     */
    dbool compare(const UtilString &str, dbool sensitive = false) const;

    /** 
     * ���pos��ʼ������Ϊlen�����ִ�
     * @param pos  ��ʼλ��
     * @param len  ��ȡ����
     * @return ���ִ�
     */
    UtilString subStr(size_t pos = 0, size_t len = -1) const;

	/** 
     * ��ô�0��ʼ����size��С�����ִ�
     * @param size  ��ʼλ��
     * @return ���ִ�
     */
    UtilString left(duint size) const;

	/** 
     * ��ô�ĩβ��ʼ����size��С�����ִ�
     * @param size  ��ʼλ��
     * @return ���ִ�
     */
    UtilString right(duint size) const;

    /** 
     * ɾ��pos��ʼ������Ϊn���ַ�
     * @param pos  ��ʼλ��
     * @param n  ɾ���ַ�����
     * @return ɾ����ĵ�ǰ����
     */
    UtilString &erase(dint pos = 0, dint n = -1);

    /** 
     * ɾ��pos��ʼ������Ϊn���ַ�
     * @param pos  ��ʼλ��
     * @param n  ɾ���ַ�����
     * @return ɾ����ĵ�ǰ����
     */
    UtilString &remove(dint pos = 0, dint n = -1)
    { return erase(pos, n); }

    /** 
     * ʹ��to�ַ��滻���е�from�ַ�
     * @param from  ���滻���ַ�
     * @param to  �滻��Ŀ���ַ�
     * @return �滻��ĵ�ǰ����
     */
    UtilString &replace(char from, char to);

    /** 
     * ʹ��to�ַ����滻���е�from�ַ���
     * @param from  ���滻���ַ���
     * @param to  �滻��Ŀ���ַ���
     * @return �滻��ĵ�ǰ����
     */
    UtilString &replace(const UtilString &from, const UtilString &to);

    /** 
     * �ж��ִ��Ƿ�Ϊ��
     * @return Ϊ�շ���true�����򷵻�false
     */
    inline dbool isEmpty() const;

    /** 
     * ����ִ�����
     */
    void clear();

    /** 
     * ����һ�����ڴ�������һ���������ַ�
     * @param n  �����ڴ��С
     */
    void reserve (duint n = 0);

    /** 
     * []��������أ��������λ�õ��ַ�
     * @param i  ����
     * @return ����λ�õ��ַ�����
     */
    inline char& operator[](dint i);

    /** 
     * []��������أ��������λ�õ��ַ�
     * @param i  ����
     * @return ����λ�õ��ַ�
     */
    inline const char& operator[](dint i) const;

    /** 
     * ��ֵ�������ʹ���ַ�ch��ֵ
     * @param ch  �ַ�
     * @return ��ֵ��ĵ�ǰ����
     */
    inline UtilString& operator=(char ch);

    /** 
     * ��ֵ�������ʹ���ִ�string��ֵ
     * @param string  �ַ���
     * @return ��ֵ��ĵ�ǰ����
     */
    inline UtilString& operator=(const char *string);

    /** 
     * ��ֵ�������ʹ��std string��ֵ
     * @param string  �ִ�
     * @return ��ֵ��ĵ�ǰ����
     */
    inline UtilString& operator=(const std::string &string);

    /** 
     * ��ֵ�������ʹ��UtilString��ֵ
     * @param other  ��ֵ����
     * @return ��ֵ��ĵ�ǰ����
     */
    inline UtilString& operator=(const UtilString &other);

    /** 
     * ������������أ���Сд����
     * @param string  �Ƚ��ִ�
     * @return ��ȷ���true�����򷵻�false
     */
    dbool operator==(const char *string) const;

    /** 
     * ������������أ���Сд����
     * @param string  �Ƚ��ִ�
     * @return ��ȷ���true�����򷵻�false
     */
    dbool operator==(const std::string &string) const;

    /** 
     * ������������أ���Сд����
     * @param other  �Ƚ��ִ�
     * @return ��ȷ���true�����򷵻�false
     */
    dbool operator==(const UtilString &other) const;

    /** 
     * ������������أ���Сд����
     * @param string  �Ƚ��ִ�
     * @return ����ȷ���true�����򷵻�false
     */
    inline dbool operator!=(const char *string) const;

    /** 
     * ������������أ���Сд����
     * @param string  �Ƚ��ִ�
     * @return ����ȷ���true�����򷵻�false
     */
    inline dbool operator!=(const std::string &string) const;

    /** 
     * ������������أ���Сд����
     * @param other  �Ƚ��ִ�
     * @return ����ȷ���true�����򷵻�false
     */
    inline dbool operator!=(const UtilString &other) const;

    /** 
     * �ӵ���������أ����ִ�ĩβ׷���ַ�
     * @param c  ׷�ӵ��ַ�
     * @return ��ǰʵ��������
     */
    UtilString& operator+=(char c);

    /** 
     * �ӵ���������أ����ִ�ĩβ׷���ִ�
     * @param string  ׷�ӵ��ִ�
     * @return ��ǰʵ��������
     */
    UtilString& operator+=(const char *string);

    /** 
     * �ӵ���������أ����ִ�ĩβ׷���ִ�
     * @param string  ׷�ӵ��ִ�
     * @return ��ǰʵ��������
     */
    UtilString& operator+=(const std::string &string);

    /** 
     * �ӵ���������أ����ִ�ĩβ׷���ִ�
     * @param other  ׷�ӵ��ִ�
     * @return ��ǰʵ��������
     */
    UtilString& operator+=(const UtilString &other);

    /** 
     * ʹ���ִ�����
     * @param s  ����UtilString���ִ�
     * @return �������ִ�
     */
    static UtilString fromAscii(const char *s);

    /** 
     * ʹ��std string����
     * @param s  ����UtilString���ִ�
     * @return �������ִ�
     */
    static UtilString fromStdString(const std::string &s);

    /** 
     * ʹ�ø�ʽ����
     * @param cformat  �����ʽ
     * @return �������ִ�
     */
    static UtilString sprintf(const char *cformat, ...);

    /** 
     * ���WString
     * @return ��ǰʵ����WString
     */
    WString toWString() const;

    void setSharable(dbool share) { DM_UNUSED(share); }
};

inline UtilString::UtilString()
{  }

inline UtilString::UtilString(char ch):
    _str(1, ch)
{
}

inline UtilString::UtilString(const char *s):
    _str(s)
{
}

inline UtilString::UtilString (const char* s, size_t n):
    _str(s, n)
{

}

inline UtilString::UtilString(const std::string &s):
    _str(s)
{
}

inline UtilString::UtilString(const UtilString &s):
    _str(s._str)
{  }

inline duint32 UtilString::size() const
{ return _str.length(); }

inline std::string UtilString::stdString() const
{
    return _str;
}

inline std::string& UtilString::asStdString()
{
    return _str;
}

inline dbool UtilString::isEmpty() const { return _str.empty(); }

inline UtilString& UtilString::operator=(char ch)
{
    _str = ch;
    return *this;
}

inline UtilString& UtilString::operator=(const char *s)
{
    _str = s;
    return *this;
}

inline UtilString& UtilString::operator=(const std::string &s)
{
    _str = s;
    return *this;
}

inline UtilString& UtilString::operator=(const UtilString &s)
{
    _str = s._str;
    return *this;
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
