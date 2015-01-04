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

#ifndef DMLOG_H
#define DMLOG_H
// ones we use in the code
DM_BEGIN_NAMESPACE
typedef enum{
    LOGDEBUG    = 0,
    LOGINFO     = 1,
    LOGNOTICE   = 2,
    LOGWARNING  = 3,
    LOGERROR    = 4,
    LOGSEVERE   = 5,
    LOGFATAL    = 6,
    LOGNONE     = 7
}LogLevel;

#ifdef DM_DEBUG
#ifdef DM_OS_WIN
    #define DM_LOG(LEVEL, FORMAT, ...) DM_NS::Logger::instance()->log(LEVEL, "<%s:%d>(%s) \n"FORMAT, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
    //info
    #define DM_LOGI(FORMAT, ...) DM_NS::Logger::instance()->log(DM_NS::LOGINFO, FORMAT, ##__VA_ARGS__)
    //warning
    #define DM_LOGW(FORMAT, ...) DM_NS::Logger::instance()->log(DM_NS::LOGWARNING, "<%s:%d>(%s) \n"FORMAT, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
    //fatal
    #define DM_FATAL(FORMAT, ...) DM_NS::Logger::instance()->log(DM_NS::LOGFATAL, "<%s:%d>(%s) \n"FORMAT, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#else
    #define DM_LOG(LEVEL, FORMAT, ARG...) DM_NS::Logger::instance()->log(LEVEL, "<%s:%d>(%s) \n"FORMAT, __FILE__, __LINE__, __FUNCTION__, ##ARG)
    //info
    #define DM_LOGI(FORMAT, ARG...) DM_NS::Logger::instance()->log(DM_NS::LOGINFO, FORMAT, ##ARG)
    //warning
    #define DM_LOGW(FORMAT, ARG...) DM_NS::Logger::instance()->log(DM_NS::LOGWARNING, "<%s:%d>(%s) \n"FORMAT, __FILE__, __LINE__, __FUNCTION__, ##ARG)
    //fatal
    #define DM_FATAL(FORMAT, ARG...) DM_NS::Logger::instance()->log(DM_NS::LOGFATAL, "<%s:%d>(%s) \n"FORMAT, __FILE__, __LINE__, __FUNCTION__, ##ARG)
#endif
#define DM_SET_LOGLEVEL(X) DM_NS::Logger::instance()->setLogLevel(X)
//assert
#define DM_ASSERT_X(cond, msg) ((!(cond)) ? DM_NS::Logger::instance()->log(DM_NS::LOGFATAL, "<%s:%d>(%s) \n%s", __FILE__, __LINE__, __FUNCTION__, msg) : dm_noop())
#else
    #define DM_LOG(LEVEL, FORMAT, ARG...)
    #define DM_LOGI(FORMAT, ...)
    #define DM_LOGW(FORMAT, ...)
    #define DM_FATAL(FORMAT, ...)
    #define DM_SET_LOGLEVEL(X)
    #define DM_ASSERT_X(cond, msg) do {DM_ASSERT(cond); DM_UNUSED(msg); }while(0)
#endif
DM_END_NAMESPACE

#endif // DMLOG_H
