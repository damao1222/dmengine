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
#include "dmmath.h"
#include "dmlogger.h"

DM_BEGIN_NAMESPACE
struct VLAActualAlloc
{
    static inline dint alloc(dint nCurrent, dint nGrow)
    {
        if (nCurrent == 0 && nGrow == 0)
            return 0;
        return nCurrent + nGrow;
    }
};

template <dint Multiple = 50>
struct VLAMultipleAlloc
{
    static inline dint alloc(dint nCurrent, dint nGrow)
    {
        if (nCurrent == 0 && nGrow == 0 && Multiple == 0)
            return 0;

        dreal fMul = 1.0f + static_cast<dreal>(Multiple) / 100;
        dint nSize = nCurrent * fMul;
        while (nSize <= nCurrent + nGrow)
        {
            nSize *= fMul;
        };
        return nSize;
    }
};

template <dint ADD = 32>
struct VLAAddAlloc
{
    static inline dint alloc(dint nCurrent, dint nGrow)
    {
        if (nCurrent == 0 && nGrow == 0 && ADD < 0)
            return 0;

        return nCurrent + nGrow + ADD;
    }
};

template <typename T, typename Alloc = VLAActualAlloc >
class VarLenArray
{
public:
    /** 
     * 构造函数.
     * @param nSize  预分配内存大小.
     */
    VarLenArray(dint nSize = 0);

    /** 
     * 使用已有内存缓存构造函数,将拷贝内存的内容到对象中的内存块.
     * @param d  内存指针.
     * @param nSize  预分配内存大小.
     */
    VarLenArray(const T *d, dint nSize);

    /** 
     * 拷贝构造函数，将对拷贝对象进行深度拷贝.
     * @param other  内存指针.
     */
    VarLenArray(const VarLenArray &other);

    /** 
     * 析构函数，删除内存缓存.
     */
    ~VarLenArray();

    /** 
     * 重置内存缓存大小,如果指定大小小于等于现有大小，将不做任何操作.
     * @return 指定重置的内存大小.
     */
    void resize(dint nSize);

    /** 
     * 预留大小.
     * @return 指定预留的内存大小.
     */
    void reserve(dint nSize);

    /** 
     * 在当前数组后追加数组.
     * @param other  需要追加的数组.
     */
    void append(const VarLenArray &other);

    /** 
     * 在当前数组后追加一个元素.
     * @param d  需要追加的元素.
     */
    void append(const T &d);

    /** 
     * 在当前数组后追加一组元素数据.
     * @param d  需要追加数据的指针.
     * @param size  需要追加的元素大小.
     */
    void append(const T *d, dint size);

    /** 
     * 在当前数组前追加数组.
     * @param other  需要追加的数组.
     */
    void prepend(const VarLenArray &other);

    /** 
     * 在当前数组前追加一个元素.
     * @param d  需要追加的元素.
     */
    void prepend(const T &d);

    /** 
     * 在当前数组前追加一组元素数据.
     * @param d  需要追加数据的指针.
     * @param size  需要追加的元素大小.
     */
    void prepend(const T *d, dint size);

    /** 
     * 在指定位置插入数组.
     * @param other  需要插入的数组.
     * @param pos  插入位置.
     */
    void insert(const VarLenArray &other, dint pos);

    /** 
     * 在指定位置插入元素.
     * @param d  需要插入的元素.
     * @param pos  插入位置.
     */
    void insert(const T &d, dint pos);

    /** 
     * 在指定位置插入元素数据.
     * @param d  需要插入的元素数据.
     * @param pos  插入位置.
     * @param size  元素数据大小.
     */
    void insert(const T *d, dint pos, dint size);

    /** 
     * 替换一个元素.
     * @param index  替换元素的位置.
     * @param d  替换的元素.
     */
    void replace(dint index, const T &d);

    /** 
     * 删除某一个元素.
     * @param index  删除元素的索引.
     */
    void remove(dint index);

    /** 
     * 从指定位置删除指定大小的元素.
     * @param index  删除元素的索引.
     * @param size  删除元素的大小.
     */
    void remove(dint index, dint size);

    /** 
     * 内存数据.
     * @return 数据.
     */
    T* data();

    /** 
     * 内存数据.
     * @return 数据.
     */
    const T* data() const;

    /** 
     * 内存数据.
     * @return 数据.
     */
    const T* constData() const;

    /** 
     * 数组大小.
     * @return 数组大小.
     */
    dint size() const;

    /** 
     * 判断数据是否为空.
     * @return 空返回true，否则返回false.
     */
    dbool isEmpty() const;

    /** 
     * 比较两组数据的所有元素，判断两组数组是否相等.
     * @return 完全相等，返回true，否则返回false.
     */
    dbool isEqual(const VarLenArray &other) const;

    /** 
     * 清楚数组内存.
     */
    void clear();

    /** 
     * .
     * @param nSize  需要重新分配的大小.
     * @return 如果发生内存重新分配，则返回之前使用的内存块，否则返回NULL.
     */
    const T& at(dint index) const;

