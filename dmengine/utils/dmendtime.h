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
#ifndef DMENDTIME_H
#define DMENDTIME_H
#include "dmnamespace.h"
DM_BEGIN_NAMESPACE
DM_PRIVATE_CLASS(EndTime);
class DM_DLL_EXPORT EndTime
{
    DM_DECLARE_PRIVATE(EndTime)
public:
    /** 
     * ���캯��
     * @param totleTime ����ʱ��
     */
    EndTime(duint totleTime = 0);

    /** 
     * ��������
     */
    ~EndTime();

    /** 
     * ���ü�ʱ��
     */
    void resetTime();

    /** 
     * ʹ��һ���µ�ʱ�����ü�ʱ��
     * @param totleTime  ����ʱ��
     */
    void resetTime(duint totleTime);

    /** 
     * ʹ��ʱ������
     */
    void setExpire();

    /** 
     * ���ü�ʱ����Զ�������
     */
    void setInfinite();

    /** 
     * ���жϼ�ʱ���Ƿ��Ѿ�����
     * @return ���ڷ���true�����򷵻�false
     */
    dbool isExpired() const;

    /** 
     * �жϼ�ʱ���Ƿ���������
     * @return �������ڷ���true�����򷵻�false
     */
    dbool isInfinite() const;

    /** 
     * ��ü�ʱ��ʣ��ʱ�䣬����������ڷ���UINT_MAX�����ڷ���0
     * @return ʣ��ʱ��
     */
    duint leftTime() const;

    /** 
     * ��ʱ���ӿ�ʼ�������Ѿ���ȥ��ʱ��
     * @return ��ȥʱ��
     */
    duint pastTime() const;
};
DM_END_NAMESPACE
#endif // DMENDTIME_H
