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
     * 增加一个广播action，对应action的接收器能接收到对应action的广播
     * @param action  广播action
     */
    void addAction(const UtilString &action);

    /** 
     * 广播action的数量
     * @return action数量
     */
    dint actionSize() const;

    /** 
     * 获得某个index的action字串
     * @param index  索引
     * @return action的字串
     */
    const UtilString& actionAt(dint index) const;

    /** 
     * 重写赋值运算符
     * @param other  右值
     * @return 赋值后的filter
     */
    BroadcastFilter &operator=(const BroadcastFilter &other);

    /** 
     * 创建一个由action组成的filter，必须由NULL结尾，并自己delete返回的filter
     * @param action  广播的action
     * @return 创建的filter
     */
    static BroadcastFilter* create(const char *action, ...);
};

DM_END_NAMESPACE
#endif //DMBROADCASTFILTER_H
