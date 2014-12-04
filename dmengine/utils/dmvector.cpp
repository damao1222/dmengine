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

#include <new>
#include "dmvector.h"
#include <string.h>

DM_BEGIN_NAMESPACE

VectorData::Data VectorData::shared_null = { DM_BASIC_ATOMIC_INITIALIZER(1), 0, 0, 0, true, { 0 } };

dint AllocMore(dint alloc, dint extra)
{
    if (alloc == 0 && extra == 0)
        return 0;
    const dint page = 1 << 12;
    dint nalloc;
    alloc += extra;
    if (alloc < 1<<6) {
        nalloc = (1<<3) + ((alloc >>3) << 3);
    } else  {
        // don't do anything if the loop will overflow signed dint.
        if (alloc >= INT_MAX/2)
            return INT_MAX;
        nalloc = (alloc < page) ? 1 << 3 : page;
        while (nalloc < alloc) {
            if (nalloc <= 0)
                return INT_MAX;
            nalloc *= 2;
        }
    }
    return nalloc - extra;
}

static dint grow(dint size)
{
    // dear compiler: don't optimize me out.
    volatile dint x = AllocMore(size * sizeof(void *), VectorData::DataHeaderSize) / sizeof(void *);
    return x;
}

void VectorData::init(dint alloc/* = 0*/)
{
    d = static_cast<Data *>(Malloc(DataHeaderSize + alloc * sizeof(void *)));
    d->begin = 0;
    d->end = 0;
    d->alloc = alloc;
    d->ref = 1;
    d->sharable = true;
}

VectorData::Data *VectorData::detach_grow(dint *idx, dint num)
{
    Data *x = d;
    dint l = x->end - x->begin;
    dint nl = l + num;
    dint alloc = grow(nl);
    Data* t = static_cast<Data *>(Malloc(DataHeaderSize + alloc * sizeof(void *)));
    DM_CHECK_PTR(t);

    t->ref = 1;
    t->sharable = true;
    t->alloc = alloc;
    // The space reservation algorithm's optimization is biased towards appending:
    // Something which looks like an append will put the data at the beginning,
    // while something which looks like a prepend will put it in the middle
    // instead of at the end. That's based on the assumption that prepending
    // is uncommon and even an initial prepend will eventually be followed by
    // at least some appends.
    dint bg;
    if (*idx < 0) {
        *idx = 0;
        bg = (alloc - nl) >> 1;
    } else if (*idx > l) {
        *idx = l;
        bg = 0;
    } else if (*idx < (l >> 1)) {
        bg = (alloc - nl) >> 1;
    } else {
        bg = 0;
    }
    t->begin = bg;
    t->end = bg + nl;
    d = t;

    return x;
}

VectorData::Data *VectorData::detach()
{
    Data *x = static_cast<Data *>(Malloc(DataHeaderSize + d->alloc * sizeof(void *)));
    DM_CHECK_PTR(x);

    x->ref = 1;
    x->sharable = true;
    x->alloc = d->alloc;
    if (!x->alloc) {
        x->begin = 0;
        x->end = 0;
    } else {
        x->begin = d->begin;
        x->end   = d->end;
    }

    Swap(d, x);
    if (!x->ref.deref())
        return x;
    return 0;
}

VectorData::Data *VectorData::detach2()
{
    Data *x = d;
    Data* t = static_cast<Data *>(Malloc(DataHeaderSize + x->alloc * sizeof(void *)));
    DM_CHECK_PTR(t);

    ::memcpy(t, d, DataHeaderSize + d->alloc * sizeof(void *));

    t->ref = 1;
    t->sharable = true;
    t->alloc = x->alloc;
    if (!t->alloc) {
        t->begin = 0;
        t->end = 0;
    } else {
        t->begin = x->begin;
        t->end   = x->end;
    }
    d = t;

    return x;
}

VectorData::Data *VectorData::detach(dint alloc)
{
    Data *x = d;
    Data* t = static_cast<Data *>(Malloc(DataHeaderSize + alloc * sizeof(void *)));
    DM_CHECK_PTR(t);

    t->ref = 1;
    t->sharable = true;
    t->alloc = alloc;
    if (!alloc) {
        t->begin = 0;
        t->end = 0;
    } else {
        t->begin = x->begin;
        t->end   = x->end;
    }
    d = t;

    return x;
}

VectorData::Data *VectorData::detach3()
{
    return detach(d->alloc);
}

void VectorData::realloc(dint alloc)
{
    DM_ASSERT(d->ref == 1);
    Data *x = static_cast<Data *>(Realloc(d, DataHeaderSize + alloc * sizeof(void *)));
    DM_CHECK_PTR(x);

    d = x;
    d->alloc = alloc;
    if (!alloc)
        d->begin = d->end = 0;
}

// ensures that enough space is available to append n elements
void **VectorData::append(dint n)
{
    DM_ASSERT(d->ref == 1);
    dint e = d->end;
    if (e + n > d->alloc) {
        dint b = d->begin;
        if (b - n >= 2 * d->alloc / 3) {
            // we have enough space. Just not at the end -> move it.
            e -= b;
            ::memmove(d->array, d->array + b, e * sizeof(void *));
            d->begin = 0;
        } else {
            realloc(grow(d->alloc + n));
        }
    }
    d->end = e + n;
    return d->array + e;
}

