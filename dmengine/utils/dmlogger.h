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
     * ���캯�������ô�ӡ�����ڸü������µĵ�����Ϣ��������ʾ��Ĭ��ΪLOGDEBUG
     * @param level  ���Լ���
     */
    Logger(LogLevel level = LOGDEBUG);

    /** 
     * ��������
     */
    ~Logger();

    /** 
     * ��ӵ�����Ϣ
     * @param level  ������Ϣ����
     * @param format  ��ʽ
     */
    void log(LogLevel level, const char *format, ...);

    /** 
     * ���õ��Լ���
     * @param level  �ڸü������µĵ�����Ϣ��������ʾ
     */
    void setLogLevel(LogLevel level);

    /** 
     * ��õ�ǰ���Լ���
     * @return ���Լ���
     */
    LogLevel getLogLevel() const;
};
DM_END_NAMESPACE
#endif // DMLOGGER_H
