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

#ifndef DMHASH_H
#define DMHASH_H

#include "dmvector.h"
#include "dmpair.h"

DM_BEGIN_NAMESPACE
class UtilString;
class WString;
inline duint dmHash(dint8 key) { return duint(key); }
inline duint dmHash(duint8 key) { return duint(key); }
inline duint dmHash(char key) { return duint(key); }
inline duint dmHash(duint16 key) { return duint(key); }
inline duint dmHash(dint16 key) { return duint(key); }
inline duint dmHash(duint key) { return key; }
inline duint dmHash(dint32 key) { return duint(key); }
inline duint dmHash(dulong key)
{
    if (sizeof(dulong) > sizeof(duint)) {
        return duint(((key >> (8 * sizeof(duint) - 1)) ^ key) & (~0U));
    } else {
        return duint(key & (~0U));
    }
}
inline duint dmHash(dlong key) { return dmHash(dulong(key)); }
inline duint dmHash(duint64 key)
{
    if (sizeof(duint64) > sizeof(duint)) {
        return duint(((key >> (8 * sizeof(duint) - 1)) ^ key) & (~0U));
    } else {
        return duint(key & (~0U));
    }
}
inline duint dmHash(dint64 key) { return dmHash(duint64(key)); }
DM_DLL_EXPORT duint dmHash(const UtilString &key);
DM_DLL_EXPORT duint dmHash(const WString &key);

#if defined(DM_CC_MSVC)
#pragma warning( push )
#pragma warning( disable : 4311 ) // disable pointer truncation warning
#endif
template <class T> inline duint dmHash(const T *key)
{
    return dmHash(reinterpret_cast<uintptr>(key));
}
#if defined(DM_CC_MSVC)
#pragma warning( pop )
#endif

template <typename T1, typename T2> inline duint dmHash(const Pair<T1, T2> &key)
{
    duint h1 = dmHash(key.first);
    duint h2 = dmHash(key.second);
    return ((h1 << 16) | (h1 >> 16)) ^ h2;
}

struct DM_DLL_EXPORT HashData
{
    struct Node {
        Node *next;
        duint h;
    };

    Node *fakeNext;
    Node **buckets;
    BasicAtomicInt ref;
    dint size;
    dint nodeSize;
    dint16 userNumBits;
    dint16 numBits;
    dint numBuckets;
    duint sharable : 1;
    duint strictAlignment : 1;
    duint reserved : 30;

    void *allocateNode();
    void *allocateNode(dint nodeAlign);
    void freeNode(void *node);
    HashData *detach_helper(void (*node_duplicate)(Node *, void *), dint nodeSize);
    HashData *detach_helper2(void (*node_duplicate)(Node *, void *), void (*node_delete)(Node *),
                              dint nodeSize, dint nodeAlign);
    void mightGrow();
    dbool willGrow();
    void hasShrunk();
    void rehash(dint hint);
    void free_helper(void (*node_delete)(Node *));
    void destroyAndFree();
    Node *firstNode();
#ifdef DM_HASH_DEBUG
    void dump();
    void checkSanity();
#endif
    static Node *nextNode(Node *node);
    static Node *previousNode(Node *node);

    static HashData shared_null;
};

inline void HashData::mightGrow()
{
    if (size >= numBuckets)
        rehash(numBits + 1);
}

inline dbool HashData::willGrow()
{
    if (size >= numBuckets) {
        rehash(numBits + 1);
        return true;
    } else {
        return false;
    }
}

inline void HashData::hasShrunk()
{
    if (size <= (numBuckets >> 3) && numBits > userNumBits) {
        DM_TRY {
            rehash(Max(dint(numBits) - 2, dint(userNumBits)));
        } DM_CATCH(const std::bad_alloc &) {
            // ignore bad allocs - shrinking shouldn't throw. rehash is exception safe.
        }
    }
}

inline HashData::Node *HashData::firstNode()
{
    Node *e = reinterpret_cast<Node *>(this);
    Node **bucket = buckets;
    dint n = numBuckets;
    while (n--) {
        if (*bucket != e)
            return *bucket;
        ++bucket;
    }
    return e;
}

struct HashDummyValue
{
};

