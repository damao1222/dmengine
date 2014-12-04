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
     * 自动释放obj，加入自动释放对象池。注意，调用该方法不会retain该对象
     * @param obj  需要自动释放的对象
     */
    void autoRelease(BaseRefPtr *obj);

    /** 
     * 取消自动释放，从对象池中取出该对象。注意，调用该方法不会release该对象，需要自己release
     * @param obj  取消自动释放的对象
     * @return 成功返回true，失败返回false
     */
    dbool cancelRelease(BaseRefPtr *obj);

    /** 
     * 将自动释放对象池中的所有对象全部release并清空对象池
     */
    void releaseAll();

    /** 
     * 检查自动释放对象池中的所有对象，release所有引用计数为1的对象（认为没有其他代码在持有该对象，可以释放）
     */
    void cleanInvalid();

    /** 
     * 获得自动释放池中负责管理释放的对象个数
     * @return 返回对象个数
     */
    duint32 managedCount() const;
};

#define DM_AUTORELEASE(x) AutoReleaseManager::getInstance()->autoRelease(x)
#define DM_CANCELAUTORELEASE(x) AutoReleaseManager::getInstance()->cancelRelease(x)
DM_END_NAMESPACE
#endif // DMAUTORELEASEMGR_H
