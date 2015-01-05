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
     * ����Ӧ����ѭ���ļ��ʱ�䣬Ӧ��ѭ��ʱ�䲻����¼��������ߣ�֪��������¼����
     * Ĭ��Ϊ20ms
     * @param delta  Ӧ��ѭ���ڵ�֡ʱ����
     * @return ��ǰʵ��
     */
    inline AppConfig& setAppFrameDelta(duint delta);

    /** 
     * �����Ƿ������㲥�̣߳��������Ϊtrue������Ӧ�ó�ʼ��ʱ�������㲥�̣߳������ڳ��������첽�ķ��͹㲥
     * Ĭ��Ϊ�ر�
     * @param enable  �����㲥�̵߳ı�־λ
     * @return ��ǰʵ��
     */
    inline AppConfig& setBroadcastMultiThread(dbool enable);

    /** 
     * ���ÿ��߳�����ִ������ʼ�����߳�����
     * Ĭ��Ϊ0��������
     * @param number  �߳�����
     * @return ��ǰʵ��
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