inline dbool operator==(const HashDummyValue & /* v1 */, const HashDummyValue & /* v2 */)
{
    return true;
}

DM_DECLARE_TYPEINFO(HashDummyValue, DM_MOVABLE_TYPE | DM_DUMMY_TYPE);

template <class Key, class T>
struct HashDummyNode
{
    HashDummyNode *next;
    duint h;
    Key key;

    inline HashDummyNode(const Key &key0) : key(key0) {}
};

template <class Key, class T>
struct HashNode
{
    HashNode *next;
    duint h;
    Key key;
    T value;

    inline HashNode(const Key &key0) : key(key0) {}
    inline HashNode(const Key &key0, const T &value0) : key(key0), value(value0) {}
    inline dbool same_key(duint h0, const Key &key0) { return h0 == h && key0 == key; }
};


#define DM_HASH_DECLARE_INT_NODES(key_type) \
    template <class T> \
    struct HashDummyNode<key_type, T> { \
        HashDummyNode *next; \
        union { duint h; key_type key; }; \
\
        inline HashDummyNode(key_type /* key0 */) {} \
    }; \
\
    template <class T> \
    struct HashNode<key_type, T> { \
        HashNode *next; \
        union { duint h; key_type key; }; \
        T value; \
\
        inline HashNode(key_type /* key0 */) {} \
        inline HashNode(key_type /* key0 */, const T &value0) : value(value0) {} \
        inline dbool same_key(duint h0, key_type) { return h0 == h; } \
    }

#if defined(DM_BYTE_ORDER) && DM_BYTE_ORDER == DM_LITTLE_ENDIAN
DM_HASH_DECLARE_INT_NODES(dint16);
DM_HASH_DECLARE_INT_NODES(duint16);
#endif
DM_HASH_DECLARE_INT_NODES(dint);
DM_HASH_DECLARE_INT_NODES(duint);
#undef DM_HASH_DECLARE_INT_NODES

template <class Key, class T>
class Hash
{
    typedef HashDummyNode<Key, T> DummyNode;
    typedef HashNode<Key, T> Node;

    union {
        HashData *d;
        HashNode<Key, T> *e;
    };

    static inline Node *concrete(HashData::Node *node) {
        return reinterpret_cast<Node *>(node);
    }

#ifdef DM_ALIGNOF
    static inline dint alignOfNode() { return Max<dint>(sizeof(void*), DM_ALIGNOF(Node)); }
    static inline dint alignOfDummyNode() { return Max<dint>(sizeof(void*), DM_ALIGNOF(DummyNode)); }
#else
    static inline dint alignOfNode() { return 0; }
    static inline dint alignOfDummyNode() { return 0; }
#endif

public:
    inline Hash() : d(&HashData::shared_null) { d->ref.ref(); }
    inline Hash(const Hash<Key, T> &other) : d(other.d) { d->ref.ref(); if (!d->sharable) detach(); }
    inline ~Hash() { if (!d->ref.deref()) freeData(d); }

    Hash<Key, T> &operator=(const Hash<Key, T> &other);
#ifdef DM_COMPILER_RVALUE_REFS
    inline Hash<Key, T> &operator=(Hash<Key, T> &&other)
    { Swap(d, other.d); return *this; }
#endif
    inline void swap(Hash<Key, T> &other) { Swap(d, other.d); }

    dbool operator==(const Hash<Key, T> &other) const;
    inline dbool operator!=(const Hash<Key, T> &other) const { return !(*this == other); }

    inline dint size() const { return d->size; }

    inline dbool isEmpty() const { return d->size == 0; }

    inline dint capacity() const { return d->numBuckets; }
    void reserve(dint size);
    inline void squeeze() { reserve(1); }

    inline void detach() { if (d->ref != 1) detach_helper(); }
    inline dbool isDetached() const { return d->ref == 1; }
    inline void setSharable(dbool sharable) { if (!sharable) detach(); d->sharable = sharable; }
    inline dbool isSharedWith(const Hash<Key, T> &other) const { return d == other.d; }

    void clear();

    dint remove(const Key &key);
    T take(const Key &key);

