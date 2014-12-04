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

#ifndef DMVECTOR_H
#define DMVECTOR_H
#include "dmatomicint.h"
#include "dmtypeinfo.h"
#include "dmnamespace.h"
#include "dmmath.h"
#include "dmmalloc.h"

#ifdef DM_STL
#include <iterator>
#include <list>
#endif
#ifdef DM_COMPILER_INITIALIZER_LISTS
#include <iterator>
#include <initializer_list>
#endif

#include <new>
#include <limits.h>
#include <string.h>

DM_BEGIN_NAMESPACE

struct DM_DLL_EXPORT VectorData {
    struct Data {
        BasicAtomicInt ref;
        dint alloc, begin, end;
        duint32 sharable : 1;
        void *array[1];
    };
    enum { DataHeaderSize = sizeof(Data) - sizeof(void *) };

    void init(dint alloc = 0);
    Data *detach(dint alloc);
    Data *detach_grow(dint *i, dint n);
    Data *detach(); 
    Data *detach2(); 
    Data *detach3(); 
    void realloc(dint alloc);
    static Data shared_null;
    Data *d;
    void **erase(void **xi);
    void **append(dint n);
    void **append();
    void **append(const VectorData &l);
    void **append2(const VectorData &l); 
    void **prepend();
    void **insert(dint i);
    void remove(dint i);
    void remove(dint i, dint n);
    void move(dint from, dint to);
    inline dint size() const { return d->end - d->begin; }
    inline dbool isEmpty() const { return d->end  == d->begin; }
    inline void **at(dint i) const { return d->array + d->begin + i; }
    inline void **begin() const { return d->array + d->begin; }
    inline void **end() const { return d->array + d->end; }
};

template <typename T>
class Vector
{
    struct Node { void *v;
        inline T &t()
        { return *reinterpret_cast<T*>(TypeInfo<T>::isLarge || TypeInfo<T>::isStatic
                                       ? v : this); }
    };

    union { VectorData p; VectorData::Data *d; };

public:
    inline Vector() : d(&VectorData::shared_null) { d->ref.ref(); }
    inline Vector(const Vector<T> &l) : d(l.d) { d->ref.ref(); if (!d->sharable) detach_helper(); }
    inline Vector(dint size);
    inline Vector(dint size, const T& t);
    ~Vector();
    Vector<T> &operator=(const Vector<T> &l);

    inline void swap(Vector<T> &other) { Swap(d, other.d); }

    dbool operator==(const Vector<T> &l) const;
    inline dbool operator!=(const Vector<T> &l) const { return !(*this == l); }

    inline dint size() const { return p.size(); }

    inline void detach() { if (d->ref != 1) detach_helper(); }

    inline void detachShared()
    {
        if (d->ref != 1 && this->d != &VectorData::shared_null)
            detach_helper();
    }

    inline dbool isDetached() const { return d->ref == 1; }
    inline void setSharable(dbool sharable) { if (!sharable) detach(); d->sharable = sharable; }
    inline dbool isSharedWith(const Vector<T> &other) const { return d == other.d; }

    inline dbool isEmpty() const { return p.isEmpty(); }

    void clear();

    const T &at(dint i) const;
    const T &operator[](dint i) const;
    T &operator[](dint i);

    void reserve(dint size);
    void append(const T &t);
    void append(const Vector<T> &t);
    void prepend(const T &t);
    void insert(dint i, const T &t);
    void replace(dint i, const T &t);
    void removeAt(dint i);
    dint removeAll(const T &t);
    dbool removeOne(const T &t);
    T takeAt(dint i);
    T takeFirst();
    T takeLast();
    void move(dint from, dint to);
    void swap(dint i, dint j);
    dint indexOf(const T &t, dint from = 0) const;
    dint lastIndexOf(const T &t, dint from = -1) const;
    dbool contains(const T &t) const;
    dint count(const T &t) const;

