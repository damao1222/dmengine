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

   Reference Count: (shared_ptr)

    modify:
        2014/01/15 ： 突破2012/01/19版本的使用限制，其中第2点 可以用过take接口来解决：

            CLASS* A = new CLASS; DM::Ref ref(A);  ref.take(); DM::Ref another_ref(A);
            其中继承关系的指针可通过 DM::RefEval(A, B)来赋值

        2012/01/19 : 创建

            使用限制：
            由于可能会引起内存泄漏、重复析构以及错误引用内存地址等问题，应避免如下情况：
            1、不要构造一个临时的DM::Ref作为函数的参数 ：
                foo(DM::Ref(new CLASS));
            2、不要同时为同一个指针做引用计数，避免循环引用:
                CLASS* A = new CLASS; DM::Ref ref(A); DM::Ref another_ref(A);
            3、避免在非同一个作用域内使用list，vector等容器装载DM::Ref对象 :
                {std::list< DM::Ref<CLASS> > mList; mList.push_back(ref);} {mList[i].get()->do()}
               如果确实需要使用，请使用swap将容器内的对象与装载对象交换，使容器获取主释放权 ：
                {list.push_back(ref);list.back().swap(ref);} list.clean();//release here
*/

#ifndef DMREFCOUNT_H
#define DMREFCOUNT_H
#include "dmatomicint.h"
DM_BEGIN_NAMESPACE

struct DM_DLL_EXPORT RefData {
    struct Data {
        BasicAtomicInt ref;
        void* ptr;
    };

    Data *setExc(void *p);
    Data *create(void *p);

    static Data shared_null;
    Data *d;
};

template<class T>
class Ref
{
public:
    inline Ref();
    inline Ref(T *p);
    inline Ref(const Ref &o);
    ~Ref();

    inline T *get();
    inline const T *get() const;

    //Notice: Will out of control, the pointer need to be deleted by yourself
    inline T *take();

    inline dint refCount() const;
    inline dbool isNull() const;

    inline dbool isShared() const;
    inline dbool isSharedWith(const Ref &other) const;

    inline T* operator->();
    inline T& operator*();

    inline const T* operator->() const;
    inline const T& operator*() const;

    inline Ref& operator=(T *p);
    inline Ref& operator=(const Ref &o);

    inline dbool operator==(T *p) const;
    inline dbool operator==(const Ref &o) const;

    inline dbool operator!=(T *p) const;
    inline dbool operator!=(const Ref &o) const;

    inline operator bool() const;
    inline bool operator!() const;
  
    inline void clear();

    template <class X> Ref<X> staticCast() const;
    template <class X> Ref<X> dynamicCast() const;
    template <class X> Ref<X> constCast() const;

private:
    inline void retain();
    inline void release();
    void set(T *p);
    void free(RefData::Data *d);
    void internalSwapData(RefData::Data *d);
    union { RefData p; RefData::Data *d; };
    template<class A, class B> friend Ref<A> & RefConstEval(Ref<A> &dest, const Ref<B> &src);
    template<class A, class B> friend Ref<A> & RefDynamicEval(Ref<A> &dest, const Ref<B> &src);
    template<class A, class B> friend Ref<A> & RefStaticEval(Ref<A> &dest, const Ref<B> &src);
};

template<class T>
inline Ref<T>::Ref(): d(&RefData::shared_null)
{ d->ref.ref(); }

template<class T>
inline Ref<T>::Ref(T *ptr):
    d(p.create(ptr))
{
}

template<class T>
inline Ref<T>::Ref(const Ref<T> &o): d(o.d)
{ d->ref.ref(); }

template<class T>
Ref<T>::~Ref()
{
    release();
}

template<class T>
inline T *Ref<T>::get()
{
    return static_cast<T*>(d->ptr);
}

template<class T>
inline const T *Ref<T>::get() const
{
    return static_cast<T*>(d->ptr);
}

template<class T>
inline T *Ref<T>::take()
{
    T *tmp = get();
    d->ptr = 0;
    return tmp;
}

template<class T>
inline dint Ref<T>::refCount() const
{
    return d->ref._value_;
}

template<class T>
inline void Ref<T>::retain()
{
    d->ref.ref();
}

template<class T>
inline void Ref<T>::release()
{
    if (d->ref._value_ > 0)
    {
        if (!d->ref.deref())
            free(d);
    }
}

template<class T>
inline dbool Ref<T>::isNull() const
{
    return d->ptr == 0;
}

