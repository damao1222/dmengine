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

#include "dmhash.h"

#ifdef truncate
#undef truncate
#endif

#include "dmutilstring.h"
#include "dmlogger.h"
#include <stdlib.h>
#ifdef DM_HASH_DEBUG
#include "dmutilstring.h"
#endif

DM_BEGIN_NAMESPACE

static duint32 hash(const char *p, dint n)
{
    duint32 h = 0;

    while (n--) {
        h = (h << 4) + *p++;
        h ^= (h & 0xf0000000) >> 23;
        h &= 0x0fffffff;
    }
    return h;
}

duint32 dmHash(const UtilString &str)
{
    duint result = hash(reinterpret_cast<const char *>(str.toCharStr()), Max<dint>(0, str.length()));
    return result;
}

duint32 dmHash(const WString &key)
{
    duint h = 0;
    dint n = key.size();
    const StdChar* p = key.toCharStr();

    while (n--) {
        h = (h << 4) + (*p++);
        h ^= (h & 0xf0000000) >> 23;
        h &= 0x0fffffff;
    }
    return h;
}

/*
    The prime_deltas array is a table of selected prime values, even
    though it doesn't look like one. The primes we are using are 1,
    2, 5, 11, 17, 37, 67, 131, 257, ..., i.e. primes in the immediate
    surrounding of a power of two.

    The primeForNumBits() function returns the prime associated to a
    power of two. For example, primeForNumBits(8) returns 257.
*/

static const duint8 prime_deltas[] = {
    0,  0,  1,  3,  1,  5,  3,  3,  1,  9,  7,  5,  3,  9, 25,  3,
    1, 21,  3, 21,  7, 15,  9,  5,  3, 29, 15,  0,  0,  0,  0,  0
};

static inline dint primeForNumBits(dint numBits)
{
    return (1 << numBits) + prime_deltas[numBits];
}

/*
    Returns the smallest integer n such that
    primeForNumBits(n) >= hint.
*/
static dint countBits(dint hint)
{
    dint numBits = 0;
    dint bits = hint;

    while (bits > 1) {
        bits >>= 1;
        numBits++;
    }

    if (numBits >= (dint)sizeof(prime_deltas)) {
        numBits = sizeof(prime_deltas) - 1;
    } else if (primeForNumBits(numBits) < hint) {
        ++numBits;
    }
    return numBits;
}

/*
    A Hash has initially around pow(2, MinNumBits) buckets. For
    example, if MinNumBits is 4, it has 17 buckets.
*/
const dint MinNumBits = 4;

HashData HashData::shared_null = {
    0, 0, DM_BASIC_ATOMIC_INITIALIZER(1), 0, 0, MinNumBits, 0, 0, true, false, 0
};

void *HashData::allocateNode()
{
    return allocateNode(0);
}

void *HashData::allocateNode(dint nodeAlign)
{
    void *ptr = strictAlignment ? MallocAligned(nodeSize, nodeAlign) : Malloc(nodeSize);
    DM_CHECK_PTR(ptr);
    return ptr;
}

void HashData::freeNode(void *node)
{
    if (strictAlignment)
        FreeAligned(node);
    else
        Free(node);
}

HashData *HashData::detach_helper(void (*node_duplicate)(Node *, void *), dint nodeSize)
{
    return detach_helper2( node_duplicate, 0, nodeSize, 0 );
}

HashData *HashData::detach_helper2(void (*node_duplicate)(Node *, void *),
                                     void (*node_delete)(Node *),
                                     dint nodeSize,
                                     dint nodeAlign)
{
    union {
        HashData *d;
        Node *e;
    };
    d = new HashData;
    d->fakeNext = 0;
    d->buckets = 0;
    d->ref = 1;
    d->size = size;
    d->nodeSize = nodeSize;
    d->userNumBits = userNumBits;
    d->numBits = numBits;
    d->numBuckets = numBuckets;
    d->sharable = true;
    d->strictAlignment = nodeAlign > 8;
    d->reserved = 0;

    if (numBuckets) {
        DM_TRY {
            d->buckets = new Node *[numBuckets];
        } DM_CATCH(...) {
            // restore a consistent state for d
            d->numBuckets = 0;
            // roll back
            d->free_helper(node_delete);
            DM_RETHROW;
        }

        Node *this_e = reinterpret_cast<Node *>(this);
        for (dint i = 0; i < numBuckets; ++i) {
            Node **nextNode = &d->buckets[i];
            Node *oldNode = buckets[i];
            while (oldNode != this_e) {
                DM_TRY {
                    Node *dup = static_cast<Node *>(allocateNode(nodeAlign));

                    DM_TRY {
                        node_duplicate(oldNode, dup);
                    } DM_CATCH(...) {
                        freeNode( dup );
                        DM_RETHROW;
                    }

                    dup->h = oldNode->h;
                    *nextNode = dup;
                    nextNode = &dup->next;
                    oldNode = oldNode->next;
                } DM_CATCH(...) {
                    // restore a consistent state for d
                    *nextNode = e;
                    d->numBuckets = i+1;
                    // roll back
                    d->free_helper(node_delete);
                    DM_RETHROW;
                }
            }
            *nextNode = e;
        }
    }
    return d;
}

