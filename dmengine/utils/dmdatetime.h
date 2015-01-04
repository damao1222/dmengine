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
#ifndef DMDATETIME_H
#define DMDATETIME_H
#include "dmnamespace.h"
DM_BEGIN_NAMESPACE
class DateTime;
DM_PRIVATE_CLASS(Date);
class DM_DLL_EXPORT Date
{
    DM_DECLARE_PRIVATE(Date);
public:
    /** 
     * 构造函数
     */
    Date();

    /** 
     * 拷贝构造函数
     * @param other  拷贝对象
     */
    Date(const Date &other);

    /** 
     * 析构函数
     */
    ~Date();

    /** 
     * 获得年份
     * @return 年份
     */
    dint year() const;

    /** 
     * 获得月份
     * @return 月份
     */
    dint month() const;

    /** 
     * 获得日
     * @return 日
     */
    dint day() const;

    /** 
     * 获得年月日
     * @param pYear  年输出地址
     * @param pMonth  月输出地址
     * @param pDay  日输出地址
     */
    void get(dint *pYear, dint *pMonth, dint *pDay);

    /** 
     * 获得当前日期
     * @return 当前日期
     */
    static Date currentDate();

    /** 
     * 赋值运算符重载
     * @param other  拷贝对象
     * @return 赋值后当前对象的引用
     */
    Date& operator=(const Date &other);

    /** 
     * 等于运算符重载
     * @param other  比较对象
     * @return 相等返回true，否则返回false
     */
    dbool operator==(const Date &other) const;

    /** 
     * 不等运算符重载
     * @param other  比较对象
     * @return 不相等返回true，否则返回false
     */
    dbool operator!=(const Date &other) const;

    //internal
    friend class DateTime;
};

DM_PRIVATE_CLASS(Time);
class DM_DLL_EXPORT Time
{
    DM_DECLARE_PRIVATE(Time);
public:
    /** 
     * 拷贝构造函数
     */
    Time();

    /** 
     * 拷贝构造函数
     * @param other  拷贝对象
     */
    Time(const Time &other);

    /** 
     * 析构函数
     */
    ~Time();

    /** 
     * 获得小时
     * @return 小时
     */
    dint hour() const;

    /** 
     * 获得分钟
     * @return 分钟
     */
    dint minute() const;

    /** 
     * 获得秒数
     * @return 秒数
     */
    dint second() const;

    /** 
     * 获得毫秒数
     * @return 毫秒数
     */
    dint msec() const;

    /** 
     * 获得当前时间
     * @return 当前时间
     */
    static Time currentTime();

    /** 
     * 获得当前秒
     * @return 秒数
     */
    static duint getSecond();

    /** 
     * 获得当前毫秒数
     * @return 毫秒数
     */
    static duint getMillisecond();

    /** 
     * 获得当前微秒数
     * @return 微秒数
     */
    static duint getMicrosecond();

    /** 
     * 获得应用运行的总时间（第一次调用此接口时开始计算，单位毫秒）
     * @return 毫秒数
     */
    static duint getAppClockMillis();

    /** 
     * 赋值运算符重载
     * @param other  拷贝对象
     * @return 赋值后当前对象的引用
     */
    Time& operator=(const Time &other);

    /** 
     * 等于运算符重载
     * @param other  比较对象
     * @return 相等返回true，否则返回false
     */
    dbool operator==(const Time &other) const;

    /** 
     * 不等运算符重载
     * @param other  比较对象
     * @return 不相等返回true，否则返回false
     */
    dbool operator!=(const Time &other) const;

    //internal
    friend class DateTime;
};

DM_PRIVATE_CLASS(DateTime);
class DM_DLL_EXPORT DateTime
{
    DM_DECLARE_PRIVATE(DateTime)
public:
    /** 
     * 默认构造函数
     */
    DateTime();

    /** 
     * 使用日期和时间构造函数
     * @param date  日期
     * @param time  时间
     */
    DateTime(const Date &date, const Time &time);

    /** 
     * 拷贝构造函数
     * @param other  拷贝对象
     */
    DateTime(const DateTime &other);

    /** 
     * 析构函数
     */
    ~DateTime();

    /** 
     * 获得日期
     * @return 日期
     */
    Date date() const;

    /** 
     * 获得时间
     * @return 时间
     */
    Time time() const;

    /** 
     * 获得当前日期时间
     * @return 当前日期时间
     */
    static DateTime currentDateTime();

    /** 
     * 赋值运算符重载
     * @param other  拷贝对象
     * @return 赋值后当前对象的引用
     */
    DateTime& operator=(const DateTime &other);

    /** 
     * 等于运算符重载
     * @param other  比较对象
     * @return 相等返回true，否则返回false
     */
    dbool operator==(const DateTime &other) const;

    /** 
     * 不等运算符重载
     * @param other  比较对象
     * @return 不相等返回true，否则返回false
     */
    dbool operator!=(const DateTime &other) const;
};
DM_END_NAMESPACE
#endif // DMDATETIME_H
