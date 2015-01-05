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
    ʹ�����ƣ�
    ���ڿ��ܻ������ڴ�й©���ظ������Լ����������ڴ��ַ�����⣬Ӧ�������������
    1����Ҫ����һ����ʱ��DM::Ref��Ϊ�����Ĳ��� ��
        foo(Dm::Ref(new CLASS));
    2����ҪͬʱΪͬһ��ָ�������ü���������ѭ������:
        CLASS* A = new CLASS; Dm::Ref ref(A); Dm::Ref another_ref(A);
    3�������ڷ�ͬһ����������ʹ��list��vector������װ��DM::Ref���� :
        {std::list< Dm::Ref<CLASS> > mList; mList.push_back(ref);} {mList[i].get()->do()}
       ���ȷʵ��Ҫʹ�ã���ʹ��swap�������ڵĶ�����װ�ض��󽻻���ʹ������ȡ���ͷ�Ȩ ��
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
