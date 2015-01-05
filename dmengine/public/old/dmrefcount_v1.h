/*
   Copyright (C) 2012-2013 Xiongfa Li
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

   Reference Count: (shared_ptr)
    使用限制：
    由于可能会引起内存泄漏、重复析构以及错误引用内存地址等问题，应避免如下情况：
    1、不要构造一个临时的DM::Ref作为函数的参数 ：
        foo(Dm::Ref(new CLASS));
    2、不要同时为同一个指针做引用计数，避免循环引用:
        CLASS* A = new CLASS; Dm::Ref ref(A); Dm::Ref another_ref(A);
    3、避免在非同一个作用域内使用list，vector等容器装载DM::Ref对象 :
        {std::list< Dm::Ref<CLASS> > mList; mList.push_back(ref);} {mList[i].get()->do()}
       如果确实需要使用，请使用swap将容器内的对象与装载对象交换，使容器获取主释放权 ：
        {list.push_back(ref);list.back().swap(ref);} list.clean();//release here


    modify:
        2012/01/19
*/

#ifndef DMREFCOUNT_H
#define DMREFCOUNT_H
#include "dmatomicint.h"
DM_BEGIN_NAMESPACE
template<class T>
class Ref
{
    typedef void (*cleanup)(T *&p);
public:
    Ref():
        mPtr(0),
        mCount(0),
        mFunc(0)
    {

    }

    Ref(T* p, cleanup func = 0):
        mPtr(p),
        mCount(1),
        mFunc(func)
    {

    }

    Ref(const Ref &other):
        mPtr(other.mPtr),
        mCount(other.mCount),
        mFunc(other.mFunc)
    {
        retain();
    }

    ~Ref()
    {
        release();
    }

    void setDeleter(cleanup func)
    {
        if (mFunc != func)
        {
            mFunc = func;
        }
    }

    int refCount() const
    {
        return mCount;
    }

    T* get() const
    {
        return mPtr;
    }

    void retain()
    {
        if (mCount < 1)
            return;

        mCount.ref();
    }

    void release()
    {
        if (mCount < 1)
            return;

        if (!mCount.deref())
        {
            deallocate();
        }
    }

    bool isNull() const
    {
        return mPtr == 0;
    }

    void swap(Ref &other)
    {
        T* tP = mPtr;
        int tC = mCount;
        cleanup tF = mFunc;

        mPtr = other.mPtr;
        mCount = other.mCount;
        mFunc = other.mFunc;

        other.mPtr = tP;
        other.mCount = tC;
        other.mFunc = tF;
    }

    bool operator==(const Ref &other) const
    {
        if (mPtr==other.mPtr)
            return true;
        else
            return false;
    }

    bool operator==(T* p) const
    {
        if (mPtr == p)
            return true;
        else
            return false;
    }

    bool operator!=(const Ref &other) const
    {
        if (mPtr == other.mPtr)
            return false;
        else
            return true;
    }

    bool operator!=(T* p) const
    {
        if (mPtr == p)
            return false;
        else
            return true;
    }

    T* operator->() const
    {
        return mPtr;
    }

    T& operator*() const
    {
        return *mPtr;
    }

    Ref& operator=(T* p)
    {
        Ref(p).swap(*this);
        return *this;
    }

    Ref& operator=(const Ref &other)
    {
        Ref r(other);
        r.swap(*this);
        return *this;
    }

#if 0 //old impliment
    Ref& operator=(T* p)
    {
        mPtr = p;

        return *this;
    }

    Ref& operator=(const Ref &other)
    {
        mPtr = other.mPtr;
        mCount = other.mCount;
        mFunc = other.mFunc;
        return *this;
    }
#endif
private:
    void deallocate()
    {
        if (mPtr)
        {
            if (mFunc == 0)
            {
                //DMDEBUG("%p is deallocated\n", mPtr);
                delete mPtr;
                mPtr = NULL;
            }
            else
            {
                //be careful, you need release memory in cleanup function, I don't check it.
                (*mFunc)(mPtr);
                mPtr = NULL;
            }
        }
    }
private:
    T* mPtr;
    AtomicInt mCount;
    cleanup mFunc;
};


DM_END_NAMESPACE

#endif // DMREFCOUNT_H
