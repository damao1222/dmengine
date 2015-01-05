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
#ifndef _DMCOW_H_
#define _DMCOW_H_

#include "dmatomicint.h"
#include "dmlogger.h"
#include <new>

DM_BEGIN_NAMESPACE
struct DM_DLL_EXPORT CowData {
    struct Data {
        BasicAtomicInt ref;
        duint32 sharable : 1;
        void* ptr;
    };

    static Data shared_null;
    Data *d;
};

//Copy on write
template<class T>
class COW
{
public:
    inline COW();
    inline explicit COW(const T &o);
    inline COW(const COW &o);
    virtual ~COW();

    T *get();
    const T& getRef() const;

    inline COW& operator=(const T& o);
    inline COW& operator=(const COW &o);

    inline dbool operator==(const T& o) const;
    inline dbool operator==(const COW &o) const;

    inline dbool operator!=(const T& o) const;
    inline dbool operator!=(const COW &o) const;

    dbool equals(const T& o) const;
    dbool equals(const COW &o) const;
    inline bool isDetached() const;
    inline void setSharable(dbool sharable);
    inline bool isSharedWith(const COW &other) const;
    inline void detach();

protected:
    void detach_helper();
    void detach_helper(const T &o);
    CowData::Data *detachExc();
    CowData::Data *detachExc(const T &o);
    void free(CowData::Data *d);
    union { CowData p; CowData::Data *d; };
private:
    CowData::Data *create(const T &o);
};

template<class T>
inline COW<T>::COW(): d(&CowData::shared_null)
{ d->ref.ref(); }

template<class T>
inline COW<T>::COW(const T &o):
    d(create(o))
{
}

template<class T>
inline COW<T>::COW(const COW<T> &o): d(o.d)
{ d->ref.ref(); if (!d->sharable) detach_helper(); }

template<class T>
COW<T>::~COW()
{
    if (!d->ref.deref())
        free(d);
}

template<class T>
inline T *COW<T>::get()
{
    detach();
    return static_cast<T*>(d->ptr);
}

template<class T>
inline const T& COW<T>::getRef() const
{
    return *static_cast<T*>(d->ptr);
}

template<class T>
inline COW<T>& COW<T>::operator=(const T& o)
{
    detach_helper(o);
    return *this;
}

template<class T>
inline COW<T>& COW<T>::operator=(const COW<T> &other)
{
    if (this == &other)
        return *this;

    if (d != other.d) {
        CowData::Data *o = other.d;
        o->ref.ref();
        if (!d->ref.deref())
            free(d);
        d = o;
        if (!d->sharable)
            detach_helper();
    }
    return (*this);
}

template<class T>
inline dbool COW<T>::operator==(const T& o) const
{
    return equals(o);
}

template<class T>
inline dbool COW<T>::operator==(const COW<T> &other) const
{
    return equals(other);
}

template<class T>
inline dbool COW<T>::equals(const T& o) const
{
    if (d->ptr == NULL) 
        return false;
    else
        return getRef() == o;
}

template<class T>
inline dbool COW<T>::equals(const COW &other) const
{
    if (d == other.d || d->ptr == other.d->ptr)
        return true;

    if (getRef() == other.getRef()) {
        return true;
    }
    else
        return false;
}

template<class T>
inline dbool COW<T>::operator!=(const T& o) const
{
    return !operator==(o);
}

template<class T>
inline dbool COW<T>::operator!=(const COW<T> &o) const
{
    return !operator==(o);
}

template<class T>
inline bool COW<T>::isDetached() const { return d->ref == 1; }

template<class T>
inline void COW<T>::setSharable(dbool sharable) { if (!sharable) detach(); d->sharable = sharable; }

template<class T>
inline bool COW<T>::isSharedWith(const COW<T> &other) const { return d == other.d; }

template<class T>
inline void COW<T>::detach()
{
    if (d->ref != 1) detach_helper();
}

template<class T>
void COW<T>::detach_helper()
{
    CowData::Data *x = detachExc();
    if (!x->ref.deref())
        free(x);
}

template<class T>
void COW<T>::detach_helper(const T &o)
{
    CowData::Data *x = detachExc(o);
    if (!x->ref.deref())
        free(x);
}

template<class T>
CowData::Data *COW<T>::detachExc()
{
    CowData::Data *x = d;
    CowData::Data* t = NULL;
    DM_TRY
    {
        t = new CowData::Data;
        t->ptr = new T(getRef());

        t->ref = 1;
        t->sharable = true;

        d = t;
    }
    DM_CATCH (const std::bad_alloc& ba)
    {
        DM_FATAL("%s", ba.what());
        delete t->ptr;
        delete t;
    }
    return x;
}

template<class T>
CowData::Data *COW<T>::detachExc(const T &o)
{
    CowData::Data *x = d;
    CowData::Data* t = NULL;
    DM_TRY
    {
        t = new CowData::Data;
        t->ptr = new T(o);

        t->ref = 1;
        t->sharable = true;

        d = t;
    }
    DM_CATCH (const std::bad_alloc& ba)
    {
        DM_FATAL("%s", ba.what());
        delete t->ptr;
        delete t;
    }
    return x;
}

template<class T>
CowData::Data *COW<T>::create(const T &o)
{
    CowData::Data* t = NULL;
    DM_TRY
    {
        t = new CowData::Data;
        t->ptr = new T(o);
        t->ref = 1;
        t->sharable = true;
    }
    DM_CATCH (const std::bad_alloc& ba)
    {
        DM_FATAL("%s", ba.what());
        delete t->ptr;
        delete t;
    }
    return t;
}

template<class T>
void COW<T>::free(CowData::Data *data)
{
    if (data->ptr != NULL)
        delete static_cast<T*>(data->ptr);
    DM_SAFE_DELETE(data);
}

DM_END_NAMESPACE
#endif //_DMCOW_H_

