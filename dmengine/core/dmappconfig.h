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

#ifndef DMAPPCONFIG_H
#define DMAPPCONFIG_H

#include "dmnamespace.h"
DM_BEGIN_NAMESPACE
class Application;
class AppConfig
{
public:
    inline AppConfig();

    /** 
     * 设置应用内循环的间隔时间，应用循环时间不足此事件，将休眠，知道满足此事件间隔
     * 默认为20ms
     * @param delta  应用循环内的帧时间间隔
     * @return 当前实例
     */
    inline AppConfig& setAppFrameDelta(duint delta);

    /** 
     * 设置是否启动广播线程，如果设置为true，则在应用初始化时，启动广播线程，可以在程序中有异步的发送广播
     * 默认为关闭
     * @param enable  启动广播线程的标志位
     * @return 当前实例
     */
    inline AppConfig& setBroadcastMultiThread(dbool enable);

    /** 
     * 设置跨线程任务执行器初始化的线程数量
     * 默认为0，不开启
     * @param number  线程数量
     * @return 当前实例
     */
    inline AppConfig& setExecutorThreadNumber(duint number);

private:
    friend class Application;
    duint frameDelta;
    dbool enableBroadcastMultiThread;
    duint executorThreadNumber;
};







#ifndef DM_APP_DELTA
#define DM_APP_DELTA 20
#endif

inline AppConfig::AppConfig() 
    : frameDelta(DM_APP_DELTA)
    , enableBroadcastMultiThread(false)
    , executorThreadNumber(0)
{
}

inline AppConfig& AppConfig::setAppFrameDelta(duint delta)
{
    frameDelta = delta;
    return *this;
}
    
inline AppConfig& AppConfig::setBroadcastMultiThread(dbool enable)
{
    enableBroadcastMultiThread = enable;
    return *this;
}

inline AppConfig& AppConfig::setExecutorThreadNumber(duint number)
{
    executorThreadNumber = number;
    return *this;
}

DM_END_NAMESPACE
#endif // DMAPPCONFIG_H
