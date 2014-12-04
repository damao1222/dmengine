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

#ifndef DMDATABUFFER_H
#define DMDATABUFFER_H

#include "dmrefptr.h"

DM_BEGIN_NAMESPACE

template <typename T>
struct DataBufferDeleter
{
    static inline void cleanup(T *pointer)
    {
        // Enforce a complete type.
        // If you get a compile error here, read the section on forward declared classes
        typedef char IsIncompleteType[ sizeof(T) ? 1 : -1 ];
        (void) sizeof(IsIncompleteType);

        delete pointer;
    }
};

template <typename T>
struct DataBufferArrayDeleter
{
    static inline void cleanup(T *pointer)
    {
        // Enforce a complete type.
        // If you get a compile error here, read the section on forward declared classes
        typedef char IsIncompleteType[ sizeof(T) ? 1 : -1 ];
        (void) sizeof(IsIncompleteType);

        delete [] pointer;
    }
};

template <typename T>
struct DataBufferReleaser
{
    static inline void cleanup(T *pointer)
    {
        // Enforce a BaseRefPtr.
        // If you get a compile error here, read the section on forward declared classes
        BaseRefPtr *p = static_cast<BaseRefPtr*>(pointer);

        p->release();
    }
};

struct DM_DLL_EXPORT DataBufferData {
    struct Data {
        BasicAtomicInt ref;
        duint32 sharable : 1;
        void* ptr;
        dint64 uSize;
        dbool bCopy;
    };

    Data* creat(void* ptr, dint64 uSize, dbool bCopy);
    Data* detach();
    Data* detach(void* ptr, dint64 uSize, dbool bCopy);
    static Data shared_null;
    Data *d;
};

template <typename T, typename Cleanup = DataBufferArrayDeleter<T> >
class DataBuffer: public RefPtr<DataBuffer<T, Cleanup> >
{
public:
    /** 
     * Ĭ�Ϲ��캯��
     */
    DataBuffer();

    /** 
     * ʹ���ڴ��ָ�롢�ڴ�鳤��������buffer
     * @param data  �ڴ��ָ��
     * @param size  �ڴ�鳤��
     * @param deepCopy  �Ƿ��ڶ�����Ԫ�ز���ʱ��ȿ����ڴ��
     */
    DataBuffer(T *data, dint64 size, dbool deepCopy = false);

    /** 
     * �������캯��
     */
    DataBuffer(const DataBuffer &other);

    /** 
     * ʹ���ڴ�鳤��������buffer,newһ�����size��С���ڴ�
     * @param size  �ڴ�鳤��
     * @param deepCopy  �Ƿ��ڶ�����Ԫ�ز���ʱ��ȿ����ڴ��
     */
    explicit DataBuffer(dint64 size, dbool deepCopy = false);

    /** 
     * ���������������ͷ��ڴ��
     */
    ~DataBuffer();

    /** 
     * ���룬����������buffer�����ڴ������
     */
    void detach();

    /** 
     * ���õ�ǰbuffer���������������deepCopy��
     * @param data  �ڴ��ָ��
     * @param size  �ڴ�鳤��
     * @param deepCopy  �Ƿ��ڶ�����Ԫ�ز���ʱ��ȿ����ڴ��
     */
    void reset(T *data, dint64 size, dbool deepCopy = false);

    /** 
     * ��������ϵ��ڴ��Ԫ�أ����������ȿ���
     * @param index  �ڴ������
     * @return �ڴ��Ԫ��
     */
    const T& at(dint64 index) const;

    /** 
     * ����ڴ�鳤��
     * @return ����
     */
    dint64 length() const;

    /** 
     * ����ڴ�鳤��
     * @return ����
     */
    dint64 size() const;

    /** 
     * ����ڴ��ָ��
     * @return �ڴ��ָ��
     */
    T* data() const;

    /** 
     * �ж��ڴ���Ƿ�Ϊ��
     * @return �ڴ��Ϊ�շ���true�����򷵻�false
     */
    dbool isEmpty() const;

    /** 
     * �����ǰbuffer�������ͷ��ڴ��
     */
    void clear();

    /** 
     * ��������ϵ��ڴ��Ԫ�أ������룬���ܽ�����ȿ���������deepCopy��
     * @param index  �ڴ������
     * @return �ڴ��Ԫ��
     */
    inline T& operator[](dint64 index);

    /** 
     * ��������ϵ��ڴ��Ԫ�أ����������ȿ���
     * @param index  �ڴ������
     * @return �ڴ��Ԫ��
     */
    inline const T& operator[](dint64 index) const;

    /** 
     * ��ֵ�������ǳ����
     * @param other  �����Ķ���
     * @return ��ǰʵ������
     */
    DataBuffer& operator=(const DataBuffer &other);

    /** 
     * ���������
     * @param data  �ڴ��ָ��
     * @return ��buffer���ڴ��ָ��һ��ʱ����true�����򷵻�false
     */
    dbool operator==(const T *data) const;

    /** 
     * ���������
     * @param other  ��һ��buffer
     * @return ��buffer��other���ڴ�ָ�롢�ڴ��С��һ��ʱ������true�����򷵻�false
     */
    dbool operator==(const DataBuffer &other) const;

    /** 
     * ���������
     * @param data  �ڴ��ָ��
     * @return ��buffer���ڴ��ָ�벻һ��ʱ����true�����򷵻�false
     */
    dbool operator!=(const T *data) const;