    /** 
     * 重新分配内存块大小，返回之前分配的内存块，需要在对数据进行操作后手工删除该废弃内存.
     * @param nSize  需要重新分配的大小.
     * @return 如果发生内存重新分配，则返回之前使用的内存块，否则返回NULL.
     */
    T& operator[](dint index);

    /** 
     * 重新分配内存块大小，返回之前分配的内存块，需要在对数据进行操作后手工删除该废弃内存.
     * @param nSize  需要重新分配的大小.
     * @return 如果发生内存重新分配，则返回之前使用的内存块，否则返回NULL.
     */
    const T& operator[](dint index) const;

    /** 
     * 重新分配内存块大小，返回之前分配的内存块，需要在对数据进行操作后手工删除该废弃内存.
     * @param nSize  需要重新分配的大小.
     * @return 如果发生内存重新分配，则返回之前使用的内存块，否则返回NULL.
     */
    dbool operator==(const VarLenArray &other) const;

    /** 
     * 重新分配内存块大小，返回之前分配的内存块，需要在对数据进行操作后手工删除该废弃内存.
     * @param nSize  需要重新分配的大小.
     * @return 如果发生内存重新分配，则返回之前使用的内存块，否则返回NULL.
     */
    dbool operator!=(const VarLenArray &other) const;

private:
    T *p; dint s; dint c;
    /** 
     * 重新分配内存块大小，返回之前分配的内存块，需要在对数据进行操作后手工删除该废弃内存.
     * @param nSize  需要重新分配的大小.
     * @return 如果发生内存重新分配，则返回之前使用的内存块，否则返回NULL.
     */
    T* realloc(dint nSize);
    dbool copy(const T *data, dint size, dint pos = 0);
    dbool move(dint src, dint dest, dint size);
    void destroy(dint pos, dint size);
};

template <typename T, typename Alloc>
VarLenArray<T, Alloc>::VarLenArray(dint nSize /*= 0*/):
    p(NULL), s(0), c(0)
{
    realloc(nSize);
}

template <typename T, typename Alloc>
VarLenArray<T, Alloc>::VarLenArray(const T *d, dint nSize):
    p(NULL), s(0), c(0)
{
    append(d, nSize);
}

template <typename T, typename Alloc>
VarLenArray<T, Alloc>::VarLenArray(const VarLenArray &other):
    p(NULL), s(0), c(0)
{
    append(other.p, other.s);
}

template <typename T, typename Alloc>
VarLenArray<T, Alloc>::~VarLenArray()
{
    clear();
}

template <typename T, typename Alloc>
void VarLenArray<T, Alloc>::resize(dint nSize)
{
    if (nSize <= s)
        return ;

    T *old = realloc(nSize);
    if (old)
    {
        copy(old, s,);
        Free(old);
    }
}

template <typename T, typename Alloc>
void VarLenArray<T, Alloc>::reserve(dint nSize)
{
    resize(s + nSize);
}

template <typename T, typename Alloc>
void VarLenArray<T, Alloc>::append(const VarLenArray &other)
{
    if (!other.isEmpty())
    {
        append(other.p, other.s);
    }
}

template <typename T, typename Alloc>
void VarLenArray<T, Alloc>::append(const T &d)
{
    append(&d, 1);
}

template <typename T, typename Alloc>
void VarLenArray<T, Alloc>::append(const T *d, dint size)
{
    if (size <= 0 || !d)
        return ;

    dbool ret = true;
    T *old = realloc(s + size);
    //new memory alloced, copy old
    if (old)
    {
        ret &= copy(old, s);
        Free(old);
    }
    ret &= copy(d, size, s);
    if (ret)
        s += size;
}

template <typename T, typename Alloc>
void VarLenArray<T, Alloc>::prepend(const VarLenArray &other)
{
    if (!other.isEmpty())
    {
        prepend(other.p, other.s);
    }
}

template <typename T, typename Alloc>
void VarLenArray<T, Alloc>::prepend(const T &d)
{
    prepend(&d, 1);
}

template <typename T, typename Alloc>
void VarLenArray<T, Alloc>::prepend(const T *d, dint size)
{
    if (size <= 0 || !d)
        return ;

    dbool ret = true;
    T *old = realloc(s + size);
    //new memory alloced, copy old
    if (old) 
    {
        ret &= copy(d, size);
        ret &= copy(old, s, size);
        Free(old);
    }
    else 
    {
        ret &= move(0, size, s);
        ret &= copy(d, size);
    }
    if (ret)
        s += size;
}

template <typename T, typename Alloc>
void VarLenArray<T, Alloc>::insert(const VarLenArray &other, dint pos)
{
    if (!other.isEmpty())
    {
        insert(other.p, other.s, pos);
    }
}

template <typename T, typename Alloc>
void VarLenArray<T, Alloc>::insert(const T &d, dint pos)
{
    insert(&d, 1, pos);
}

