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
#ifndef DMRENDOM_H
#define DMRENDOM_H

#include "dmnamespace.h"

DM_BEGIN_NAMESPACE
class DM_DLL_EXPORT Random
{
public:
    /** 
     * �����������Ӧ��ֻ�����һ��
     */
    static void shuffle();

    /** 
     * ���һ�������
     * @return �����
     */
    static duint32 getRandom();

    /** 
     * ���һ��nFrom��nTo֮��������
     * @param nFrom  ��һ����Χ
     * @param nTo  �ڶ�����Χ
     * @return �����
     */
    static duint32 getRandom(duint32 nFrom, duint32 nTo);

    /** 
     * ���һ��0-1֮��������
     * @return �����
     */
    static dreal getRandom_0_1();
};
DM_END_NAMESPACE
#endif // DMRENDOM_H
