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
#if 1
#include "dmlogger.h"
#include "dmnamespace.h"
//#include "dmsinglelock.h"
#include "dmtime.h"
//#include "dmthread.h"
#include "dmlog.h"
#ifdef DM_USE_STDSTRING
#include <StdString.h>
#endif

#include <string.h>
#include <stdarg.h>

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#if !defined(DM_OS_WINCE)
#  include <errno.h>
#  if defined(DM_CC_MSVC)
#    include <crtdbg.h>
#  endif
#endif
#ifdef DM_OS_WIN
#ifndef DM_OS_WP8
#include <winsock2.h>
#endif
#include <windows.h>
#endif

#if defined(DM_OS_VXWORKS)
#  include <envLib.h>
#endif

#if defined(DM_OS_MACX) && !defined(DM_OS_IOS)
#include <CoreServices/CoreServices.h>
#endif

#include "thread/dmsinglelock.h"
#include "dmutilstring.h"
DM_BEGIN_NAMESPACE
//DM_GLOBEL_INSTANCE_QUICK(CriticalSection, Lock)
//#define Mutex Lock_instance()
static char levelNames[][8] =
{"DEBUG", "INFO", "NOTICE", "WARNING", "ERROR", "SEVERE", "FATAL", "NONE"};
enum{
    MSG_BUFFER_SIZE = 2*DM_MEMORYNUM+1024
};

#ifdef DM_OS_ANDROID
#include <android/log.h>
int android_printf(int level, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int result = __android_log_vprint(level, "DMDebug", format, args);
    va_end(args);
    //return result;
}
#endif

class LoggerPrivate
{
public:
    //void OutputDebugString(const std::string& line);
    void outputDebugString(const UtilString &line);
    LogLevel logLevel;
    dint repeatCount;
    Mutex mutex;
};

Logger::Logger(LogLevel level/* = LOGDEBUG*/):
    C_D(Logger)
{
    pdm->logLevel = level;
}

Logger::~Logger()
{
    D_D(Logger);
}

void Logger::setLogLevel(LogLevel level)
{
    SingleLock lock(&pdm->mutex);
    pdm->logLevel = level;
}

LogLevel Logger::getLogLevel() const
{
    return pdm->logLevel;
}

void Logger::log(LogLevel loglevel, const char *format, ... )
{
#ifndef DM_DEBUG
    DM_UNUSED(loglevel);
    DM_UNUSED(format);
#else
//    static const char* prefixFormat = "[%02.2d:%02.2d:%02.2d] [T:%"DM_FmIu64"] [%7s]: ";
    //static const char* prefixFormat = "[DM %7s]: ";
    static const char* prefixFormat = "[DM %s]: ";
    SingleLock lock(&pdm->mutex);
#ifndef DM_USE_STDSTRING
//    DM_SAFE_BUF char prefix[DM_MEMORYNUM];
    DM_SAFE_BUF char Msg[MSG_BUFFER_SIZE];
#endif
//    SingleLock lock(*Mutex);

    if (pdm->logLevel <= loglevel)
    {
#if 0
    if (!m_file)
      return;
#endif

//        TimeUtil time = TimeUtil::currentDateTime();
#ifdef DM_USE_STDSTRING
        CStdString strPrefix, strData;

        strData.reserve(16384);
        va_list va;
        va_start(va, format);
        strData.FormatV(format, va);
        va_end(va);

        strPrefix.Format(prefixFormat, time.hour(), time.minute(), time.second(), 111LL/*(uint64_t)Thread::currentThreadId()*/, levelNames[loglevel]);
        pdm->OutputDebugString(strPrefix);
        pdm->OutputDebugString(strData);
#else
        memset(Msg, 0 ,sizeof(Msg));
//        sprintf(Msg, prefixFormat, time.hour(), time.minute(), time.second(), 111LL/*(uint64_t)Thread::currentThreadId()*/, levelNames[loglevel]);
#ifdef DM_CC_MSVC
        sprintf_s(Msg, prefixFormat, levelNames[loglevel]);
#else
        snprintf(Msg, 13, prefixFormat, levelNames[loglevel]);
#endif
        va_list ArgPtr;
        va_start(ArgPtr,format);
        int len = strlen(Msg);
#ifdef DM_CC_MSVC
        vsprintf_s(Msg+len, MSG_BUFFER_SIZE-len, format,ArgPtr);
#else
        vsnprintf(Msg+len, MSG_BUFFER_SIZE-len, format,ArgPtr);
#endif
//        vsprintf(Msg,format,ArgPtr);
        va_end(ArgPtr);
//        pdm->OutputDebugString(prefix);
        pdm->outputDebugString(Msg);

#if defined(DM_OS_ANDROID) && defined(DM_DEBUG)
        int andLv = ANDROID_LOG_VERBOSE;
        if (loglevel == LOGWARNING)
            andLv = ANDROID_LOG_WARN;
        else if (loglevel > LOGSEVERE)
            andLv = ANDROID_LOG_ERROR;
        android_printf(andLv, "%s", Msg);
#endif

#endif
        if (loglevel > LOGSEVERE)
        {
            exit(-1);
        }
    }
#endif //DM_DEBUG
}

#if 0
void LoggerPrivate::OutputDebugString(const std::string& line)
{
#if defined(DM_DEBUG)
#ifdef DM_USE_STDSTRING
#if defined(DM_OS_WIN)
  // we can't use charsetconverter here as it's initialized later than CLog and deinitialized early
  int bufSize = MultiByteToWideChar(CP_UTF8, 0, line.c_str(), -1, NULL, 0);
  CStdStringW wstr (L"", bufSize);
  if ( MultiByteToWideChar(CP_UTF8, 0, line.c_str(), -1, wstr.GetBuf(bufSize), bufSize) == bufSize )
  {
    wstr.RelBuf();
    ::OutputDebugStringW(wstr.c_str());
  }
  else
  {
    ::OutputDebugString(line.c_str());
  }
#else
    fprintf(stderr, "%s\n", line.c_str());
    fflush(stderr);
#endif // TARGET_WINDOWS
//  ::OutputDebugString(line.c_str());
//  ::OutputDebugString("\n");
#endif
#endif
}
#endif

void LoggerPrivate::outputDebugString(const UtilString &line)
{
#if defined(DM_DEBUG)
#if defined(DM_OS_WIN)
//    ::OutputDebugString(line);
    /*
    DM_SAFE_BUF WCHAR wszBuf[MSG_BUFFER_SIZE/2];
    MultiByteToWideChar(CP_UTF8, 0, line.toCharStr(), -1, wszBuf, sizeof(wszBuf));
    OutputDebugStringW(wszBuf);
    OutputDebugStringA("\n");
    */
    int bufSize = MultiByteToWideChar(CP_UTF8, 0, line.toCharStr(), -1, NULL, 0);
    CStdStringW wstr (L"", bufSize);
    if ( MultiByteToWideChar(CP_UTF8, 0, line.toCharStr(), -1, wstr.GetBuf(bufSize), bufSize) == bufSize )
    {
        wstr.RelBuf();
        ::OutputDebugStringW(wstr.c_str());
    }
    OutputDebugStringA("\n");
#else
    fprintf(stderr, "%s", line.toCharStr());
    fflush(stderr);
#endif // TARGET_WINDOWS
#endif
}
DM_END_NAMESPACE
#endif
