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
#ifndef _DMSHAREDDATA_H_
#define _DMSHAREDDATA_H_

#include "dmnamespace.h"

DM_BEGIN_NAMESPACE
//Copy on write
template<class T>
class SharedData
{
public:
    inline SharedData();
    inline SharedData(T *ptr);
    inline SharedData(const SharedData &o);
    template<class X>
    inline SharedData(const SharedData<X> &o) : d(static_cast<T *>(o.get())) { if(d) d->ref.ref(); }
    inline ~SharedData();

    inline T *data();
    inline const T* constData() const;

    inline void clear();

    inline T &operator*();
    inline const T &operator*() const;

    inline T* operator->();
    inline const T* operator->() const;

    inline SharedData& operator=(T *ptr);
    inline SharedData& operator=(const SharedData &o);

    inline dbool operator==(T *ptr) const;
    inline dbool operator==(const SharedData &o) const;

    inline dbool operator!=(T *ptr) const;
    inline dbool operator!=(const SharedData &o) const;

    inline T *clone();

    inline void detach() ;
private:
     inline void free(T *d);
    void detach_helper();
    T *d;
};

template<class T>
inline SharedData<T>::SharedData(): d(NULL)
{ }

template<class T>
inline SharedData<T>::SharedData(T *ptr):
    d(ptr)
{
    if (d) d->ref.ref();
}

template<class T>
inline SharedData<T>::SharedData(const SharedData<T> &o): d(o.d)
{ if (d) d->ref.ref(); }

template<class T>
SharedData<T>::~SharedData()
{
    if (d && !d->ref.deref())
        free(d);
}

template<class T>
inline T *SharedData<T>::data()
{
    return d;
}

template<class T>
inline const T* SharedData<T>::constData() const
{
    return d;
}

template<class T>
void SharedData<T>::clear()
{
    if (d && !d->ref.deref())
        free(d);
    d = NULL;
}

template<class T>
inline T* SharedData<T>::operator->()
{
    return d;
}

template<class T>
inline const T* SharedData<T>::operator->() const
{
    return d;
}

template<class T>
inline T &SharedData<T>::operator*()
{
    return *d;
}

template<class T>
inline const T &SharedData<T>::operator*() const
{
    return *d;
}

template<class T>
inline SharedData<T>& SharedData<T>::operator=(T *ptr)
{
    if (d != ptr) {
        if (ptr)
            ptr->ref.ref();
        T *old = d;
        d = ptr;
        if (old && !old->ref.deref())
            free(old);
    }
    return *this;
}

template<class T>
inline SharedData<T>& SharedData<T>::operator=(const SharedData<T> &other)
{
    if (this == &other)
        return *this;

    if (other.d != d) {
        if (other.d)
            other.d->ref.ref();
        T *old = d;
        d = other.d;
        if (old && !old->ref.deref())
            free(old);
    }
    return *this;
}

template<class T>
inline dbool SharedData<T>::operator==(T *ptr) const
{
    return d == ptr;
}

template<class T>
inline dbool SharedData<T>::operator==(const SharedData<T> &other) const
{
    return d == other.d;
}

template<class T>
inline dbool SharedData<T>::operator!=(T *ptr) const
{
    return !operator==(ptr);
}

template<class T>
inline dbool SharedData<T>::operator!=(const SharedData<T> &o) const
{
    return !operator==(o);
}

template<class T>
void SharedData<T>::free(T *data)
{
    DM_SAFE_DELETE(data);
}

template<class T>
inline T *SharedData<T>::clone()
{
    return new T(*d);
}

template<class T>
inline void SharedData<T>::detach() 
{
    if (d && d->ref != 1) detach_helper();
}

template<class T>
void SharedData<T>::detach_helper()
{
    T *x = clone();
    //x->ref.ref();
    x->ref = 1;
    if (!d->ref.deref())
        free(d);
    d = x;
}

DM_END_NAMESPACE
#endif //_DMSHAREDDATA_H_

