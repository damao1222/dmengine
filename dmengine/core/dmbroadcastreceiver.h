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
     * ע��һ���㲥������
     * @return �ɹ�����true�����򷵻�false
     */
    dbool regist();

    /** 
     * ע��һ���㲥������
     * @return Return true if Success else return false
     */
    dbool unregist();

    /** 
     * ���չ㲥�Ļص��������յ�����recievefilter()���ص�filter�㲥ʱ���÷�������ִ��
     * ע�⣺Ӧ�ù㲥������Ϊ���߳�ʱ���÷������ڹ㲥�̱߳�����
     * @param msg  ���յ�����Ϣ
     */
    virtual void onReceive(const ReceiverMessage &msg) = 0;

    /** 
     * ���ظý������Ĺ㲥filter��ϵͳ����filter���ý������ַ��㲥
     * @return �㲥filter
     */
    virtual const BroadcastFilter& receivefilter() const = 0;
};

DM_END_NAMESPACE
//#include "dmobjectpro.h"
#endif //DMBROADCASTRECEIVER_H