// ensures that enough space is available to append one element
void **VectorData::append()
{
    return append(1);
}

void **VectorData::append(const VectorData& l)
{
    DM_ASSERT(d->ref == 1);
    dint e = d->end;
    dint n = l.d->end - l.d->begin;
    if (n) {
        if (e + n > d->alloc)
            realloc(grow(e + n));
        ::memcpy(d->array + d->end, l.d->array + l.d->begin, n*sizeof(void*));
        d->end += n;
    }
    return d->array + e;
}

// ensures that enough space is available to append the list
void **VectorData::append2(const VectorData& l)
{
    return append(l.d->end - l.d->begin);
}

void **VectorData::prepend()
{
    DM_ASSERT(d->ref == 1);
    if (d->begin == 0) {
        if (d->end >= d->alloc / 3)
            realloc(grow(d->alloc + 1));

        if (d->end < d->alloc / 3)
            d->begin = d->alloc - 2 * d->end;
        else
            d->begin = d->alloc - d->end;

        ::memmove(d->array + d->begin, d->array, d->end * sizeof(void *));
        d->end += d->begin;
    }
    return d->array + --d->begin;
}

void **VectorData::insert(dint i)
{
    DM_ASSERT(d->ref == 1);
    if (i <= 0)
        return prepend();
    dint size = d->end - d->begin;
    if (i >= size)
        return append();

    dbool leftward = false;

    if (d->begin == 0) {
        if (d->end == d->alloc) {
            // If the array is full, we expand it and move some items rightward
            realloc(grow(d->alloc + 1));
        } else {
            // If there is free space at the end of the array, we move some items rightward
        }
    } else {
        if (d->end == d->alloc) {
            // If there is free space at the beginning of the array, we move some items leftward
            leftward = true;
        } else {
            // If there is free space at both ends, we move as few items as possible
            leftward = (i < size - i);
        }
    }

    if (leftward) {
        --d->begin;
        ::memmove(d->array + d->begin, d->array + d->begin + 1, i * sizeof(void *));
    } else {
        ::memmove(d->array + d->begin + i + 1, d->array + d->begin + i,
                  (size - i) * sizeof(void *));
        ++d->end;
    }
    return d->array + d->begin + i;
}

void VectorData::remove(dint i)
{
    DM_ASSERT(d->ref == 1);
    i += d->begin;
    if (i - d->begin < d->end - i) {
        if (dint offset = i - d->begin)
            ::memmove(d->array + d->begin + 1, d->array + d->begin, offset * sizeof(void *));
        d->begin++;
    } else {
        if (dint offset = d->end - i - 1)
            ::memmove(d->array + i, d->array + i + 1, offset * sizeof(void *));
        d->end--;
    }
}

void VectorData::remove(dint i, dint n)
{
    DM_ASSERT(d->ref == 1);
    i += d->begin;
    dint middle = i + n/2;
    if (middle - d->begin < d->end - middle) {
        ::memmove(d->array + d->begin + n, d->array + d->begin,
                   (i - d->begin) * sizeof(void*));
        d->begin += n;
    } else {
        ::memmove(d->array + i, d->array + i + n,
                   (d->end - i - n) * sizeof(void*));
        d->end -= n;
    }
}

void VectorData::move(dint from, dint to)
{
    DM_ASSERT(d->ref == 1);
    if (from == to)
        return;

    from += d->begin;
    to += d->begin;
    void *t = d->array[from];

    if (from < to) {
        if (d->end == d->alloc || 3 * (to - from) < 2 * (d->end - d->begin)) {
            ::memmove(d->array + from, d->array + from + 1, (to - from) * sizeof(void *));
        } else {
            // optimization
            if (dint offset = from - d->begin)
                ::memmove(d->array + d->begin + 1, d->array + d->begin, offset * sizeof(void *));
            if (dint offset = d->end - (to + 1))
                ::memmove(d->array + to + 2, d->array + to + 1, offset * sizeof(void *));
            ++d->begin;
            ++d->end;
            ++to;
        }
    } else {
        if (d->begin == 0 || 3 * (from - to) < 2 * (d->end - d->begin)) {
            ::memmove(d->array + to + 1, d->array + to, (from - to) * sizeof(void *));
        } else {
            // optimization
            if (dint offset = to - d->begin)
                ::memmove(d->array + d->begin - 1, d->array + d->begin, offset * sizeof(void *));
            if (dint offset = d->end - (from + 1))
                ::memmove(d->array + from, d->array + from + 1, offset * sizeof(void *));
            --d->begin;
            --d->end;
            --to;
        }
    }
    d->array[to] = t;
}

void **VectorData::erase(void **xi)
{
    DM_ASSERT(d->ref == 1);
    dint i = xi - (d->array + d->begin);
    remove(i);
    return d->array + d->begin + i;
}

DM_END_NAMESPACE