template <typename T, typename Alloc>
void VarLenArray<T, Alloc>::insert(const T *d, dint pos, dint size)
{
    if (pos < 0 && pos > s) return ;

    if (pos == 0)
    {
        prepend(d, size);
        return ;
    }
    else if (pos == s) 
    {
        append(d, size);
        return ;
    }

    if (size <= 0 || !d)
        return ;

    dbool ret = true;
    T *old = realloc(s + size);
    //new memory alloced, copy old
    if (old)
    {
        ret &= copy(old, pos);
        ret &= copy(d, size, pos);
        ret &= copy(old + pos, s - pos, pos + size);
        Free(old);
    }
    else 
    {
        ret &= move(pos, pos + size, s - pos);
        ret &= copy(d, size, pos);
    }
    if (ret)
        s += size;
}

template <typename T, typename Alloc>
void VarLenArray<T, Alloc>::replace(dint index, const T &d)
{
    DM_ASSERT_X(index >= 0 && index < s, "VarLenArray::replace: index out of range");
    p[index] = d;
}

template <typename T, typename Alloc>
void VarLenArray<T, Alloc>::remove(dint index)
{
    remove(index, 1);
}

template <typename T, typename Alloc>
void VarLenArray<T, Alloc>::remove(dint index, dint size)
{
    DM_ASSERT_X(index >= 0 && index < s, "VarLenArray::replace: index out of range");
    dbool ret = true;
    ret &= move(index + size, index, s - index - size);
    destroy(s - size, size);
    if (ret)
        s -= size;
}

template <typename T, typename Alloc>
inline T* VarLenArray<T, Alloc>::data()
{
    return p;
}

template <typename T, typename Alloc>
inline const T* VarLenArray<T, Alloc>::data() const
{
    return p;
}

template <typename T, typename Alloc>
const T* VarLenArray<T, Alloc>::constData() const
{
    return p;
}

template <typename T, typename Alloc>
dint VarLenArray<T, Alloc>::size() const
{
    return s;
}

template <typename T, typename Alloc>
dbool VarLenArray<T, Alloc>::isEmpty() const
{
    return p == NULL || s == 0;
}

template <typename T, typename Alloc>
dbool VarLenArray<T, Alloc>::isEqual(const VarLenArray &other) const
{
    if (s != other.s) {
        return false;
    }
    else {
        if (p == other.p)
            return true;
    }

    for (dint i=0; i<s; ++i)
    {
        if (data()[i] != other.data()[i])
            return false;
    }
    return true;
}

template <typename T, typename Alloc>
void VarLenArray<T, Alloc>::clear()
{
    if (p)
    {
        Free(p); p = NULL;
    }
    s = c = 0;
}

template <typename T, typename Alloc>
const T& VarLenArray<T, Alloc>::at(dint index) const
{
    return p[index];
}

template <typename T, typename Alloc>
T& VarLenArray<T, Alloc>::operator[](dint index)
{
    return p[index];
}

template <typename T, typename Alloc>
const T& VarLenArray<T, Alloc>::operator[](dint index) const
{
    return p[index];
}

template <typename T, typename Alloc>
dbool VarLenArray<T, Alloc>::operator==(const VarLenArray &other) const
{
    return p == other.p && s == other.s;
}

template <typename T, typename Alloc>
dbool VarLenArray<T, Alloc>::operator!=(const VarLenArray &other) const
{
    return !((*this) == other);
}

template <typename T, typename Alloc>
T* VarLenArray<T, Alloc>::realloc(dint nSize)
{
    if (nSize == 0) return NULL;
    
    if (c >= nSize)
        return NULL;
    else 
    {
        dint size = Alloc::alloc(s * sizeof(T), (nSize - s) * sizeof(T));
        T* np = (T*)Malloc(size);
        T* tmp = p;
        DM_CHECK_PTR(np);

        c = size;
        p = np;
        //dont update size
        //s = nSize;
        return tmp;
    }
}

template <typename T, typename Alloc>
dbool VarLenArray<T, Alloc>::copy(const T *data, dint size, dint pos /*= 0*/)
{
    if (pos + size > c)
        return false;

    dint i = 0;
    while (i < size) 
    {
        p[pos + i] = data[i];
        i++;
    }
    return true;
}

template <typename T, typename Alloc>
dbool VarLenArray<T, Alloc>::move(dint src, dint dest, dint size)
{
    if (src == dest) return true;
    else if (src < dest) {
        if (dest + size >= c)
            return false;

        dint from = src + size -1, to = dest + size - 1, i = 0;
        while (i < size) {
            p[to - i] = p[from - i];
            i++;
        }
        return true;
    } else {
        if (dest < 0)
            return false;

        dint from = src, to = dest, i = 0;
        while (i < size) {
            p[to + i] = p[from + i];
            i++;
        }
        return true;
    }
}

template <typename T, typename Alloc>
void VarLenArray<T, Alloc>::destroy(dint pos, dint size)
{
    if (pos < 0 || size <= 0 || pos + size > s)
        return ;

    for (dint i=0; i<size; ++i)
    {
        p[pos + i] = 0;
    }
}

DM_END_NAMESPACE

DMVARLENARRAY_H
#endif // DMBYTEARRAY_H