    dbool contains(const Key &key) const;
    const Key key(const T &value) const;
    const Key key(const T &value, const Key &defaultKey) const;
    const T value(const Key &key) const;
    const T value(const Key &key, const T &defaultValue) const;
    T &operator[](const Key &key);
    const T operator[](const Key &key) const;

    Vector<Key> uniqueKeys() const;
    Vector<Key> keys() const;
    Vector<Key> keys(const T &value) const;
    Vector<T> values() const;
    Vector<T> values(const Key &key) const;
    dint count(const Key &key) const;

    class const_iterator;

    class iterator
    {
        friend class const_iterator;
        HashData::Node *i;

    public:
        //typedef std::bidirectional_iterator_tag iterator_category;
        typedef ptrdiff difference_type;
        typedef T value_type;
        typedef T *pointer;
        typedef T &reference;

        inline operator Node *() const { return concrete(i); }
        inline iterator() : i(0) { }
        explicit inline iterator(void *node) : i(reinterpret_cast<HashData::Node *>(node)) { }

        inline const Key &key() const { return concrete(i)->key; }
        inline T &value() const { return concrete(i)->value; }
        inline T &operator*() const { return concrete(i)->value; }
        inline T *operator->() const { return &concrete(i)->value; }
        inline dbool operator==(const iterator &o) const { return i == o.i; }
        inline dbool operator!=(const iterator &o) const { return i != o.i; }

        inline iterator &operator++() {
            i = HashData::nextNode(i);
            return *this;
        }
        inline iterator operator++(dint) {
            iterator r = *this;
            i = HashData::nextNode(i);
            return r;
        }
        inline iterator &operator--() {
            i = HashData::previousNode(i);
            return *this;
        }
        inline iterator operator--(dint) {
            iterator r = *this;
            i = HashData::previousNode(i);
            return r;
        }
        inline iterator operator+(dint j) const
        { iterator r = *this; if (j > 0) while (j--) ++r; else while (j++) --r; return r; }
        inline iterator operator-(dint j) const { return operator+(-j); }
        inline iterator &operator+=(dint j) { return *this = *this + j; }
        inline iterator &operator-=(dint j) { return *this = *this - j; }

#ifdef DM_STRICT_ITERATORS
    private:
#else
    public:
#endif
        inline dbool operator==(const const_iterator &o) const
            { return i == o.i; }
        inline dbool operator!=(const const_iterator &o) const
            { return i != o.i; }

    private:
        inline operator dbool() const { return false; }
    };
    friend class iterator;

    class const_iterator
    {
        friend class iterator;
        HashData::Node *i;

    public:
        //typedef std::bidirectional_iterator_tag iterator_category;
        typedef ptrdiff difference_type;
        typedef T value_type;
        typedef const T *pointer;
        typedef const T &reference;

        inline operator Node *() const { return concrete(i); }
        inline const_iterator() : i(0) { }
        explicit inline const_iterator(void *node)
            : i(reinterpret_cast<HashData::Node *>(node)) { }
#ifdef DM_STRICT_ITERATORS
        explicit inline const_iterator(const iterator &o)
#else
        inline const_iterator(const iterator &o)
#endif
        { i = o.i; }

        inline const Key &key() const { return concrete(i)->key; }
        inline const T &value() const { return concrete(i)->value; }
        inline const T &operator*() const { return concrete(i)->value; }
        inline const T *operator->() const { return &concrete(i)->value; }
        inline dbool operator==(const const_iterator &o) const { return i == o.i; }
        inline dbool operator!=(const const_iterator &o) const { return i != o.i; }

        inline const_iterator &operator++() {
            i = HashData::nextNode(i);
            return *this;
        }
        inline const_iterator operator++(dint) {
            const_iterator r = *this;
            i = HashData::nextNode(i);
            return r;
        }
        inline const_iterator &operator--() {
            i = HashData::previousNode(i);
            return *this;
        }
        inline const_iterator operator--(dint) {
            const_iterator r = *this;
            i = HashData::previousNode(i);
            return r;
        }
        inline const_iterator operator+(dint j) const
        { const_iterator r = *this; if (j > 0) while (j--) ++r; else while (j++) --r; return r; }
        inline const_iterator operator-(dint j) const { return operator+(-j); }
        inline const_iterator &operator+=(dint j) { return *this = *this + j; }
        inline const_iterator &operator-=(dint j) { return *this = *this - j; }

#ifdef DM_STRICT_ITERATORS
    private:
        inline dbool operator==(const iterator &o) const { return operator==(const_iterator(o)); }
        inline dbool operator!=(const iterator &o) const { return operator!=(const_iterator(o)); }
#endif