    /** 
     * ���������
     * @param other  ��һ��buffer
     * @return ��buffer��other���ڴ�ָ�롢�ڴ��С��һ�һ��ʱ������true�����򷵻�false
     */
    dbool operator!=(const DataBuffer &other) const;

protected:
    union { DataBufferData p; DataBufferData::Data *d; };
    void detach_helper();
    void detach_helper(T* ptr, dint64 uSize, dbool bCopy);
    void free(DataBufferData::Data *data);
};

template <typename T, typename Cleanup>
inline DataBuffer<T, Cleanup>::DataBuffer(): d(&DataBufferData::shared_null)
{ d->ref.ref(); }

template <typename T, typename Cleanup>
inline DataBuffer<T, Cleanup>::DataBuffer(T *data, dint64 size, dbool deepCopy /*= true*/):
    d(p.creat(data, size, deepCopy))
{
}

template <typename T, typename Cleanup>
inline DataBuffer<T, Cleanup>::DataBuffer(dint64 size, dbool deepCopy /*= false*/):
    d(p.creat(new T[size], size, deepCopy))
{
}

template <typename T, typename Cleanup>
inline DataBuffer<T, Cleanup>::DataBuffer(const DataBuffer &other) : d(other.d) 
{ 
    d->ref.ref(); if (!d->sharable) detach_helper(); 
}

template <typename T, typename Cleanup>
inline DataBuffer<T, Cleanup>::~DataBuffer()
{ 
    if (!d->ref.deref())
        free(d);
}

template <typename T, typename Cleanup>
inline void DataBuffer<T, Cleanup>::detach()
{
    if (d->ref != 1) 
        detach_helper();
}

template <typename T, typename Cleanup>
void DataBuffer<T, Cleanup>::reset(T *ptr, dint64 uSize, dbool bCopy /*= true*/)
{
    DataBufferData::Data *x = p.detach(ptr, uSize, bCopy);
    if (!x->ref.deref())
        free(x);
}

template <typename T, typename Cleanup>
inline const T& DataBuffer<T, Cleanup>::at(dint64 index) const
{
    return static_cast<T*>(d->ptr)[index];
}

template <typename T, typename Cleanup>
inline dint64 DataBuffer<T, Cleanup>::length() const
{ 
    return d->uSize; 
}

template <typename T, typename Cleanup>
inline dint64 DataBuffer<T, Cleanup>::size() const
{ return d->uSize; ; }

template <typename T, typename Cleanup>
inline T* DataBuffer<T, Cleanup>::data() const
{ 
    return static_cast<T*>(d->ptr); 
}

template <typename T, typename Cleanup>
inline dbool DataBuffer<T, Cleanup>::isEmpty() const
{ 
    return d->uSize == 0; 
}

template <typename T, typename Cleanup>
void DataBuffer<T, Cleanup>::clear()
{
    *this = DataBuffer();
}

template <typename T, typename Cleanup>
inline T& DataBuffer<T, Cleanup>::operator[](dint64 index)
{ 
    detach();
    return static_cast<T*>(d->ptr)[index]; 
}

template <typename T, typename Cleanup>
inline const T& DataBuffer<T, Cleanup>::operator[](dint64 index) const
{ return static_cast<T*>(d->ptr)[index];  }

template <typename T, typename Cleanup>
inline DataBuffer<T, Cleanup>& DataBuffer<T, Cleanup>::operator=(const DataBuffer<T, Cleanup> &other)
{
    if (this == &other)
        return *this;

    if (d != other.d) {
        DataBufferData::Data *o = other.d;
        o->ref.ref();
        if (!d->ref.deref())
            free(d);
        d = o;
        if (!d->sharable)
            detach_helper();
    }
    return (*this);
}

template <typename T, typename Cleanup>
inline dbool DataBuffer<T, Cleanup>::operator==(const T *data) const
{ 
    return d->ptr == data;
}

template <typename T, typename Cleanup>
inline dbool DataBuffer<T, Cleanup>::operator==(const DataBuffer &other) const
{ 
    if (d == other.d)
        return true;

    if (d->ptr == other.d->ptr && d->uSize == other.d->uSize) 
    {
        return true;
    }
    return false;
}

template <typename T, typename Cleanup>
inline dbool DataBuffer<T, Cleanup>::operator!=(const T *data) const
{ 
    return !operator==(data);
}

template <typename T, typename Cleanup>
inline dbool DataBuffer<T, Cleanup>::operator!=(const DataBuffer &other) const
{ return !(*this == other); }

template <typename T, typename Cleanup>
inline void DataBuffer<T, Cleanup>::detach_helper()
{
    DataBufferData::Data *x = p.detach();
    if (!x->ref.deref())
        free(x);
}

template <typename T, typename Cleanup>
inline void DataBuffer<T, Cleanup>::detach_helper(T* ptr, dint64 uSize, dbool bCopy)
{
    DataBufferData::Data *x = p.detach(ptr, uSize, bCopy);
    if (!x->ref.deref())
        free(x);
}

template <typename T, typename Cleanup>
void DataBuffer<T, Cleanup>::free(DataBufferData::Data *data)
{
    if (data->ptr != NULL)
        Cleanup::cleanup(static_cast<T*>(data->ptr));
    DM_SAFE_DELETE(data);
}

typedef DataBuffer<dbyte>  ByteBuffer;
typedef DataBuffer<BaseRefPtr, DataBufferReleaser<BaseRefPtr> > SingleRef;

DM_END_NAMESPACE
#endif // DMDATABUFFER_H
