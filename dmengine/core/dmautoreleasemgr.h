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
#ifndef DMAUTORELEASEMGR_H
#define DMAUTORELEASEMGR_H

#include "dmsingleton.h"
DM_BEGIN_NAMESPACE
class BaseRefPtr;
DM_PRIVATE_CLASS(AutoReleaseManager);
class DM_DLL_EXPORT AutoReleaseManager: public Singleton<AutoReleaseManager>
{
    DM_DECLARE_PRIVATE(AutoReleaseManager)
public:
    AutoReleaseManager();
    ~AutoReleaseManager();

    /** 
     * �Զ��ͷ�obj�������Զ��ͷŶ���ء�ע�⣬���ø÷�������retain�ö���
     * @param obj  ��Ҫ�Զ��ͷŵĶ���
     */
    void autoRelease(BaseRefPtr *obj);

    /** 
     * ȡ���Զ��ͷţ��Ӷ������ȡ���ö���ע�⣬���ø÷�������release�ö�����Ҫ�Լ�release
     * @param obj  ȡ���Զ��ͷŵĶ���
     * @return �ɹ�����true��ʧ�ܷ���false
     */
    dbool cancelRelease(BaseRefPtr *obj);

    /** 
     * ���Զ��ͷŶ�����е����ж���ȫ��release����ն����
     */
    void releaseAll();

    /** 
     * ����Զ��ͷŶ�����е����ж���release�������ü���Ϊ1�Ķ�����Ϊû�����������ڳ��иö��󣬿����ͷţ�
     */
    void cleanInvalid();

    /** 
     * ����Զ��ͷų��и�������ͷŵĶ������
     * @return ���ض������
     */
    duint32 managedCount() const;
};

#define DM_AUTORELEASE(x) AutoReleaseManager::getInstance()->autoRelease(x)
#define DM_CANCELAUTORELEASE(x) AutoReleaseManager::getInstance()->cancelRelease(x)
DM_END_NAMESPACE
#endif // DMAUTORELEASEMGR_H