    private:
        inline operator dbool() const { return false; }
    };
    friend class const_iterator;

    // STL style
    inline iterator begin() { detach(); return iterator(d->firstNode()); }
    inline const_iterator begin() const { return const_iterator(d->firstNode()); }
    inline const_iterator constBegin() const { return const_iterator(d->firstNode()); }
    inline iterator end() { detach(); return iterator(e); }
    inline const_iterator end() const { return const_iterator(e); }
    inline const_iterator constEnd() const { return const_iterator(e); }
    iterator erase(iterator it);

    typedef iterator Iterator;
    typedef const_iterator ConstIterator;
    inline dint count() const { return d->size; }
    iterator find(const Key &key);
    const_iterator find(const Key &key) const;
    const_iterator constFind(const Key &key) const;
    iterator insert(const Key &key, const T &value);
    iterator insertMulti(const Key &key, const T &value);
    Hash<Key, T> &unite(const Hash<Key, T> &other);

    // STL compatibility
    typedef T mapped_type;
    typedef Key key_type;
    typedef ptrdiff difference_type;
    typedef dint size_type;

    inline dbool empty() const { return isEmpty(); }

#ifdef DM_HASH_DEBUG
    inline void dump() const { d->dump(); }
    inline void checkSanity() const { d->checkSanity(); }
#endif

private:
    void detach_helper();
    void freeData(HashData *d);
    Node **findNode(const Key &key, duint *hp = 0) const;
    Node *createNode(duint h, const Key &key, const T &value, Node **nextNode);
    void deleteNode(Node *node);
    static void deleteNode2(HashData::Node *node);

    static void duplicateNode(HashData::Node *originalNode, void *newNode);
};


template <class Key, class T>
inline void Hash<Key, T>::deleteNode(Node *node)
{
    deleteNode2(reinterpret_cast<HashData::Node*>(node));
    d->freeNode(node);
}

template <class Key, class T>
inline void Hash<Key, T>::deleteNode2(HashData::Node *node)
{
#ifdef DM_CC_BOR
    concrete(node)->~HashNode<Key, T>();
#else
    concrete(node)->~Node();
#endif
}

template <class Key, class T>
inline void Hash<Key, T>::duplicateNode(HashData::Node *node, void *newNode)
{
    Node *concreteNode = concrete(node);
    if (TypeInfo<T>::isDummy) {
        (void) new (newNode) DummyNode(concreteNode->key);
    } else {
        (void) new (newNode) Node(concreteNode->key, concreteNode->value);
    }
}

template <class Key, class T>
inline typename Hash<Key, T>::Node *
Hash<Key, T>::createNode(duint ah, const Key &akey, const T &avalue, Node **anextNode)
{
    Node *node;

    if (TypeInfo<T>::isDummy) {
        node = reinterpret_cast<Node *>(new (d->allocateNode(alignOfDummyNode())) DummyNode(akey));
    } else {
        node = new (d->allocateNode(alignOfNode())) Node(akey, avalue);
    }

    node->h = ah;
    node->next = *anextNode;
    *anextNode = node;
    ++d->size;
    return node;
}

template <class Key, class T>
inline Hash<Key, T> &Hash<Key, T>::unite(const Hash<Key, T> &other)
{
    Hash<Key, T> copy(other);
    const_iterator it = copy.constEnd();
    while (it != copy.constBegin()) {
        --it;
        insertMulti(it.key(), it.value());
    }
    return *this;
}

template <class Key, class T>
inline void Hash<Key, T>::freeData(HashData *x)
{
    x->free_helper(deleteNode2);
}

template <class Key, class T>
inline void Hash<Key, T>::clear()
{
    *this = Hash<Key,T>();
}

