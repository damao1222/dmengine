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

#ifndef DMREF_H
#define DMREF_H

#include "dmatomicint.h"
#include "dmautoreleasemgr.h"
#include "dmlogger.h"
DM_BEGIN_NAMESPACE

class DM_DLL_EXPORT BaseRefPtr
{
    friend class AutoReleaseManager;
public:
    virtual void retain() = 0;
    virtual void release() = 0;
    virtual void autoRelease() { DM_ASSERT(false); }
    virtual void cancelAutoRelease() { DM_ASSERT(false); }
    virtual dbool isAutoRelease() const { DM_ASSERT(false); return false; }
    virtual dint refCount()const = 0;

protected:
    BaseRefPtr():_ref_count(1), _auto_release(0){}
    virtual ~BaseRefPtr() {}

protected:
    AtomicInt _ref_count;
    //_auto_release is used by AutoReleaseManager
    AtomicInt _auto_release;
};

inline void cleanRefPtr(BaseRefPtr *&p)
{
    p->release();
}

template<typename T>
class RefPtr : public BaseRefPtr
{
public:
    void retain()
    {
        DM_ASSERT(_ref_count > 0);
        _ref_count.ref();
    }

    void release()
    {
        DM_ASSERT(_ref_count > 0);
        /*
        _ref_count.deref();
        if (_ref_count == 0) {
        */
        //is 0
        if (!_ref_count.deref()) {
            delete static_cast<T*>(this);
        }
    }

    void autoRelease()
    {
        DM_AUTORELEASE(this);
    }

    void cancelAutoRelease()
    {
        DM_CANCELAUTORELEASE(this);
    }

    dbool isAutoRelease() const { return _auto_release != 0; }

    dint refCount() const
    {
        return _ref_count;
    }
};

template<typename T>
class SingleRefPtr
{
public:
    inline SingleRefPtr(BaseRefPtr *ptr): p(ptr) { DM_ASSERT(p); }
    inline ~SingleRefPtr() { DM_SAFE_RELEASE(p); }
    inline T* get() const { return static_cast<T*>(p); }

private:
    SingleRefPtr() {}
    BaseRefPtr *p;
};

DM_END_NAMESPACE
#endif // DMREF_H
