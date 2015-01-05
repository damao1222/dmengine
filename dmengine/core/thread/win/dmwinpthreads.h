/*
 * Copyright (C) 2010-2014
 *
 * Authors: Xiongfa Li
 *          Steven Walters <kemuri9@gmail.com>
 *          Pegasys Inc. <http://www.pegasys-inc.com>     
 *
 * This file is part of Libav.
 *
 * Libav is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Libav is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Libav; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file
 * w32threads to pthreads wrapper
 */

#ifndef DMWINPTHREADS_H
#define DMWINPTHREADS_H

/* Build up a pthread-like API using underlying Windows API. Have only static
 * methods so as to not conflict with a potentially linked in pthread-win32
 * library.
 * As most functions here are used without checking return values,
 * only implement return values as necessary. */

#define WIN32_LEAN_AND_MEAN
#ifndef DM_OS_WP8
#include <winsock2.h>
#endif
#include <windows.h>
#include <process.h>
#include "dmmalloc.h"

extern "C" {

#ifndef attribute_align_arg
#ifdef DM_ARCH_I386
#    define ARCH_X86_32 1
#endif
#if ARCH_X86_32 && DM_GCC_VERSION_AT_LEAST(4,2)
#    define attribute_align_arg __attribute__((force_align_arg_pointer))
#else
#    define attribute_align_arg
#endif
#endif

typedef struct {
    void *handle;
    void *(*func)(void* arg);
    void *arg;
    void *ret;
} pthread_t;

/* the conditional variable api for windows 6.0+ uses critical sections and
 * not mutexes */
typedef CRITICAL_SECTION pthread_mutex_t;

/* This is the CONDITIONAL_VARIABLE typedef for using Window's native
 * conditional variables on kernels 6.0+.
 * MinGW does not currently have this typedef. */
typedef struct {
    void *ptr;
} pthread_cond_t;

/* function pointers to conditional variable API on windows 6.0+ kernels */
#ifndef __cplusplus
static void (WINAPI *cond_broadcast)(pthread_cond_t *cond);
static void (WINAPI *cond_init)(pthread_cond_t *cond);
static void (WINAPI *cond_signal)(pthread_cond_t *cond);
static BOOL (WINAPI *cond_wait)(pthread_cond_t *cond, pthread_mutex_t *mutex,
                                DWORD milliseconds);
#else
typedef VOID (WINAPI *TakesCV)(pthread_cond_t *);
typedef BOOL (WINAPI *SleepCVCS)(pthread_cond_t *, pthread_mutex_t *, DWORD);

struct timespec {
	long tv_sec;
	long tv_nsec;
};
#endif

DM_DLL_EXPORT unsigned __stdcall attribute_align_arg win32thread_worker(void *arg);

DM_DLL_EXPORT int pthread_create(pthread_t *thread, const void *unused_attr,
                          void *(*start_routine)(void*), void *arg);

DM_DLL_EXPORT void pthread_join(pthread_t thread, void **value_ptr);

static inline int pthread_mutex_init(pthread_mutex_t *m, void* attr)
{
    InitializeCriticalSection(m);
    return 0;
}
static inline int pthread_mutex_destroy(pthread_mutex_t *m)
{
    DeleteCriticalSection(m);
    return 0;
}
static inline int pthread_mutex_lock(pthread_mutex_t *m)
{
    EnterCriticalSection(m);
    return 0;
}
static inline int pthread_mutex_unlock(pthread_mutex_t *m)
{
    LeaveCriticalSection(m);
    return 0;
}

/* for pre-Windows 6.0 platforms we need to define and use our own condition
 * variable and api */
typedef struct {
    pthread_mutex_t mtx_broadcast;
    pthread_mutex_t mtx_waiter_count;
    volatile int waiter_count;
    HANDLE semaphore;
    HANDLE waiters_done;
    volatile int is_broadcast;
} win32_cond_t;

DM_DLL_EXPORT void pthread_cond_init(pthread_cond_t *cond, const void *unused_attr);

DM_DLL_EXPORT void pthread_cond_destroy(pthread_cond_t *cond);

DM_DLL_EXPORT void pthread_cond_broadcast(pthread_cond_t *cond);

DM_DLL_EXPORT int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);

DM_DLL_EXPORT int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime);

DM_DLL_EXPORT void pthread_cond_signal(pthread_cond_t *cond);

DM_DLL_EXPORT void w32thread_init(void);

} //extern "C" {
#endif /* DMWINPTHREADS_H */