template <class Key, class T>
inline void Hash<Key, T>::detach_helper()
{
    HashData *x = d->detach_helper2(duplicateNode, deleteNode2,
        TypeInfo<T>::isDummy ? sizeof(DummyNode) : sizeof(Node),
        TypeInfo<T>::isDummy ? alignOfDummyNode() : alignOfNode());
    if (!d->ref.deref())
        freeData(d);
    d = x;
}

template <class Key, class T>
inline Hash<Key, T> &Hash<Key, T>::operator=(const Hash<Key, T> &other)
{
    if (d != other.d) {
        HashData *o = other.d;
        o->ref.ref();
        if (!d->ref.deref())
            freeData(d);
        d = o;
        if (!d->sharable)
            detach_helper();
    }
    return *this;
}

template <class Key, class T>
inline const T Hash<Key, T>::value(const Key &akey) const
{
    Node *node;
    if (d->size == 0 || (node = *findNode(akey)) == e) {
        return T();
    } else {
        return node->value;
    }
}

template <class Key, class T>
inline const T Hash<Key, T>::value(const Key &akey, const T &adefaultValue) const
{
    Node *node;
    if (d->size == 0 || (node = *findNode(akey)) == e) {
        return adefaultValue;
    } else {
        return node->value;
    }
}

template <class Key, class T>
inline Vector<Key> Hash<Key, T>::uniqueKeys() const
{
    Vector<Key> res;
    res.reserve(size()); // May be too much, but assume short lifetime
    const_iterator i = begin();
    if (i != end()) {
        for (;;) {
            const Key &aKey = i.key();
            res.append(aKey);
            do {
                if (++i == end())
                    goto break_out_of_outer_loop;
            } while (aKey == i.key());
        }
    }
break_out_of_outer_loop:
    return res;
}

template <class Key, class T>
inline Vector<Key> Hash<Key, T>::keys() const
{
    Vector<Key> res;
    res.reserve(size());
    const_iterator i = begin();
    while (i != end()) {
        res.append(i.key());
        ++i;
    }
    return res;
}

template <class Key, class T>
inline Vector<Key> Hash<Key, T>::keys(const T &avalue) const
{
    Vector<Key> res;
    const_iterator i = begin();
    while (i != end()) {
        if (i.value() == avalue)
            res.append(i.key());
        ++i;
    }
    return res;
}

template <class Key, class T>
inline const Key Hash<Key, T>::key(const T &avalue) const
{
    return key(avalue, Key());
}

template <class Key, class T>
inline const Key Hash<Key, T>::key(const T &avalue, const Key &defaultValue) const
{
    const_iterator i = begin();
    while (i != end()) {
        if (i.value() == avalue)
            return i.key();
        ++i;
    }

    return defaultValue;
}

template <class Key, class T>
inline Vector<T> Hash<Key, T>::values() const
{
    Vector<T> res;
    res.reserve(size());
    const_iterator i = begin();
    while (i != end()) {
        res.append(i.value());
        ++i;
    }
    return res;
}

template <class Key, class T>
inline Vector<T> Hash<Key, T>::values(const Key &akey) const
{
    Vector<T> res;
    Node *node = *findNode(akey);
    if (node != e) {
        do {
            res.append(node->value);
        } while ((node = node->next) != e && node->key == akey);
    }
    return res;
}

template <class Key, class T>
inline dint Hash<Key, T>::count(const Key &akey) const
{
    dint cnt = 0;
    Node *node = *findNode(akey);
    if (node != e) {
        do {
            ++cnt;
        } while ((node = node->next) != e && node->key == akey);
    }
    return cnt;
}

template <class Key, class T>
inline const T Hash<Key, T>::operator[](const Key &akey) const
{
    return value(akey);
}

template <class Key, class T>
inline T &Hash<Key, T>::operator[](const Key &akey)
{
    detach();

    duint h;
    Node **node = findNode(akey, &h);
    if (*node == e) {
        if (d->willGrow())
            node = findNode(akey, &h);
        return createNode(h, akey, T(), node)->value;
    }
    return (*node)->value;
}

