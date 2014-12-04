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
     * 构造函数
     * @param totleTime 结束时间
     */
    EndTime(duint totleTime = 0);

    /** 
     * 析构函数
     */
    ~EndTime();

    /** 
     * 重置计时器
     */
    void resetTime();

    /** 
     * 使用一个新的时间重置计时器
     * @param totleTime  结束时间
     */
    void resetTime(duint totleTime);

    /** 
     * 使计时器过期
     */
    void setExpire();

    /** 
     * 设置计时器永远不会过期
     */
    void setInfinite();

    /** 
     * 将判断计时器是否已经过期
     * @return 过期返回true，否则返回false
     */
    dbool isExpired() const;

    /** 
     * 判断计时器是否永不过期
     * @return 永不过期返回true，否则返回false
     */
    dbool isInfinite() const;

    /** 
     * 获得计时器剩余时间，如果永不过期返回UINT_MAX，过期返回0
     * @return 剩余时间
     */
    duint leftTime() const;

    /** 
     * 计时器从开始到现在已经过去的时间
     * @return 过去时间
     */
    duint pastTime() const;
};
DM_END_NAMESPACE
#endif // DMENDTIME_H