    class const_iterator;

    class iterator {
    public:
        Node *i;
        typedef T value_type;
        typedef T *pointer;
        typedef T &reference;

        inline iterator() : i(0) {}
        inline iterator(Node *n) : i(n) {}
        inline iterator(const iterator &o): i(o.i){}
        inline T &operator*() const { return i->t(); }
        inline T *operator->() const { return &i->t(); }
        inline T &operator[](dint j) const { return i[j].t(); }
        inline dbool operator==(const iterator &o) const { return i == o.i; }
        inline dbool operator!=(const iterator &o) const { return i != o.i; }
        inline dbool operator<(const iterator& other) const { return i < other.i; }
        inline dbool operator<=(const iterator& other) const { return i <= other.i; }
        inline dbool operator>(const iterator& other) const { return i > other.i; }
        inline dbool operator>=(const iterator& other) const { return i >= other.i; }

        inline dbool operator==(const const_iterator &o) const
            { return i == o.i; }
        inline dbool operator!=(const const_iterator &o) const
            { return i != o.i; }
        inline dbool operator<(const const_iterator& other) const
            { return i < other.i; }
        inline dbool operator<=(const const_iterator& other) const
            { return i <= other.i; }
        inline dbool operator>(const const_iterator& other) const
            { return i > other.i; }
        inline dbool operator>=(const const_iterator& other) const
            { return i >= other.i; }

        inline iterator &operator++() { ++i; return *this; }
        inline iterator operator++(dint) { Node *n = i; ++i; return n; }
        inline iterator &operator--() { i--; return *this; }
        inline iterator operator--(dint) { Node *n = i; i--; return n; }
        inline iterator &operator+=(dint j) { i+=j; return *this; }
        inline iterator &operator-=(dint j) { i-=j; return *this; }
        inline iterator operator+(dint j) const { return iterator(i+j); }
        inline iterator operator-(dint j) const { return iterator(i-j); }
        inline dint operator-(iterator j) const { return dint(i - j.i); }
    };
    friend class iterator;

    class const_iterator {
    public:
        Node *i;
        typedef T value_type;
        typedef const T *pointer;
        typedef const T &reference;

        inline const_iterator() : i(0) {}
        inline const_iterator(Node *n) : i(n) {}
        inline const_iterator(const const_iterator &o): i(o.i) {}

        inline const_iterator(const iterator &o): i(o.i) {}

        inline const T &operator*() const { return i->t(); }
        inline const T *operator->() const { return &i->t(); }
        inline const T &operator[](dint j) const { return i[j].t(); }
        inline dbool operator==(const const_iterator &o) const { return i == o.i; }
        inline dbool operator!=(const const_iterator &o) const { return i != o.i; }
        inline dbool operator<(const const_iterator& other) const { return i < other.i; }
        inline dbool operator<=(const const_iterator& other) const { return i <= other.i; }
        inline dbool operator>(const const_iterator& other) const { return i > other.i; }
        inline dbool operator>=(const const_iterator& other) const { return i >= other.i; }
        inline const_iterator &operator++() { ++i; return *this; }
        inline const_iterator operator++(dint) { Node *n = i; ++i; return n; }
        inline const_iterator &operator--() { i--; return *this; }
        inline const_iterator operator--(dint) { Node *n = i; i--; return n; }
        inline const_iterator &operator+=(dint j) { i+=j; return *this; }
        inline const_iterator &operator-=(dint j) { i-=j; return *this; }
        inline const_iterator operator+(dint j) const { return const_iterator(i+j); }
        inline const_iterator operator-(dint j) const { return const_iterator(i-j); }
        inline dint operator-(const_iterator j) const { return i - j.i; }
    };
    friend class const_iterator;