template <class Key, class T>
inline typename Hash<Key, T>::iterator Hash<Key, T>::insert(const Key &akey,
                                                                         const T &avalue)
{
    detach();

    duint h;
    Node **node = findNode(akey, &h);
    if (*node == e) {
        if (d->willGrow())
            node = findNode(akey, &h);
        return iterator(createNode(h, akey, avalue, node));
    }

    if (!TypeInfo<T>::isDummy)
        (*node)->value = avalue;
    return iterator(*node);
}

template <class Key, class T>
inline typename Hash<Key, T>::iterator Hash<Key, T>::insertMulti(const Key &akey,
                                                                              const T &avalue)
{
    detach();
    d->willGrow();

    duint h;
    Node **nextNode = findNode(akey, &h);
    return iterator(createNode(h, akey, avalue, nextNode));
}

template <class Key, class T>
inline dint Hash<Key, T>::remove(const Key &akey)
{
    if (isEmpty()) // prevents detaching shared null
        return 0;
    detach();

    dint oldSize = d->size;
    Node **node = findNode(akey);
    if (*node != e) {
        dbool deleteNext = true;
        do {
            Node *next = (*node)->next;
            deleteNext = (next != e && next->key == (*node)->key);
            deleteNode(*node);
            *node = next;
            --d->size;
        } while (deleteNext);
        d->hasShrunk();
    }
    return oldSize - d->size;
}

template <class Key, class T>
inline T Hash<Key, T>::take(const Key &akey)
{
    if (isEmpty()) // prevents detaching shared null
        return T();
    detach();

    Node **node = findNode(akey);
    if (*node != e) {
        T t = (*node)->value;
        Node *next = (*node)->next;
        deleteNode(*node);
        *node = next;
        --d->size;
        d->hasShrunk();
        return t;
    }
    return T();
}

template <class Key, class T>
inline typename Hash<Key, T>::iterator Hash<Key, T>::erase(iterator it)
{
    if (it == iterator(e))
        return it;

    iterator ret = it;
    ++ret;

    Node *node = it;
    Node **node_ptr = reinterpret_cast<Node **>(&d->buckets[node->h % d->numBuckets]);
    while (*node_ptr != node)
        node_ptr = &(*node_ptr)->next;
    *node_ptr = node->next;
    deleteNode(node);
    --d->size;
    return ret;
}

template <class Key, class T>
inline void Hash<Key, T>::reserve(dint asize)
{
    detach();
    d->rehash(-Max(asize, 1));
}

template <class Key, class T>
inline typename Hash<Key, T>::const_iterator Hash<Key, T>::find(const Key &akey) const
{
    return const_iterator(*findNode(akey));
}

template <class Key, class T>
inline typename Hash<Key, T>::const_iterator Hash<Key, T>::constFind(const Key &akey) const
{
    return const_iterator(*findNode(akey));
}

template <class Key, class T>
inline typename Hash<Key, T>::iterator Hash<Key, T>::find(const Key &akey)
{
    detach();
    return iterator(*findNode(akey));
}

template <class Key, class T>
inline dbool Hash<Key, T>::contains(const Key &akey) const
{
    return *findNode(akey) != e;
}

template <class Key, class T>
inline typename Hash<Key, T>::Node **Hash<Key, T>::findNode(const Key &akey, duint *ahp) const
{
    Node **node;
    duint h = dmHash(akey);

    if (d->numBuckets) {
        node = reinterpret_cast<Node **>(&d->buckets[h % d->numBuckets]);
        DM_ASSERT(*node == e || (*node)->next);
        while (*node != e && !(*node)->same_key(h, akey))
            node = &(*node)->next;
    } else {
        node = const_cast<Node **>(reinterpret_cast<const Node * const *>(&e));
    }
    if (ahp)
        *ahp = h;
    return node;
}

template <class Key, class T>
inline dbool Hash<Key, T>::operator==(const Hash<Key, T> &other) const
{
    if (size() != other.size())
        return false;
    if (d == other.d)
        return true;

    const_iterator it = begin();

    while (it != end()) {
        const Key &akey = it.key();

        const_iterator it2 = other.find(akey);
        do {
            if (it2 == other.end() || !(it2.key() == akey))
                return false;
            if (!TypeInfo<T>::isDummy && !(it.value() == it2.value()))
                return false;
            ++it;
            ++it2;
        } while (it != end() && it.key() == akey);
    }
    return true;
}

