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
 * �����ڴ�
 * @param size �ڴ��С
 * @return �������ڴ�ָ�룬ʧ�ܷ���NULL
 */
DM_DLL_EXPORT void *Malloc(size_t size);

/** 
 * �ͷ��ڴ�
 * @param ptr ��Ҫ�ͷ��ڴ��ָ��
 */
DM_DLL_EXPORT void Free(void *ptr);

/** 
 * ���·���size��С���ڴ�
 * @param ptr  ԭ�ڴ���ָ��
 * @param size  ���ڴ��Ĵ�С
 * @return �·����ڴ���ָ��
 */
DM_DLL_EXPORT void *Realloc(void *ptr, size_t size);

/** 
 * ��ָ���Ķ���߽�����ڴ�
 * @param size  ������ڴ����Ĵ�С
 * @param alignment  ����ֵ������2����������
 * @return �������ʧ�ܣ���ָ��ָ��������Ϊ NULL ���ڴ�顣ָ���� alignment�ı�����
 */
DM_DLL_EXPORT void *MallocAligned(size_t size, size_t alignment);

/** 
 * ��ָ���Ķ���߽�����ڴ�
 * @param oldptr  ԭ�����ڴ��ָ��
 * @param newsize  ��������ڴ����Ĵ�С
 * @param oldsize  ԭ�ڴ��С
 * @param alignment  ����ֵ������2����������
 * @return �������ʧ�ܣ���ָ��ָ��������Ϊ NULL ���ڴ�顣ָ���� alignment�ı�����
 */
DM_DLL_EXPORT void *ReallocAligned(void *oldptr, size_t newsize, size_t oldsize, size_t alignment);

/** 
 * �ͷű߽������ڴ�
 * @param ptr  �ڴ�ָ��
 */
DM_DLL_EXPORT void FreeAligned(void *ptr);

/** 
 * ��ԭ��ַ����ָ����С���ڴ����ݵ�Ŀ���ַ.
 * @param dest  Ŀ���ڴ��ַ.
 * @param src  Դ�ڴ��ַ.
 * @param n  �����ڴ��С.
 * @return Ŀ���ַ.
 */
DM_DLL_EXPORT void *MemCopy(void *dest, const void *src, size_t n);

/** 
 * ��Ŀ���ַָ����С���ڴ�����Ϊָ����ֵ
 * @param dest  Ŀ���ڴ��ַ
 * @param v  ��Ҫ���õ�ֵ
 * @param n  �����ڴ��С
 * @return Ŀ���ַ��
 */
DM_DLL_EXPORT void *MemSet(void *dest, int v, size_t n);

/** 
 * ��ԭ��ַ�ƶ�ָ����С���ڴ����ݵ�Ŀ���ַ.
 * @param dest  Ŀ���ڴ��ַ.
 * @param src  Դ�ڴ��ַ.
 * @param n  �ƶ��ڴ��С.
 * @return Ŀ���ַ.
 */
DM_DLL_EXPORT void *MemMove(void *dest, const void *src, size_t n);

#ifdef DM_NO_EXCEPTIONS
#  define DM_CHECK_PTR(p) DM_ASSERT(p);
#else
#  define DM_CHECK_PTR(p) do { if (!(p)) DM_THROW(std::bad_alloc()); } while (0)
#endif

DM_END_NAMESPACE
#endif // DMMALLOC_H
