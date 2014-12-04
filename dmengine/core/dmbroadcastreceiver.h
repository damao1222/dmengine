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
#ifndef DMBROADCASTRECEIVER_H
#define DMBROADCASTRECEIVER_H

#include "dmbroadcastfilter.h"
#include "dmutilstring.h"
#include "dmvariant.h"

DM_BEGIN_NAMESPACE  
class ReceiverMessage
{
public:
    DM_PROPERTY_REF(UtilString, action, Action)
    DM_PROPERTY_REF(Variant, data, Data)
};

class DM_DLL_EXPORT BroadcastReceiver
{
public:
    /** 
     * 注册一个广播接收器
     * @return 成功返回true，否则返回false
     */
    dbool regist();

    /** 
     * 注销一个广播接收器
     * @return Return true if Success else return false
     */
    dbool unregist();

    /** 
     * 接收广播的回调，当接收到符合recievefilter()返回的filter广播时，该方法将被执行
     * 注意：应用广播被设置为多线程时，该方法将在广播线程被调用
     * @param msg  接收到的消息
     */
    virtual void onReceive(const ReceiverMessage &msg) = 0;

    /** 
     * 返回该接收器的广播filter，系统根据filter给该接收器分发广播
     * @return 广播filter
     */
    virtual const BroadcastFilter& receivefilter() const = 0;
};

DM_END_NAMESPACE
//#include "dmobjectpro.h"
#endif //DMBROADCASTRECEIVER_H
