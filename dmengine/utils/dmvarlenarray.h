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

#ifndef DMVARLENARRAY_H
#define DMVARLENARRAY_H

#include "dmmalloc.h"

DM_BEGIN_NAMESPACE
template <typename T>
class VarLenArray
{
public:

    VarLenArray(dint nSize = 0);
    ~VarLenArray();

    void resize(dint nSize);

    void append(const VarLenArray &other);
    void append(const T &d);
    void append(const T *d, dint size);

    void prepend(const VarLenArray &other);
    void prepend(const T &d);
    void prepend(const T *d, dint size);

    T* data();
    const T* data() const;
    const T* constData() const;

    dint size() const;
    dbool isEmpty() const;

    void clear();

    const T& at(dint index) const;

    T& operator[](dint index);

    const T& operator[](dint index) const;

    dbool operator==(const VarLenArray &other) const;
    dbool operator!=(const VarLenArray &other) const;

private:
    T *p; dint s; dint c;
    void realloc(dint nSize);
};

template <typename T>
VarLenArray<T>::VarLenArray(dint nSize /*= 0*/):
    p(NULL), s(0), c(0)
{
    realloc(nSize);
}

template <typename T>
VarLenArray<T>::~VarLenArray()
{
    clear();
}

template <typename T>
void VarLenArray<T>::resize(dint nSize)
{
    realloc(nSize);
}

template <typename T>
void VarLenArray<T>::append(const VarLenArray &other)
{

}

template <typename T>
void VarLenArray<T>::append(const T &d)
{

}

template <typename T>
void VarLenArray<T>::append(const T *d, dint size)
{

}

template <typename T>
void VarLenArray<T>::prepend(const VarLenArray &other)
{

}

template <typename T>
void VarLenArray<T>::prepend(const T &d)
{

}

template <typename T>
void VarLenArray<T>::prepend(const T *d, dint size)
{

}

template <typename T>
inline T* VarLenArray<T>::data()
{
    return p;
}

template <typename T>
inline const T* VarLenArray<T>::data() const
{
    return p;
}

template <typename T>
const T* VarLenArray<T>::constData() const
{
    return p;
}

template <typename T>
dint VarLenArray<T>::size() const
{
    return s;
}

template <typename T>
dbool VarLenArray<T>::isEmpty() const
{
    return p == NULL || s == 0;
}

template <typename T>
void VarLenArray<T>::clear()
{
    if (p)
    {
        Free(p); p = NULL;
    }
}

template <typename T>
const T& VarLenArray<T>::at(dint index) const
{
    return p[index];
}

template <typename T>
T& VarLenArray<T>::operator[](dint index)
{
    return p[index];
}

template <typename T>
const T& VarLenArray<T>::operator[](dint index) const
{
    return p[index];
}

template <typename T>
dbool VarLenArray<T>::operator==(const VarLenArray &other) const
{
    return p == other.p && s == other.s;
}

template <typename T>
dbool VarLenArray<T>::operator!=(const VarLenArray &other) const
{
    return !((*this) == other);
}

template <typename T>
void VarLenArray<T>::realloc(dint nSize)
{
    if (nSize == 0) return;
    if (p == NULL)
    {
        size_t size = nSize * sizeof(T);
        p = Malloc(size);
        DM_CHECK_PTR(p);
        s = size;
    }
    else
    {
        size_t size = nSize * sizeof(T);
        p = Realloc(p, size);
        DM_CHECK_PTR(p);
        s = size;
    }
}
DMVARLENARRAY_H
#endif // DMBYTEARRAY_H
