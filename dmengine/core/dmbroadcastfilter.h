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
#ifndef DMBROADCASTFILTER_H
#define DMBROADCASTFILTER_H

#include "dmnamespace.h"

DM_BEGIN_NAMESPACE  
class UtilString;
DM_PRIVATE_CLASS(BroadcastFilter);
class DM_DLL_EXPORT BroadcastFilter
{
    DM_DECLARE_PRIVATE(BroadcastFilter)
public:

    BroadcastFilter();
    ~BroadcastFilter();

    /** 
     * ����һ���㲥action����Ӧaction�Ľ������ܽ��յ���Ӧaction�Ĺ㲥
     * @param action  �㲥action
     */
    void addAction(const UtilString &action);

    /** 
     * �㲥action������
     * @return action����
     */
    dint actionSize() const;

    /** 
     * ���ĳ��index��action�ִ�
     * @param index  ����
     * @return action���ִ�
     */
    const UtilString& actionAt(dint index) const;

    /** 
     * ��д��ֵ�����
     * @param other  ��ֵ
     * @return ��ֵ���filter
     */
    BroadcastFilter &operator=(const BroadcastFilter &other);

    /** 
     * ����һ����action��ɵ�filter��������NULL��β�����Լ�delete���ص�filter
     * @param action  �㲥��action
     * @return ������filter
     */
    static BroadcastFilter* create(const char *action, ...);
};

DM_END_NAMESPACE
#endif //DMBROADCASTFILTER_H
