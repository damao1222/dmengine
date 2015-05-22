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

#ifndef DMMALLOC_H
#define DMMALLOC_H

#include "dmnamespace.h"
#include <stdlib.h>
#include <new>

DM_BEGIN_NAMESPACE

/** 
 * 分配内存
 * @param size 内存大小
 * @return 分配后的内存指针，失败返回NULL
 */
DM_DLL_EXPORT void *Malloc(size_t size);

/** 
 * 释放内存
 * @param ptr 需要释放内存的指针
 */
DM_DLL_EXPORT void Free(void *ptr);

/** 
 * 重新分配size大小的内存
 * @param ptr  原内存块的指针
 * @param size  新内存块的大小
 * @return 新分配内存块的指针
 */
DM_DLL_EXPORT void *Realloc(void *ptr, size_t size);

/** 
 * 在指定的对齐边界分配内存
 * @param size  请求的内存分配的大小
 * @param alignment  对齐值必须是2的整数次幂
 * @return 如果操作失败，则指针指向分配或是为 NULL 的内存块。指针是 alignment的倍数。
 */
DM_DLL_EXPORT void *MallocAligned(size_t size, size_t alignment);

/** 
 * 在指定的对齐边界分配内存
 * @param oldptr  原分配内存的指针
 * @param newsize  请求的新内存分配的大小
 * @param oldsize  原内存大小
 * @param alignment  对齐值必须是2的整数次幂
 * @return 如果操作失败，则指针指向分配或是为 NULL 的内存块。指针是 alignment的倍数。
 */
DM_DLL_EXPORT void *ReallocAligned(void *oldptr, size_t newsize, size_t oldsize, size_t alignment);

/** 
 * 释放边界对齐的内存
 * @param ptr  内存指针
 */
DM_DLL_EXPORT void FreeAligned(void *ptr);

/** 
 * 从原地址拷贝指定大小的内存数据到目标地址.
 * @param dest  目标内存地址.
 * @param src  源内存地址.
 * @param n  拷贝内存大小.
 * @return 目标地址.
 */
DM_DLL_EXPORT void *MemCopy(void *dest, const void *src, size_t n);

/** 
 * 将目标地址指定大小的内存设置为指定的值
 * @param dest  目标内存地址
 * @param v  需要设置的值
 * @param n  设置内存大小
 * @return 目标地址。
 */
DM_DLL_EXPORT void *MemSet(void *dest, int v, size_t n);

/** 
 * 将原地址移动指定大小的内存数据到目标地址.
 * @param dest  目标内存地址.
 * @param src  源内存地址.
 * @param n  移动内存大小.
 * @return 目标地址.
 */
DM_DLL_EXPORT void *MemMove(void *dest, const void *src, size_t n);

#ifdef DM_NO_EXCEPTIONS
#  define DM_CHECK_PTR(p) DM_ASSERT(p);
#else
#  define DM_CHECK_PTR(p) do { if (!(p)) DM_THROW(std::bad_alloc()); } while (0)
#endif

DM_END_NAMESPACE
#endif // DMMALLOC_H
