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
#ifndef DMVARIANT_P_H
#define DMVARIANT_P_H
#include "dmnamespace.h"
#include "dmatomicint.h"

DM_BEGIN_NAMESPACE
class VariantPrivate
{
public:
    struct Data{
        BasicAtomicInt ref;
        union _Data{
            dchar c;
            dint i;
            duint u;
            dbool b;
            double d;
            float f;
            dlonglong ll;
            dulonglong ull;
            void *ptr;
        }data;
        duint type : 30;
        duint sharable : 1;
        duint isComplex : 1;
    };

    Data* create();
    //void construct(Data *data);
    void construct(Data *&d, Data *data);
    void detach();
    void detach_helper();
    void detach_helper(Data* data);
    Data* __detach(Data* data);
    Data* __detach();
    void free(Data *d);
    static Data shared_null;
    Data *d;
};

template <typename T>
inline const T *v_cast(const VariantPrivate::Data *d, T * = 0)
{
    return static_cast<T*>(d->data.ptr);
}

template <typename T>
inline T *v_cast(VariantPrivate::Data *d, T * = 0)
{
    return static_cast<T*>(d->data.ptr);
}

template <class T>
inline void v_construct(VariantPrivate::Data *x, const void *copy, T * = 0)
{
    if (x->isComplex || sizeof(T) > sizeof(VariantPrivate::Data::_Data)) {
        x->data.ptr = copy ? new T(*static_cast<const T *>(copy)) : new T;
    } else {
        if (copy)
            new (&x->data.ptr) T(*static_cast<const T *>(copy));
        else
            new (&x->data.ptr) T;
    }
}

template <class T>
inline void v_construct(VariantPrivate::Data *x, const T &t)
{
    if (x->isComplex || sizeof(T) > sizeof(VariantPrivate::Data::_Data)) {
        x->data.ptr = new T(t);
    } else {
        new (&x->data.ptr) T(t);
    }
}

template <class T>
inline void v_clear(VariantPrivate::Data *d, T* = 0)
{
    delete static_cast<T*>(d->data.ptr);
#if 0
    if (sizeof(T) > sizeof(VariantPrivate::Data::_Data)) {
        //now we need to cast
        //because QVariant::PrivateShared doesn't have a virtual destructor
        delete static_cast<T*>(d->data.ptr);
    } else {
        v_cast<T>(d)->~T();
    }
#endif
}
DM_END_NAMESPACE
#endif // DMVARIANT_P_H
