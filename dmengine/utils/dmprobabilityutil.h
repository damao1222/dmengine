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
     * 洗牌重置，应用只需调用一次
     */
    static void shuffle();

    /** 
     * 随机选择几率数组中的一个元素
     * @param array  几率数组
     * @param size  数组大小
     * @return 几率数组中的某个元素索引
     */
    static dint choose(const dint *array, dint size);

    /** 
     * 随机选择几率数组中的一个元素，total值必须小于等于几率数组的和
     * @param array  几率数组
     * @param size  数组大小
     * @param total  总几率
     * @return 几率数组中的某个元素索引
     */
    static dint choose(const dint *array, dint size, dint total);
    
    /** 
     * 随机选择几率数组中的一个元素，注意几率数组的总和不能大于1
     * @param array  几率数组
     * @param size  数组大小
     * @return 几率数组中的某个元素索引
     */
    static dint choose(const dreal *array, dint size);
};
DM_END_NAMESPACE
#endif // DMPROBABILITYUTIL_H
