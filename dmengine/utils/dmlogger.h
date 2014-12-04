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

#ifndef DMLOGGER_H
#define DMLOGGER_H

#include "dmnamespace.h"
#include "dmlog.h"
#include "dmsingleton.h"

DM_BEGIN_NAMESPACE
DM_PRIVATE_CLASS(Logger);
class DM_DLL_EXPORT Logger : public Singleton<Logger>
{
    DM_DECLARE_PRIVATE(Logger)
public:
    /** 
     * 构造函数，设置打印机别，在该级别以下的调试信息将不会显示，默认为LOGDEBUG
     * @param level  调试级别
     */
    Logger(LogLevel level = LOGDEBUG);

    /** 
     * 析构函数
     */
    ~Logger();

    /** 
     * 添加调试信息
     * @param level  调试信息级别
     * @param format  格式
     */
    void log(LogLevel level, const char *format, ...);

    /** 
     * 设置调试级别
     * @param level  在该级别以下的调试信息将不会显示
     */
    void setLogLevel(LogLevel level);

    /** 
     * 获得当前调试级别
     * @return 调试级别
     */
    LogLevel getLogLevel() const;
};
DM_END_NAMESPACE
#endif // DMLOGGER_H