    // stl style
    inline iterator begin() { detach(); return reinterpret_cast<Node *>(p.begin()); }
    inline const_iterator begin() const { return reinterpret_cast<Node *>(p.begin()); }
    inline const_iterator constBegin() const { return reinterpret_cast<Node *>(p.begin()); }
    inline iterator end() { detach(); return reinterpret_cast<Node *>(p.end()); }
    inline const_iterator end() const { return reinterpret_cast<Node *>(p.end()); }
    inline const_iterator constEnd() const { return reinterpret_cast<Node *>(p.end()); }
    iterator insert(iterator before, const T &t);
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);

    typedef iterator Iterator;
    typedef const_iterator ConstIterator;
    inline dint count() const { return p.size(); }
    inline dint length() const { return p.size(); } // Same as count()
    inline T& first() { DM_ASSERT(!isEmpty()); return *begin(); }
    inline const T& first() const { DM_ASSERT(!isEmpty()); return at(0); }
    T& last() { DM_ASSERT(!isEmpty()); return *(--end()); }
    const T& last() const { DM_ASSERT(!isEmpty()); return at(count() - 1); }
    inline void removeFirst() { DM_ASSERT(!isEmpty()); erase(begin()); }
    inline void removeLast() { DM_ASSERT(!isEmpty()); erase(--end()); }
    inline dbool startsWith(const T &t) const { return !isEmpty() && first() == t; }
    inline dbool endsWith(const T &t) const { return !isEmpty() && last() == t; }
    Vector<T> mid(dint pos, dint length = -1) const;

    T value(dint i) const;
    T value(dint i, const T &defaultValue) const;

    // stl compatibility
    inline void push_back(const T &t) { append(t); }
    inline void push_front(const T &t) { prepend(t); }
    inline T& front() { return first(); }
    inline const T& front() const { return first(); }
    inline T& back() { return last(); }
    inline const T& back() const { return last(); }
    inline void pop_front() { removeFirst(); }
    inline void pop_back() { removeLast(); }
    inline dbool empty() const { return isEmpty(); }
    typedef dint size_type;
    typedef T value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;

    // comfort
    Vector<T> &operator+=(const Vector<T> &l);
    inline Vector<T> operator+(const Vector<T> &l) const
    { Vector n = *this; n += l; return n; }
    inline Vector<T> &operator+=(const T &t)
    { append(t); return *this; }
    inline Vector<T> &operator<< (const T &t)
    { append(t); return *this; }
    inline Vector<T> &operator<<(const Vector<T> &l)
    { *this += l; return *this; }

#ifdef DM_STL
    static inline Vector<T> fromStdList(const std::list<T> &list)
    { Vector<T> tmp; Copy(list.begin(), list.end(), std::back_inserter(tmp)); return tmp; }
    inline std::list<T> toStdList() const
    { std::list<T> tmp; Copy(constBegin(), constEnd(), std::back_inserter(tmp)); return tmp; }
#endif

private:
    Node *detach_helper_grow(dint i, dint n);
    void detach_helper(dint alloc);
    void detach_helper();
    void free(VectorData::Data *d);

    void node_construct(Node *n, const T &t);
    void node_destruct(Node *n);
    void node_copy(Node *from, Node *to, Node *src);
    void node_destruct(Node *from, Node *to);
};

template <typename T>
inline Vector<T>::Vector(dint size) 
{ 
    p.init(size); 
}
template <typename T>
inline Vector<T>::Vector(dint size, const T& t) 
{ 
    p.init(size); 
    while (size--)
    {
        append(t);
    }
}

template <typename T>
inline void Vector<T>::node_construct(Node *n, const T &t)
{
    if (TypeInfo<T>::isLarge || TypeInfo<T>::isStatic) n->v = new T(t);
    else if (TypeInfo<T>::isComplex) new (n) T(t);
#if (defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__IBMCPP__)) && !defined(__OPTIMIZE__)
    // This violates pointer aliasing rules, but it is known to be safe (and silent)
    // in unoptimized GCC builds (-fno-strict-aliasing). The other compilers which
    // set the same define are assumed to be safe.
    else *reinterpret_cast<T*>(n) = t;
#else
    // This is always safe, but penaltizes unoptimized builds a lot.
    else ::memcpy(n, &t, sizeof(T));
#endif
}

