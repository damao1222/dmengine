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
     * ���캯��
     */
    Date();

    /** 
     * �������캯��
     * @param other  ��������
     */
    Date(const Date &other);

    /** 
     * ��������
     */
    ~Date();

    /** 
     * ������
     * @return ���
     */
    dint year() const;

    /** 
     * ����·�
     * @return �·�
     */
    dint month() const;

    /** 
     * �����
     * @return ��
     */
    dint day() const;

    /** 
     * ���������
     * @param pYear  �������ַ
     * @param pMonth  �������ַ
     * @param pDay  �������ַ
     */
    void get(dint *pYear, dint *pMonth, dint *pDay);

    /** 
     * ��õ�ǰ����
     * @return ��ǰ����
     */
    static Date currentDate();

    /** 
     * ��ֵ���������
     * @param other  ��������
     * @return ��ֵ��ǰ���������
     */
    Date& operator=(const Date &other);

    /** 
     * �������������
     * @param other  �Ƚ϶���
     * @return ��ȷ���true�����򷵻�false
     */
    dbool operator==(const Date &other) const;

    /** 
     * �������������
     * @param other  �Ƚ϶���
     * @return ����ȷ���true�����򷵻�false
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
     * �������캯��
     */
    Time();

    /** 
     * �������캯��
     * @param other  ��������
     */
    Time(const Time &other);

    /** 
     * ��������
     */
    ~Time();

    /** 
     * ���Сʱ
     * @return Сʱ
     */
    dint hour() const;

    /** 
     * ��÷���
     * @return ����
     */
    dint minute() const;

    /** 
     * �������
     * @return ����
     */
    dint second() const;

    /** 
     * ��ú�����
     * @return ������
     */
    dint msec() const;

    /** 
     * ��õ�ǰʱ��
     * @return ��ǰʱ��
     */
    static Time currentTime();

    /** 
     * ��õ�ǰ��
     * @return ����
     */
    static duint getSecond();

    /** 
     * ��õ�ǰ������
     * @return ������
     */
    static duint getMillisecond();

    /** 
     * ��õ�ǰ΢����
     * @return ΢����
     */
    static duint getMicrosecond();

    /** 
     * ���Ӧ�����е���ʱ�䣨��һ�ε��ô˽ӿ�ʱ��ʼ���㣬��λ���룩
     * @return ������
     */
    static duint getAppClockMillis();

    /** 
     * ��ֵ���������
     * @param other  ��������
     * @return ��ֵ��ǰ���������
     */
    Time& operator=(const Time &other);

    /** 
     * �������������
     * @param other  �Ƚ϶���
     * @return ��ȷ���true�����򷵻�false
     */
    dbool operator==(const Time &other) const;

    /** 
     * �������������
     * @param other  �Ƚ϶���
     * @return ����ȷ���true�����򷵻�false
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
     * Ĭ�Ϲ��캯��
     */
    DateTime();

    /** 
     * ʹ�����ں�ʱ�乹�캯��
     * @param date  ����
     * @param time  ʱ��
     */
    DateTime(const Date &date, const Time &time);

    /** 
     * �������캯��
     * @param other  ��������
     */
    DateTime(const DateTime &other);

    /** 
     * ��������
     */
    ~DateTime();

    /** 
     * �������
     * @return ����
     */
    Date date() const;

    /** 
     * ���ʱ��
     * @return ʱ��
     */
    Time time() const;

    /** 
     * ��õ�ǰ����ʱ��
     * @return ��ǰ����ʱ��
     */
    static DateTime currentDateTime();

    /** 
     * ��ֵ���������
     * @param other  ��������
     * @return ��ֵ��ǰ���������
     */
    DateTime& operator=(const DateTime &other);

    /** 
     * �������������
     * @param other  �Ƚ϶���
     * @return ��ȷ���true�����򷵻�false
     */
    dbool operator==(const DateTime &other) const;

    /** 
     * �������������
     * @param other  �Ƚ϶���
     * @return ����ȷ���true�����򷵻�false
     */
    dbool operator!=(const DateTime &other) const;
};
DM_END_NAMESPACE
#endif // DMDATETIME_H
