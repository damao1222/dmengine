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
#ifndef DMREFARRAY_H
#define DMREFARRAY_H
#include "dmnamespace.h"
#include "dmrefptr.h"
#include "private/dmrefarray_p.h"

DM_BEGIN_NAMESPACE

typedef BaseRefPtr ArrayObject;

DM_PRIVATE_CLASS(RefArray);
class DM_DLL_EXPORT RefArray: public RefPtr<RefArray>
{
    DM_DECLARE_PRIVATE(RefArray)
public:
    /** 
     * 构造函数
     */
    RefArray();

    /** 
     * 析构函数，将release所有队列中的对象
     */
    ~RefArray();

    /** 
     * 加入一个对象，并retain，同append
     * @param p  对象指针
     */
    void addObject(ArrayObject *p);

    /** 
     * 尝试移除一个对象，如果该对象在队列中则移出，并release
     * @param p  对象指针
     * @return 移出成功返回true，否则返回false
     */
    dbool removeObject(ArrayObject *p);

    /** 
     * 移除并release队列中的所有对象
     */
    void removeAll();

    /** 
     * 在队列末尾加入一个对象，并retain
     * @param p  对象指针
     */
    void append(ArrayObject *p);

    /** 
     * 在队列前段加入一个对象，并retain
     * @param p  对象指针
     */
    void prepend(ArrayObject *p);

    /** 
     * 从头查找一个对象
     * @param p  对象指针
     * @return 返回对象的索引，如果不存在返回-1
     */
    dint indexOf(ArrayObject *p)  const;

    /** 
     * 从尾部查找一个对象
     * @param p  对象指针
     * @return 返回对象的索引，如果不存在返回-1
     */
    dint lastIndexOf(ArrayObject *p)  const;

    /** 
     * 根据索引返回对象
     * @param index  索引
     * @return 返回对象
     */
    ArrayObject* at(dint index) const;

    /** 
     * 移除索引位置上的对象，并release
     * @param index  索引
     */
    void removeAt(dint index);

    /** 
     * 判断队列是否为空
     * @return 空返回true，否则返回false
     */
    dbool isEmpty() const;

    /** 
     * 队列长度
     * @return 长度
     */
    dint count() const;

    /** 
     * 队列长度
     * @return 长度
     */
    dint size() const;

    /** 
     * 获得第一个对象
     * @return 对象指针
     */
    ArrayObject* first() const;

    /** 
     * 获得最后一个对象
     * @return 对象指针
     */
    ArrayObject* last() const;
    
    /** 
     * 移除第一个对象并release
     */
    void removeFirst();

    /** 
     * 移除最后一个对象并release
     */
    void removeLast();

    /** 
     * 获得第一个对象，用户必须手动release这个对象
     * @return 对象指针
     */
    ArrayObject* takeFirst();

    /** 
     * 获得最后一个对象，用户必须手动release这个对象
     * @return 对象指针
     */
    ArrayObject* takeLast();

    /** 
     * 根据索引返回对象
     * @param index  索引
     * @return 返回对象
     */
    ArrayObject* operator[](dint i);

    /** 
     * 根据索引返回对象
     * @param index  索引
     * @return 返回对象
     */
    ArrayObject* operator[](dint i) const;

    /** 
     * 获得队列的拷贝
     * @return 拷贝
     */
    RefArray* clone() const;

protected:
    RefArray(const RefArray &other) { DM_ASSERT(false); }
    RefArray& operator=(const RefArray &other) { DM_ASSERT(false); return *this; }
};
DM_END_NAMESPACE

#define DM_FOREACH_ARRAY(ITEM, ARRAY) \
        DM_NS::ArrayContainer __array__(ARRAY, &ITEM);   \
        if (!ARRAY->isEmpty())                      \
            for (int __index__=0; __array__.check(__index__); ++__index__)
#endif //DMREFARRAY_H