template <typename T>
inline void Vector<T>::node_destruct(Node *n)
{
    if (TypeInfo<T>::isLarge || TypeInfo<T>::isStatic) delete reinterpret_cast<T*>(n->v);
    else if (TypeInfo<T>::isComplex) reinterpret_cast<T*>(n)->~T();
}

template <typename T>
inline void Vector<T>::node_copy(Node *from, Node *to, Node *src)
{
    Node *current = from;
    if (TypeInfo<T>::isLarge || TypeInfo<T>::isStatic) {
        DM_TRY {
            while(current != to) {
                current->v = new T(*reinterpret_cast<T*>(src->v));
                ++current;
                ++src;
            }
        } DM_CATCH(...) {
            while (current-- != from)
                delete reinterpret_cast<T*>(current->v);
            DM_RETHROW;
        }

    } else if (TypeInfo<T>::isComplex) {
        DM_TRY {
            while(current != to) {
                new (current) T(*reinterpret_cast<T*>(src));
                ++current;
                ++src;
            }
        } DM_CATCH(...) {
            while (current-- != from)
                (reinterpret_cast<T*>(current))->~T();
            DM_RETHROW;
        }
    } else {
        if (src != from && to - from > 0)
            memcpy(from, src, (to - from) * sizeof(Node *));
    }
}

template <typename T>
inline void Vector<T>::node_destruct(Node *from, Node *to)
{
    if (TypeInfo<T>::isLarge || TypeInfo<T>::isStatic)
        while(from != to) --to, delete reinterpret_cast<T*>(to->v);
    else if (TypeInfo<T>::isComplex)
        while (from != to) --to, reinterpret_cast<T*>(to)->~T();
}

template <typename T>
inline Vector<T> &Vector<T>::operator=(const Vector<T> &l)
{
    if (d != l.d) {
        VectorData::Data *o = l.d;
        o->ref.ref();
        if (!d->ref.deref())
            free(d);
        d = o;
        if (!d->sharable)
            detach_helper();
    }
    return *this;
}
template <typename T>
inline typename Vector<T>::iterator Vector<T>::insert(iterator before, const T &t)
{
    dint iBefore = dint(before.i - reinterpret_cast<Node *>(p.begin()));
    Node *n = reinterpret_cast<Node *>(p.insert(iBefore));
    DM_TRY {
        node_construct(n, t);
    } DM_CATCH(...) {
        p.remove(iBefore);
        DM_RETHROW;
    }
    return n;
}
template <typename T>
inline typename Vector<T>::iterator Vector<T>::erase(iterator it)
{ node_destruct(it.i);
 return reinterpret_cast<Node *>(p.erase(reinterpret_cast<void**>(it.i))); }
template <typename T>
inline const T &Vector<T>::at(dint i) const
{ DM_ASSERT(i >= 0 && i < p.size());
 return reinterpret_cast<Node *>(p.at(i))->t(); }
template <typename T>
inline const T &Vector<T>::operator[](dint i) const
{ DM_ASSERT(i >= 0 && i < p.size());
 return reinterpret_cast<Node *>(p.at(i))->t(); }
template <typename T>
inline T &Vector<T>::operator[](dint i)
{ DM_ASSERT(i >= 0 && i < p.size());
  detach(); return reinterpret_cast<Node *>(p.at(i))->t(); }
template <typename T>
inline void Vector<T>::removeAt(dint i)
{ if(i >= 0 && i < p.size()) { detach();
 node_destruct(reinterpret_cast<Node *>(p.at(i))); p.remove(i); } }
