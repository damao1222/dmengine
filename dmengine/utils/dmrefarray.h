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
     * ���캯��
     */
    RefArray();

    /** 
     * ������������release���ж����еĶ���
     */
    ~RefArray();

    /** 
     * ����һ�����󣬲�retain��ͬappend
     * @param p  ����ָ��
     */
    void addObject(ArrayObject *p);

    /** 
     * �����Ƴ�һ����������ö����ڶ��������Ƴ�����release
     * @param p  ����ָ��
     * @return �Ƴ��ɹ�����true�����򷵻�false
     */
    dbool removeObject(ArrayObject *p);

    /** 
     * �Ƴ���release�����е����ж���
     */
    void removeAll();

    /** 
     * �ڶ���ĩβ����һ�����󣬲�retain
     * @param p  ����ָ��
     */
    void append(ArrayObject *p);

    /** 
     * �ڶ���ǰ�μ���һ�����󣬲�retain
     * @param p  ����ָ��
     */
    void prepend(ArrayObject *p);

    /** 
     * ��ͷ����һ������
     * @param p  ����ָ��
     * @return ���ض������������������ڷ���-1
     */
    dint indexOf(ArrayObject *p)  const;

    /** 
     * ��β������һ������
     * @param p  ����ָ��
     * @return ���ض������������������ڷ���-1
     */
    dint lastIndexOf(ArrayObject *p)  const;

    /** 
     * �����������ض���
     * @param index  ����
     * @return ���ض���
     */
    ArrayObject* at(dint index) const;

    /** 
     * �Ƴ�����λ���ϵĶ��󣬲�release
     * @param index  ����
     */
    void removeAt(dint index);

    /** 
     * �ж϶����Ƿ�Ϊ��
     * @return �շ���true�����򷵻�false
     */
    dbool isEmpty() const;

    /** 
     * ���г���
     * @return ����
     */
    dint count() const;

    /** 
     * ���г���
     * @return ����
     */
    dint size() const;

    /** 
     * ��õ�һ������
     * @return ����ָ��
     */
    ArrayObject* first() const;

    /** 
     * ������һ������
     * @return ����ָ��
     */
    ArrayObject* last() const;
    
    /** 
     * �Ƴ���һ������release
     */
    void removeFirst();

    /** 
     * �Ƴ����һ������release
     */
    void removeLast();

    /** 
     * ��õ�һ�������û������ֶ�release�������
     * @return ����ָ��
     */
    ArrayObject* takeFirst();

    /** 
     * ������һ�������û������ֶ�release�������
     * @return ����ָ��
     */
    ArrayObject* takeLast();

    /** 
     * �����������ض���
     * @param index  ����
     * @return ���ض���
     */
    ArrayObject* operator[](dint i);

    /** 
     * �����������ض���
     * @param index  ����
     * @return ���ض���
     */
    ArrayObject* operator[](dint i) const;

    /** 
     * ��ö��еĿ���
     * @return ����
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
