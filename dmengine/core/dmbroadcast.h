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
#ifndef DMBROADCAST_H
#define DMBROADCAST_H

#include "dmutilstring.h"
#include "dmvariant.h"

DM_BEGIN_NAMESPACE  
class BroadcastFilter;
class DM_DLL_EXPORT Broadcast
{
public:
    /** 
     * 在应用内广播消息
     * @param action  广播的action，对应action的接收器将接收到广播
     * @param msg  广播的数据
     * @return 成功返回true，否则返回false
     */
    static dbool sendBroadcast(const UtilString &action, const Variant &msg);

    /** 
     * 在应用内广播消息
     * @param filter  广播的filter，对应filter的接收器将接收到广播
     * @param msg  广播的数据
     * @return 成功返回true，否则返回false
     */
    static dbool sendBroadcast(const BroadcastFilter &filter, const Variant &msg);
};

DM_END_NAMESPACE
#endif //DMBROADCAST_H
