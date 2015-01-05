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

#include "dmmalloc.h"
DM_BEGIN_NAMESPACE
void *Malloc(size_t size)
{
    return ::malloc(size);
}

void Free(void *ptr)
{
    ::free(ptr);
}

void *Realloc(void *ptr, size_t size)
{
    return ::realloc(ptr, size);
}

void *MallocAligned(size_t size, size_t alignment)
{
    return ReallocAligned(0, size, 0, alignment);
}

void *ReallocAligned(void *oldptr, size_t newsize, size_t oldsize, size_t alignment)
{
    // fake an aligned allocation
    DM_UNUSED(oldsize);

    void *actualptr = oldptr ? static_cast<void **>(oldptr)[-1] : 0;
    if (alignment <= sizeof(void*)) {
        // special, fast case
        void **newptr = static_cast<void **>(Realloc(actualptr, newsize + sizeof(void*)));
        if (!newptr)
            return 0;
        if (newptr == actualptr) {
            // realloc succeeded without reallocating
            return oldptr;
        }

        *newptr = newptr;
        return newptr + 1;
    }

    // Malloc returns pointers aligned at least at sizeof(size_t) boundaries
    // but usually more (8- or 16-byte boundaries).
    // So we overallocate by alignment-sizeof(size_t) bytes, so we're guaranteed to find a
    // somewhere within the first alignment-sizeof(size_t) that is properly aligned.

    // However, we need to store the actual pointer, so we need to allocate actually size +
    // alignment anyway.

    void *real = Realloc(actualptr, newsize + alignment);
    if (!real)
        return 0;

    uintptr faked = reinterpret_cast<uintptr>(real) + alignment;
    faked &= ~(alignment - 1);

    void **faked_ptr = reinterpret_cast<void **>(faked);

    // now save the value of the real pointer at faked-sizeof(void*)
    // by construction, alignment > sizeof(void*) and is a power of 2, so
    // faked-sizeof(void*) is properly aligned for a pointer
    faked_ptr[-1] = real;

    return faked_ptr;
}

void FreeAligned(void *ptr)
{
    if (!ptr)
        return;
    void **ptr2 = static_cast<void **>(ptr);
    free(ptr2[-1]);
}

DM_END_NAMESPACE
