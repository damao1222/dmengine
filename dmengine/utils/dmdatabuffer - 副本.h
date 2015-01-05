/*
   Copyright (C) 2012-2014 Xiongfa Li
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

#ifndef DMBYTEBUFFER_H
#define DMBYTEBUFFER_H

#include "dmrefptr.h"

DM_BEGIN_NAMESPACE
template <typename T>
class DataBuffer: public RefPtr<DataBuffer<T> >
{
public:
    DataBuffer();
    DataBuffer(T *data, duint size, dbool autoFree = true);
    ~DataBuffer();

    void reset(T *data, duint size, dbool autoFree = true);

    const T& at(duint index) const;
    T at(duint index);

    //get the length of string
    duint32 length() const;
    duint32 size() const;

    T* data() const;
    dbool isEmpty() const;
    void clear();

    inline T& operator[](duint i);
    inline const T& operator[](duint i) const;
    DataBuffer& operator=(const DataBuffer &other);

    dbool operator==(const T *data) const;
    dbool operator==(const DataBuffer &other) const;

    dbool operator!=(const T *data) const;
    dbool operator!=(const DataBuffer &other) const;

private:
    T *pData;
    duint uSize;
    dbool bAuto;
};

template <typename T>
inline DataBuffer<T>::DataBuffer():
    pData(NULL),uSize(0),bAuto(false)
{
}

template <typename T>
inline DataBuffer<T>::DataBuffer(T *data, duint size, dbool autoFree /*= true*/):
    pData(data),uSize(size),bAuto(autoFree)
{
}

template <typename T>
inline DataBuffer<T>::~DataBuffer()
{ clear(); }

template <typename T>
void DataBuffer<T>::reset(T *data, duint size, dbool autoFree /*= true*/)
{
    clear();
    pData = data;
    uSize = size;
    bAuto = autoFree;
}

template <typename T>
inline const T& DataBuffer<T>::at(duint index) const
{
    return pData[index];
}

template <typename T>
inline T DataBuffer<T>::at(duint index)
{ return pData[index]; }

template <typename T>
inline duint32 DataBuffer<T>::length() const
{ return uSize; }

template <typename T>
inline duint32 DataBuffer<T>::size() const
{ return uSize; }

template <typename T>
inline T* DataBuffer<T>::data() const
{ return pData; }

template <typename T>
inline dbool DataBuffer<T>::isEmpty() const
{ return uSize == 0; }

template <typename T>
void DataBuffer<T>::clear()
{
    if (bAuto)
        DM_SAFE_DELETE_ARRAY(pData);
    uSize = 0;
}

template <typename T>
inline T& DataBuffer<T>::operator[](duint i)
{ return pData[i]; }

template <typename T>
inline const T& DataBuffer<T>::operator[](duint i) const
{ return pData[i]; }

template <typename T>
inline DataBuffer<T>& DataBuffer<T>::operator=(const DataBuffer<T> &other)
{
    DM_ASSERT(false);
/*
    clear();
    pData = other.pData;
    uSize = other.uSize;
    bAuto = other.bAuto;
    return *this;
    */
}

template <typename T>
inline dbool DataBuffer<T>::operator==(const T *data) const
{ return pData == data; }

template <typename T>
inline dbool DataBuffer<T>::operator==(const DataBuffer &other) const
{ return pData == other.pData && uSize == other.uSize; }

template <typename T>
inline dbool DataBuffer<T>::operator!=(const T *data) const
{ return pData != data; }

template <typename T>
inline dbool DataBuffer<T>::operator!=(const DataBuffer &other) const
{ return !(*this == other); }

typedef DataBuffer<dbyte>  ByteBuffer;

DM_END_NAMESPACE
#endif // DMBYTEBUFFER_H
