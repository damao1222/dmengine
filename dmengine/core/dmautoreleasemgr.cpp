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

#include "dmautoreleasemgr.h"
#include "dmrefptr.h"
#include "dmvector.h"

DM_BEGIN_NAMESPACE
typedef Vector<BaseRefPtr*> pool;
class AutoReleaseManagerPrivate
{
public:
    Vector<pool*> pools;
};

AutoReleaseManager::AutoReleaseManager():
    C_D(AutoReleaseManager)
{
    pool* p = new pool(100);
    pdm->pools.append(p);
}


AutoReleaseManager::~AutoReleaseManager()
{
    releaseAll();
    for (int i=0; i<pdm->pools.size(); ++i)
    {
        pool *p = pdm->pools.at(i);
        delete p;
    }
    D_D(AutoReleaseManager);
}

void AutoReleaseManager::autoRelease(BaseRefPtr *obj)
{
    if (obj->isAutoRelease())
    {
        return ;
    }

    //FIXME: only one pool
    pdm->pools.at(0)->append(obj);
    obj->_auto_release.ref();
}

dbool AutoReleaseManager::cancelRelease(BaseRefPtr *obj)
{
    if (!obj->isAutoRelease())
    {
        return false;
    }

    dint index = pdm->pools.at(0)->indexOf(obj);
    if (index < 0)
        return false;

    //release here? you need release obj yourself
    obj->_auto_release.deref();
    pdm->pools.at(0)->removeAt(index);
    return true;
}

void AutoReleaseManager::releaseAll()
{
    for (int i=0; i<pdm->pools.size(); ++i)
    {
        pool* p = pdm->pools.at(i);
        for (int j=0; j<p->size(); ++j)
        {
            BaseRefPtr *obj = p->at(j);
            obj->_auto_release.deref();
            obj->release();
        }
        p->clear();
    }
}

void AutoReleaseManager::cleanInvalid()
{
    for (int i=0; i<pdm->pools.size(); ++i)
    {
        pool* p = pdm->pools.at(i);
        pool::Iterator it = p->begin();
        while (it != p->constEnd())
        {
            BaseRefPtr* obj = *it;
            if (obj->refCount() == 1)
            {
                it = p->erase(it);
                //delete obj at here
                obj->_auto_release.deref();
                obj->release();
                continue;
            }
            ++it;
        }
    }
}

duint32 AutoReleaseManager::managedCount() const
{
    duint32 count = 0;
    for (int i=0; i<pdm->pools.size(); ++i)
    {
        pool* p = pdm->pools.at(i);
        count += p->size();
    }
    return count;
}
DM_END_NAMESPACE