void HashData::free_helper(void (*node_delete)(Node *))
{
    if (node_delete) {
        Node *this_e = reinterpret_cast<Node *>(this);
        Node **bucket = reinterpret_cast<Node **>(this->buckets);

        dint n = numBuckets;
        while (n--) {
            Node *cur = *bucket++;
            while (cur != this_e) {
                Node *next = cur->next;
                node_delete(cur);
                freeNode(cur);
                cur = next;
            }
        }
    }
    delete [] buckets;
    delete this;
}

HashData::Node *HashData::nextNode(Node *node)
{
    union {
        Node *next;
        Node *e;
        HashData *d;
    };
    next = node->next;
    DM_ASSERT_X(next, "Hash: Iterating beyond end()");
    if (next->next)
        return next;

    dint start = (node->h % d->numBuckets) + 1;
    Node **bucket = d->buckets + start;
    dint n = d->numBuckets - start;
    while (n--) {
        if (*bucket != e)
            return *bucket;
        ++bucket;
    }
    return e;
}

HashData::Node *HashData::previousNode(Node *node)
{
    union {
        Node *e;
        HashData *d;
    };

    e = node;
    while (e->next)
        e = e->next;

    dint start;
    if (node == e)
        start = d->numBuckets - 1;
    else
        start = node->h % d->numBuckets;

    Node *sentinel = node;
    Node **bucket = d->buckets + start;
    while (start >= 0) {
        if (*bucket != sentinel) {
            Node *prev = *bucket;
            while (prev->next != sentinel)
                prev = prev->next;
            return prev;
        }

        sentinel = e;
        --bucket;
        --start;
    }
    DM_ASSERT_X(start >= 0, "Hash: Iterating backward beyond begin()");
    return e;
}

/*
    If hint is negative, -hint gives the approximate number of
    buckets that should be used for the hash table. If hint is
    nonnegative, (1 << hint) gives the approximate number
    of buckets that should be used.
*/
void HashData::rehash(dint hint)
{
    if (hint < 0) {
        hint = countBits(-hint);
        if (hint < MinNumBits)
            hint = MinNumBits;
        userNumBits = hint;
        while (primeForNumBits(hint) < (size >> 1))
            ++hint;
    } else if (hint < MinNumBits) {
        hint = MinNumBits;
    }

    if (numBits != hint) {
        Node *e = reinterpret_cast<Node *>(this);
        Node **oldBuckets = buckets;
        dint oldNumBuckets = numBuckets;

        dint nb = primeForNumBits(hint);
        buckets = new Node *[nb];
        numBits = hint;
        numBuckets = nb;
        for (dint i = 0; i < numBuckets; ++i)
            buckets[i] = e;

        for (dint i = 0; i < oldNumBuckets; ++i) {
            Node *firstNode = oldBuckets[i];
            while (firstNode != e) {
                duint32 h = firstNode->h;
                Node *lastNode = firstNode;
                while (lastNode->next != e && lastNode->next->h == h)
                    lastNode = lastNode->next;

                Node *afterLastNode = lastNode->next;
                Node **beforeFirstNode = &buckets[h % numBuckets];
                while (*beforeFirstNode != e)
                    beforeFirstNode = &(*beforeFirstNode)->next;
                lastNode->next = *beforeFirstNode;
                *beforeFirstNode = firstNode;
                firstNode = afterLastNode;
            }
        }
        delete [] oldBuckets;
    }
}

void HashData::destroyAndFree()
{
    free_helper(0);
}

#ifdef DM_HASH_DEBUG

void HashData::dump()
{
    DM_LOGI("Hash data (ref = %d, size = %d, nodeSize = %d, userNumBits = %d, numBits = %d, numBuckets = %d)",
            dint(ref), size, nodeSize, userNumBits, numBits,
            numBuckets);
    DM_LOGI("    %p (fakeNode = %p)", this, fakeNext);
    for (dint i = 0; i < numBuckets; ++i) {
        UtilString line;
        Node *n = buckets[i];
        if (n != reinterpret_cast<Node *>(this)) {
            line.sprintf("%d:", i);
            while (n != reinterpret_cast<Node *>(this)) {
                line += UtilString::sprintf(" -> [%p]", n);
                if (!n) {
                    line += " (CORRUPT)";
                    break;
                }
                n = n->next;
            }
            DM_LOGI(line.toCharStr());
        }
    }
}

void HashData::checkSanity()
{
    if (fakeNext)
        DM_FATAL("Fake next isn't 0");

    for (dint i = 0; i < numBuckets; ++i) {
        Node *n = buckets[i];
        Node *p = n;
        if (!n)
            DM_FATAL("%d: Bucket entry is 0", i);
        if (n != reinterpret_cast<Node *>(this)) {
            while (n != reinterpret_cast<Node *>(this)) {
                if (!n->next)
                    DM_FATAL("%d: Next of %p is 0, should be %p", i, n, this);
                n = n->next;
            }
        }
    }
}
#endif

DM_END_NAMESPACE
