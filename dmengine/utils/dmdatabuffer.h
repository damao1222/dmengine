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
     * 默认构造函数
     */
    DataBuffer();

    /** 
     * 使用内存块指针、内存块长度来创建buffer
     * @param data  内存块指针
     * @param size  内存块长度
     * @param deepCopy  是否在对数据元素操作时深度拷贝内存块
     */
    DataBuffer(T *data, dint64 size, dbool deepCopy = false);

    /** 
     * 拷贝构造函数
     */
    DataBuffer(const DataBuffer &other);

    /** 
     * 使用内存块长度来创建buffer,new一块参数size大小的内存
     * @param size  内存块长度
     * @param deepCopy  是否在对数据元素操作时深度拷贝内存块
     */
    explicit DataBuffer(dint64 size, dbool deepCopy = false);

    /** 
     * 析构函数，尝试释放内存块
     */
    ~DataBuffer();

    /** 
     * 分离，不再与其他buffer共享内存块数据
     */
    void detach();

    /** 
     * 重置当前buffer，可能深拷贝（根据deepCopy）
     * @param data  内存块指针
     * @param size  内存块长度
     * @param deepCopy  是否在对数据元素操作时深度拷贝内存块
     */
    void reset(T *data, dint64 size, dbool deepCopy = false);

    /** 
     * 获得索引上的内存块元素，不会产生深度拷贝
     * @param index  内存块索引
     * @return 内存块元素
     */
    const T& at(dint64 index) const;

    /** 
     * 获得内存块长度
     * @return 长度
     */
    dint64 length() const;

    /** 
     * 获得内存块长度
     * @return 长度
     */
    dint64 size() const;

    /** 
     * 获得内存块指针
     * @return 内存块指针
     */
    T* data() const;

    /** 
     * 判断内存块是否为空
     * @return 内存块为空返回true，否则返回false
     */
    dbool isEmpty() const;

    /** 
     * 清除当前buffer，尝试释放内存块
     */
    void clear();

    /** 
     * 获得索引上的内存块元素，将分离，可能进行深度拷贝（根据deepCopy）
     * @param index  内存块索引
     * @return 内存块元素
     */
    inline T& operator[](dint64 index);

    /** 
     * 获得索引上的内存块元素，不会产生深度拷贝
     * @param index  内存块索引
     * @return 内存块元素
     */
    inline const T& operator[](dint64 index) const;

    /** 
     * 赋值运算符，浅拷贝
     * @param other  拷贝的对象
     * @return 当前实例引用
     */
    DataBuffer& operator=(const DataBuffer &other);

    /** 
     * 等于运算符
     * @param data  内存块指针
     * @return 当buffer与内存块指针一致时返回true，否则返回false
     */
    dbool operator==(const T *data) const;

    /** 
     * 等于运算符
     * @param other  另一个buffer
     * @return 该buffer与other的内存指针、内存大小都一致时，返回true，否则返回false
     */
    dbool operator==(const DataBuffer &other) const;

    /** 
     * 不等运算符
     * @param data  内存块指针
     * @return 当buffer与内存块指针不一致时返回true，否则返回false
     */
    dbool operator!=(const T *data) const;

    /** 
     * 不等运算符
     * @param other  另一个buffer
     * @return 该buffer与other的内存指针、内存大小有一项不一致时，返回true，否则返回false
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