template <class Key, class T>
class MultiHash : public Hash<Key, T>
{
public:
    MultiHash() {}
    MultiHash(const Hash<Key, T> &other) : Hash<Key, T>(other) {}
    inline void swap(MultiHash<Key, T> &other) { Hash<Key, T>::swap(other); } // prevent MultiHash<->Hash swaps

    inline typename Hash<Key, T>::iterator replace(const Key &key, const T &value)
    { return Hash<Key, T>::insert(key, value); }

    inline typename Hash<Key, T>::iterator insert(const Key &key, const T &value)
    { return Hash<Key, T>::insertMulti(key, value); }

    inline MultiHash &operator+=(const MultiHash &other)
    { this->unite(other); return *this; }
    inline MultiHash operator+(const MultiHash &other) const
    { MultiHash result = *this; result += other; return result; }

#if !defined(DM_NO_USING_KEYWORD) && !defined(DM_CC_RVCT)
    // RVCT compiler doesn't handle using-keyword right when used functions are overloaded in child class
    using Hash<Key, T>::contains;
    using Hash<Key, T>::remove;
    using Hash<Key, T>::count;
    using Hash<Key, T>::find;
    using Hash<Key, T>::constFind;
#else
    inline dbool contains(const Key &key) const
    { return Hash<Key, T>::contains(key); }
    inline dint remove(const Key &key)
    { return Hash<Key, T>::remove(key); }
    inline dint count(const Key &key) const
    { return Hash<Key, T>::count(key); }
    inline dint count() const
    { return Hash<Key, T>::count(); }
    inline typename Hash<Key, T>::iterator find(const Key &key)
    { return Hash<Key, T>::find(key); }
    inline typename Hash<Key, T>::const_iterator find(const Key &key) const
    { return Hash<Key, T>::find(key); }
    inline typename Hash<Key, T>::const_iterator constFind(const Key &key) const
    { return Hash<Key, T>::constFind(key); }
#endif

    dbool contains(const Key &key, const T &value) const;

    dint remove(const Key &key, const T &value);

    dint count(const Key &key, const T &value) const;

    typename Hash<Key, T>::iterator find(const Key &key, const T &value) {
        typename Hash<Key, T>::iterator i(find(key));
        typename Hash<Key, T>::iterator end(this->end());
        while (i != end && i.key() == key) {
            if (i.value() == value)
                return i;
            ++i;
        }
        return end;
    }
    typename Hash<Key, T>::const_iterator find(const Key &key, const T &value) const {
        typename Hash<Key, T>::const_iterator i(constFind(key));
        typename Hash<Key, T>::const_iterator end(Hash<Key, T>::constEnd());
        while (i != end && i.key() == key) {
            if (i.value() == value)
                return i;
            ++i;
        }
        return end;
    }
    typename Hash<Key, T>::const_iterator constFind(const Key &key, const T &value) const
        { return find(key, value); }
private:
    T &operator[](const Key &key);
    const T operator[](const Key &key) const;
};

template <class Key, class T>
inline dbool MultiHash<Key, T>::contains(const Key &key, const T &value) const
{
    return constFind(key, value) != Hash<Key, T>::constEnd();
}

template <class Key, class T>
inline dint MultiHash<Key, T>::remove(const Key &key, const T &value)
{
    dint n = 0;
    typename Hash<Key, T>::iterator i(find(key));
    typename Hash<Key, T>::iterator end(Hash<Key, T>::end());
    while (i != end && i.key() == key) {
        if (i.value() == value) {
            i = this->erase(i);
            ++n;
        } else {
            ++i;
        }
    }
    return n;
}

template <class Key, class T>
inline dint MultiHash<Key, T>::count(const Key &key, const T &value) const
{
    dint n = 0;
    typename Hash<Key, T>::const_iterator i(constFind(key));
    typename Hash<Key, T>::const_iterator end(Hash<Key, T>::constEnd());
    while (i != end && i.key() == key) {
        if (i.value() == value)
            ++n;
        ++i;
    }
    return n;
}

DM_END_NAMESPACE

#endif // DMHASH_H
