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

#ifndef DMPROBABILITYUTIL_H
#define DMPROBABILITYUTIL_H
#include "dmnamespace.h"
DM_BEGIN_NAMESPACE
class DM_DLL_EXPORT ProbabilityUtil
{
public:
    /** 
     * ϴ�����ã�Ӧ��ֻ�����һ��
     */
    static void shuffle();

    /** 
     * ���ѡ���������е�һ��Ԫ��
     * @param array  ��������
     * @param size  �����С
     * @return ���������е�ĳ��Ԫ������
     */
    static dint choose(const dint *array, dint size);

    /** 
     * ���ѡ���������е�һ��Ԫ�أ�totalֵ����С�ڵ��ڼ�������ĺ�
     * @param array  ��������
     * @param size  �����С
     * @param total  �ܼ���
     * @return ���������е�ĳ��Ԫ������
     */
    static dint choose(const dint *array, dint size, dint total);
    
    /** 
     * ���ѡ���������е�һ��Ԫ�أ�ע�⼸��������ܺͲ��ܴ���1
     * @param array  ��������
     * @param size  �����С
     * @return ���������е�ĳ��Ԫ������
     */
    static dint choose(const dreal *array, dint size);
};
DM_END_NAMESPACE
#endif // DMPROBABILITYUTIL_H