template <typename T>
inline T Vector<T>::takeAt(dint i)
{ DM_ASSERT(i >= 0 && i < p.size());
 detach(); Node *n = reinterpret_cast<Node *>(p.at(i)); T t = n->t(); node_destruct(n);
 p.remove(i); return t; }
template <typename T>
inline T Vector<T>::takeFirst()
{ T t = first(); removeFirst(); return t; }
template <typename T>
inline T Vector<T>::takeLast()
{ T t = last(); removeLast(); return t; }

template <typename T>
 void Vector<T>::reserve(dint alloc)
{
    if (d->alloc < alloc) {
        if (d->ref != 1)
            detach_helper(alloc);
        else
            p.realloc(alloc);
    }
}

template <typename T>
 void Vector<T>::append(const T &t)
{
    if (d->ref != 1) {
        Node *n = detach_helper_grow(INT_MAX, 1);
        DM_TRY {
            node_construct(n, t);
        } DM_CATCH(...) {
            --d->end;
            DM_RETHROW;
        }
    } else {
        if (TypeInfo<T>::isLarge || TypeInfo<T>::isStatic) {
            Node *n = reinterpret_cast<Node *>(p.append());
            DM_TRY {
                node_construct(n, t);
            } DM_CATCH(...) {
                --d->end;
                DM_RETHROW;
            }
        } else {
            Node *n, copy;
            node_construct(&copy, t); // t might be a reference to an object in the array
            DM_TRY {
                n = reinterpret_cast<Node *>(p.append());;
            } DM_CATCH(...) {
                node_destruct(&copy);
                DM_RETHROW;
            }
            *n = copy;
        }
    }
}

template <typename T>
inline void Vector<T>::prepend(const T &t)
{
    if (d->ref != 1) {
        Node *n = detach_helper_grow(0, 1);
        DM_TRY {
            node_construct(n, t);
        } DM_CATCH(...) {
            ++d->begin;
            DM_RETHROW;
        }
    } else {
        if (TypeInfo<T>::isLarge || TypeInfo<T>::isStatic) {
            Node *n = reinterpret_cast<Node *>(p.prepend());
            DM_TRY {
                node_construct(n, t);
            } DM_CATCH(...) {
                ++d->begin;
                DM_RETHROW;
            }
        } else {
            Node *n, copy;
            node_construct(&copy, t); // t might be a reference to an object in the array
            DM_TRY {
                n = reinterpret_cast<Node *>(p.prepend());;
            } DM_CATCH(...) {
                node_destruct(&copy);
                DM_RETHROW;
            }
            *n = copy;
        }
    }
}

template <typename T>
inline void Vector<T>::insert(dint i, const T &t)
{
    if (d->ref != 1) {
        Node *n = detach_helper_grow(i, 1);
        DM_TRY {
            node_construct(n, t);
        } DM_CATCH(...) {
            p.remove(i);
            DM_RETHROW;
        }
    } else {
        if (TypeInfo<T>::isLarge || TypeInfo<T>::isStatic) {
            Node *n = reinterpret_cast<Node *>(p.insert(i));
            DM_TRY {
                node_construct(n, t);
            } DM_CATCH(...) {
                p.remove(i);
                DM_RETHROW;
            }
        } else {
            Node *n, copy;
            node_construct(&copy, t); // t might be a reference to an object in the array
            DM_TRY {
                n = reinterpret_cast<Node *>(p.insert(i));;
            } DM_CATCH(...) {
                node_destruct(&copy);
                DM_RETHROW;
            }
            *n = copy;
        }
    }
}

template <typename T>
inline void Vector<T>::replace(dint i, const T &t)
{
    DM_ASSERT(i >= 0 && i < p.size());
    detach();
    reinterpret_cast<Node *>(p.at(i))->t() = t;
}