template<class T>
inline Ref<T>& Ref<T>::operator=(T *p)
{
    set(p);
    return *this;
}

template<class T>
inline Ref<T>& Ref<T>::operator=(const Ref<T> &other)
{
    if (this == &other)
        return *this;

    if (d != other.d) {
        RefData::Data *o = other.d;
        o->ref.ref();
        if (!d->ref.deref())
            free(d);
        d = o;
    }
    return (*this);
}

template<class T>
inline dbool Ref<T>::operator==(T *p) const
{
    return d->ptr == p;
}

template<class T>
inline dbool Ref<T>::operator==(const Ref<T> &other) const
{
    if (this == &other || d == other.d)
        return true;

    return d->ptr == other.d->ptr;
}

template<class T>
inline dbool Ref<T>::operator!=(T *p) const
{
    return !operator==(p);
}

template<class T>
inline dbool Ref<T>::operator!=(const Ref<T> &o) const
{
    return !operator==(o);
}

template<class T>
inline Ref<T>::operator bool() const 
{ return !isNull(); }

template<class T>
inline bool Ref<T>::operator !() const 
{ return isNull(); }

template<class T>
inline dbool Ref<T>::isShared() const { return d->ref > 1; }

template<class T>
inline dbool Ref<T>::isSharedWith(const Ref<T> &other) const { return d == other.d; }

template<class T>
inline T* Ref<T>::operator->()
{
    return get();
}

template<class T>
inline T& Ref<T>::operator*()
{
    return *get();
}

template<class T>
inline const T* Ref<T>::operator->() const
{
    return get();
}

template<class T>
inline const T& Ref<T>::operator*() const
{
    return *get();
}

template<class T> 
template <class X> 
inline Ref<X> Ref<T>::staticCast() const
{
    Ref<X> ref;
    RefStaticEval(ref, *this);
    return ref;
}

template<class T>
template <class X> 
inline Ref<X> Ref<T>::dynamicCast() const
{
    Ref<X> ref;
    RefDynamicEval(ref, *this);
    return ref;
}

template<class T>
template <class X> 
inline Ref<X> Ref<T>::constCast() const
{
    Ref<X> ref;
    RefConstEval(ref, *this);
    return ref;
}

template<class T>
void Ref<T>::internalSwapData(RefData::Data *data)
{
    if (d != data) {
        data->ref.ref();
        if (!d->ref.deref())
            free(d);
        d = data;
    }
}

template<class T>
inline void Ref<T>::clear()
{
    *this = NULL;
}

template<class T>
void Ref<T>::set(T *ptr)
{
    if (ptr != d->ptr)
    {
        RefData::Data *x = p.setExc(ptr);
        if (!x->ref.deref())
            free(x);
    }
}

template<class T>
void Ref<T>::free(RefData::Data *data)
{
    if (data->ptr)
        delete static_cast<T*>(data->ptr);

    DM_SAFE_DELETE(data);
}
#if 0
template<class A, class B>
Ref<A> & RefEval(Ref<A> &dest, const Ref<B> &src)
{
    //Dummy, cause compiler error if B is not a sub class of A
    const A *dummy = src.get();
    dest.internalSwapData(src.d);
    return dest;
}
#endif
template<class A, class B>
Ref<A> & RefConstEval(Ref<A> &dest, const Ref<B> &src)
{
    //Dummy, cause compiler error if B is not a sub class of A
    A *dummy = const_cast<A*>(src.get());
    dest.internalSwapData(src.d);
    return dest;
}

template<class A, class B>
Ref<A> & RefDynamicEval(Ref<A> &dest, const Ref<B> &src)
{
    //Dummy, cause Compiler ERROR if there is not inheritance relationship between A and B. 
    //Will evaluate value if src is A type else evaluate NULL
    A *dummy = dynamic_cast<A*>(src.d->ptr);
    if (dummy)
    {
        dest.internalSwapData(src.d);
    }
    else
    {
        dest = NULL;
    //    DM_LOGW("Cannot evaluate Ref value");
    }
    return dest;
}

template<class A, class B>
Ref<A> & RefStaticEval(Ref<A> &dest, const Ref<B> &src)
{
    //Dummy, cause Compiler ERROR if there is not inheritance relationship between A and B.
    A *dummy = static_cast<A*>(src.d->ptr);
    dest.internalSwapData(src.d);
    return dest;
}

DM_END_NAMESPACE

#endif // DMREFCOUNT_H
