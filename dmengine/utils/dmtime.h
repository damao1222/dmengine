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
#ifndef DMTIME_H
#define DMTIME_H
#include "dmnamespace.h"
DM_BEGIN_NAMESPACE
class DM_DLL_EXPORT Time
{
public:
    /** 
     * 获得当前秒
     * @return 秒数
     */
    static duint getSecond();

    /** 
     * 获得当前毫秒数
     * @return 毫秒数
     */
    static duint getMillisecond();

    /** 
     * 获得当前微秒数
     * @return 微秒数
     */
    static duint getMicrosecond();

    /** 
     * 获得应用运行的总时间（第一次调用此接口时开始计算，单位毫秒）
     * @return 毫秒数
     */
    static duint getAppClockMillis();
};
DM_END_NAMESPACE
#endif // DMTIME_H
