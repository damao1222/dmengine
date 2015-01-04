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

#include "dmnamespace.h"
DM_BEGIN_NAMESPACE
class BaseRefPtr;
DM_PRIVATE_CLASS(AutoReleaseManager);
class DM_DLL_EXPORT AutoReleaseManager
{
    DM_DECLARE_PRIVATE(AutoReleaseManager)
public:
    AutoReleaseManager();
    ~AutoReleaseManager();

    /** 
     * �Զ��ͷ�obj�������Զ��ͷŶ���ء�ע�⣬���ø÷�������retain�ö����̰߳�ȫ��.
     * @param obj  ��Ҫ�Զ��ͷŵĶ���
     */
    void autoRelease(BaseRefPtr *obj);

    /** 
     * ȡ���Զ��ͷţ��Ӷ������ȡ���ö���ע�⣬���ø÷�������release�ö�����Ҫ�Լ�release���̰߳�ȫ��.
     * @param obj  ȡ���Զ��ͷŵĶ���
     * @return �ɹ�����true��ʧ�ܷ���false
     */
    dbool cancelRelease(BaseRefPtr *obj);

    /** 
     * ���Զ��ͷŶ�����е����ж���ȫ��release����ն���أ��̰߳�ȫ��.
     */
    void releaseAll();

    /** 
     * ����Զ��ͷŶ�����е����ж���release�������ü���Ϊ1�Ķ�����Ϊû�����������ڳ��иö��󣬿����ͷţ��̰߳�ȫ��.
     */
    void cleanInvalid();

    /** 
     * ����Զ��ͷų��и�������ͷŵĶ���������̰߳�ȫ��.
     * @return ���ض������
     */
    duint32 managedCount() const;

    /** 
     * ���AutoReleaseManager�ĵ��������������������newһ��AutoReleaseManager�����̰߳�ȫ��.
     * @return ����AutoReleaseManager����.
     */
    static AutoReleaseManager* getInstance();

    /** 
     * ���AutoReleaseManager�ĵ���������������ڷ��ص���ָ�룬���򷵻�NULL
     * @return ����AutoReleaseManager����.
     */
    static AutoReleaseManager* instance();

    /** 
     * �ͷ�AutoReleaseManager�ĵ������̰߳�ȫ��.
     */
    static void releaseInstance();
};
DM_END_NAMESPACE

#define DM_AUTORELEASE(x) DM_NS::AutoReleaseManager::instance()->autoRelease(x)
#define DM_CANCELAUTORELEASE(x) DM_NS::AutoReleaseManager::instance()->cancelRelease(x)
#endif // DMAUTORELEASEMGR_H