template <typename T>
inline void Vector<T>::swap(dint i, dint j)
{
    DM_ASSERT(i >= 0 && i < p.size() && j >= 0 && j < p.size());
    detach();
    void *t = d->array[d->begin + i];
    d->array[d->begin + i] = d->array[d->begin + j];
    d->array[d->begin + j] = t;
}

template <typename T>
inline void Vector<T>::move(dint from, dint to)
{
    DM_ASSERT(from >= 0 && from < p.size() && to >= 0 && to < p.size());
    detach();
    p.move(from, to);
}

template<typename T>
 Vector<T> Vector<T>::mid(dint pos, dint alength) const
{
    if (alength < 0 || pos + alength > size())
        alength = size() - pos;
    if (pos == 0 && alength == size())
        return *this;
    Vector<T> cpy;
    if (alength <= 0)
        return cpy;
    cpy.reserve(alength);
    cpy.d->end = alength;
    DM_TRY {
        cpy.node_copy(reinterpret_cast<Node *>(cpy.p.begin()),
                      reinterpret_cast<Node *>(cpy.p.end()),
                      reinterpret_cast<Node *>(p.begin() + pos));
    } DM_CATCH(...) {
        // restore the old end
        cpy.d->end = 0;
        DM_RETHROW;
    }
    return cpy;
}

template<typename T>
 T Vector<T>::value(dint i) const
{
    if (i < 0 || i >= p.size()) {
        return T();
    }
    return reinterpret_cast<Node *>(p.at(i))->t();
}

template<typename T>
 T Vector<T>::value(dint i, const T& defaultValue) const
{
    return ((i < 0 || i >= p.size()) ? defaultValue : reinterpret_cast<Node *>(p.at(i))->t());
}

template <typename T>
 typename Vector<T>::Node *Vector<T>::detach_helper_grow(dint i, dint c)
{
    Node *n = reinterpret_cast<Node *>(p.begin());
    VectorData::Data *x = p.detach_grow(&i, c);
    DM_TRY {
        node_copy(reinterpret_cast<Node *>(p.begin()),
                  reinterpret_cast<Node *>(p.begin() + i), n);
    } DM_CATCH(...) {
        Free(d);
        d = x;
        DM_RETHROW;
    }
    DM_TRY {
        node_copy(reinterpret_cast<Node *>(p.begin() + i + c),
                  reinterpret_cast<Node *>(p.end()), n + i);
    } DM_CATCH(...) {
        node_destruct(reinterpret_cast<Node *>(p.begin()),
                      reinterpret_cast<Node *>(p.begin() + i));
        Free(d);
        d = x;
        DM_RETHROW;
    }

    if (!x->ref.deref())
        free(x);

    return reinterpret_cast<Node *>(p.begin() + i);
}

template <typename T>
 void Vector<T>::detach_helper(dint alloc)
{
    Node *n = reinterpret_cast<Node *>(p.begin());
    VectorData::Data *x = p.detach(alloc);
    DM_TRY {
        node_copy(reinterpret_cast<Node *>(p.begin()), reinterpret_cast<Node *>(p.end()), n);
    } DM_CATCH(...) {
        Free(d);
        d = x;
        DM_RETHROW;
    }

    if (!x->ref.deref())
        free(x);
}

template <typename T>
 void Vector<T>::detach_helper()
{
    detach_helper(d->alloc);
}

template <typename T>
 Vector<T>::~Vector()
{
    if (!d->ref.deref())
        free(d);
}

template <typename T>
 dbool Vector<T>::operator==(const Vector<T> &l) const
{
    if (p.size() != l.p.size())
        return false;
    if (d == l.d)
        return true;
    Node *i = reinterpret_cast<Node *>(p.end());
    Node *b = reinterpret_cast<Node *>(p.begin());
    Node *li = reinterpret_cast<Node *>(l.p.end());
    while (i != b) {
        --i; --li;
        if (!(i->t() == li->t()))
            return false;
    }
    return true;
}

template <typename T>
 void Vector<T>::free(VectorData::Data *data)
{
    node_destruct(reinterpret_cast<Node *>(data->array + data->begin),
                  reinterpret_cast<Node *>(data->array + data->end));
    Free(data);
}


template <typename T>
 void Vector<T>::clear()
{
    *this = Vector<T>();
}

template <typename T>
 dint Vector<T>::removeAll(const T &_t)
{
    dint index = indexOf(_t);
    if (index == -1)
        return 0;

    const T t = _t;
    detach();

    Node *i = reinterpret_cast<Node *>(p.at(index));
    Node *e = reinterpret_cast<Node *>(p.end());
    Node *n = i;
    node_destruct(i);
    while (++i != e) {
        if (i->t() == t)
            node_destruct(i);
        else
            *n++ = *i;
    }

    dint removedCount = e - n;
    d->end -= removedCount;
    return removedCount;
}

template <typename T>
 dbool Vector<T>::removeOne(const T &_t)
{
    dint index = indexOf(_t);
    if (index != -1) {
        removeAt(index);
        return true;
    }
    return false;
}

template <typename T>
 typename Vector<T>::iterator Vector<T>::erase(typename Vector<T>::iterator afirst,
                                                                 typename Vector<T>::iterator alast)
{
    for (Node *n = afirst.i; n < alast.i; ++n)
        node_destruct(n);
    dint idx = afirst - begin();
    p.remove(idx, alast - afirst);
    return begin() + idx;
}

template <typename T>
 Vector<T> &Vector<T>::operator+=(const Vector<T> &l)
{
    if (!l.isEmpty()) {
        if (isEmpty()) {
            *this = l;
        } else {
            Node *n = (d->ref != 1)
                      ? detach_helper_grow(INT_MAX, l.size())
                      : reinterpret_cast<Node *>(p.append2(l.p));
            DM_TRY {
                node_copy(n, reinterpret_cast<Node *>(p.end()),
                          reinterpret_cast<Node *>(l.p.begin()));
            } DM_CATCH(...) {
                // restore the old end
                d->end -= dint(reinterpret_cast<Node *>(p.end()) - n);
                DM_RETHROW;
            }
        }
    }
    return *this;
}

template <typename T>
inline void Vector<T>::append(const Vector<T> &t)
{
    *this += t;
}

template <typename T>
 dint Vector<T>::indexOf(const T &t, dint from) const
{
    if (from < 0)
        from = Max(from + p.size(), 0);
    if (from < p.size()) {
        Node *n = reinterpret_cast<Node *>(p.at(from -1));
        Node *e = reinterpret_cast<Node *>(p.end());
        while (++n != e)
            if (n->t() == t)
                return dint(n - reinterpret_cast<Node *>(p.begin()));
    }
    return -1;
}

template <typename T>
 dint Vector<T>::lastIndexOf(const T &t, dint from) const
{
    if (from < 0)
        from += p.size();
    else if (from >= p.size())
        from = p.size()-1;
    if (from >= 0) {
        Node *b = reinterpret_cast<Node *>(p.begin());
        Node *n = reinterpret_cast<Node *>(p.at(from + 1));
        while (n-- != b) {
            if (n->t() == t)
                return n - b;
        }
    }
    return -1;
}

template <typename T>
 dbool Vector<T>::contains(const T &t) const
{
    Node *b = reinterpret_cast<Node *>(p.begin());
    Node *i = reinterpret_cast<Node *>(p.end());
    while (i-- != b)
        if (i->t() == t)
            return dbool(true);
    return dbool(false);
}

template <typename T>
 dint Vector<T>::count(const T &t) const
{
    dint c = 0;
    Node *b = reinterpret_cast<Node *>(p.begin());
    Node *i = reinterpret_cast<Node *>(p.end());
    while (i-- != b)
        if (i->t() == t)
            ++c;
    return c;
}

DM_END_NAMESPACE

#endif // DMVECTOR_H
