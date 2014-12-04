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

#ifndef WINPTHREADS_H
#define WINPTHREADS_H

/* Build up a pthread-like API using underlying Windows API. Have only static
 * methods so as to not conflict with a potentially linked in pthread-win32
 * library.
 * As most functions here are used without checking return values,
 * only implement return values as necessary. */

#define WIN32_LEAN_AND_MEAN

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

static TakesCV cond_broadcast;
static TakesCV cond_init;
static TakesCV cond_signal;
static SleepCVCS cond_wait;

struct timespec {
	long tv_sec;
	long tv_nsec;
};
#endif

static unsigned __stdcall attribute_align_arg win32thread_worker(void *arg)
{
    pthread_t *h = (pthread_t *)arg;
    h->ret = h->func(h->arg);
    return 0;
}

static int pthread_create(pthread_t *thread, const void *unused_attr,
                          void *(*start_routine)(void*), void *arg)
{
    thread->func   = start_routine;
    thread->arg    = arg;
    thread->handle = (void*)_beginthreadex(NULL, 0, win32thread_worker, thread,
                                           0, NULL);
    return !thread->handle;
}

static void pthread_join(pthread_t thread, void **value_ptr)
{
    DWORD ret = WaitForSingleObject(thread.handle, INFINITE);
    if (ret != WAIT_OBJECT_0)
        return;
    if (value_ptr)
        *value_ptr = thread.ret;
    CloseHandle(thread.handle);
}

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

static void pthread_cond_init(pthread_cond_t *cond, const void *unused_attr)
{
    win32_cond_t *win32_cond = NULL;
    if (cond_init) {
        cond_init(cond);
        return;
    }

    /* non native condition variables */
    win32_cond = (win32_cond_t*)DM::Malloc(sizeof(win32_cond_t));
    if (!win32_cond)
        return;
    cond->ptr = win32_cond;
    win32_cond->semaphore = CreateSemaphore(NULL, 0, 0x7fffffff, NULL);
    if (!win32_cond->semaphore)
        return;
    win32_cond->waiters_done = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (!win32_cond->waiters_done)
        return;

    pthread_mutex_init(&win32_cond->mtx_waiter_count, NULL);
    pthread_mutex_init(&win32_cond->mtx_broadcast, NULL);
}

static void pthread_cond_destroy(pthread_cond_t *cond)
{
    win32_cond_t *win32_cond = (win32_cond_t*)cond->ptr;
    /* native condition variables do not destroy */
    if (cond_init)
        return;

    /* non native condition variables */
    CloseHandle(win32_cond->semaphore);
    CloseHandle(win32_cond->waiters_done);
    pthread_mutex_destroy(&win32_cond->mtx_waiter_count);
    pthread_mutex_destroy(&win32_cond->mtx_broadcast);
    DM::Free(&win32_cond);
    cond->ptr = NULL;
}

static void pthread_cond_broadcast(pthread_cond_t *cond)
{
    win32_cond_t *win32_cond = (win32_cond_t *)cond->ptr;
    int have_waiter;

    if (cond_broadcast) {
        cond_broadcast(cond);
        return;
    }

    /* non native condition variables */
    pthread_mutex_lock(&win32_cond->mtx_broadcast);
    pthread_mutex_lock(&win32_cond->mtx_waiter_count);
    have_waiter = 0;

    if (win32_cond->waiter_count) {
        win32_cond->is_broadcast = 1;
        have_waiter = 1;
    }

    if (have_waiter) {
        ReleaseSemaphore(win32_cond->semaphore, win32_cond->waiter_count, NULL);
        pthread_mutex_unlock(&win32_cond->mtx_waiter_count);
        WaitForSingleObject(win32_cond->waiters_done, INFINITE);
        ResetEvent(win32_cond->waiters_done);
        win32_cond->is_broadcast = 0;
    } else
        pthread_mutex_unlock(&win32_cond->mtx_waiter_count);
    pthread_mutex_unlock(&win32_cond->mtx_broadcast);
}

static int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    win32_cond_t *win32_cond = (win32_cond_t *)cond->ptr;
    int last_waiter;
    if (cond_wait) {
        cond_wait(cond, mutex, INFINITE);
        return 0;
    }

    /* non native condition variables */
    pthread_mutex_lock(&win32_cond->mtx_broadcast);
    pthread_mutex_lock(&win32_cond->mtx_waiter_count);
    win32_cond->waiter_count++;
    pthread_mutex_unlock(&win32_cond->mtx_waiter_count);
    pthread_mutex_unlock(&win32_cond->mtx_broadcast);

    // unlock the external mutex
    pthread_mutex_unlock(mutex);
    WaitForSingleObject(win32_cond->semaphore, INFINITE);

    pthread_mutex_lock(&win32_cond->mtx_waiter_count);
    win32_cond->waiter_count--;
    last_waiter = !win32_cond->waiter_count || !win32_cond->is_broadcast;
    pthread_mutex_unlock(&win32_cond->mtx_waiter_count);

    if (last_waiter)
        SetEvent(win32_cond->waiters_done);

    // lock the external mutex
    return pthread_mutex_lock(mutex);
}

static int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime)
{
    win32_cond_t *win32_cond = (win32_cond_t *)cond->ptr;
    DWORD msec = abstime->tv_sec * 1000;
    msec += abstime->tv_nsec / 1000;
    int last_waiter;
    if (cond_wait) {
        cond_wait(cond, mutex, msec);
        return 0;
    }

    /* non native condition variables */
    pthread_mutex_lock(&win32_cond->mtx_broadcast);
    pthread_mutex_lock(&win32_cond->mtx_waiter_count);
    win32_cond->waiter_count++;
    pthread_mutex_unlock(&win32_cond->mtx_waiter_count);
    pthread_mutex_unlock(&win32_cond->mtx_broadcast);

    // unlock the external mutex
    pthread_mutex_unlock(mutex);
    WaitForSingleObject(win32_cond->semaphore, msec);

    pthread_mutex_lock(&win32_cond->mtx_waiter_count);
    win32_cond->waiter_count--;
    last_waiter = !win32_cond->waiter_count || !win32_cond->is_broadcast;
    pthread_mutex_unlock(&win32_cond->mtx_waiter_count);

    if (last_waiter)
        SetEvent(win32_cond->waiters_done);

    // lock the external mutex
    return pthread_mutex_lock(mutex);
}

static void pthread_cond_signal(pthread_cond_t *cond)
{
    win32_cond_t *win32_cond = (win32_cond_t *)cond->ptr;
    int have_waiter;
    if (cond_signal) {
        cond_signal(cond);
        return;
    }

    pthread_mutex_lock(&win32_cond->mtx_broadcast);

    /* non-native condition variables */
    pthread_mutex_lock(&win32_cond->mtx_waiter_count);
    have_waiter = win32_cond->waiter_count;
    pthread_mutex_unlock(&win32_cond->mtx_waiter_count);

    if (have_waiter) {
        ReleaseSemaphore(win32_cond->semaphore, 1, NULL);
        WaitForSingleObject(win32_cond->waiters_done, INFINITE);
        ResetEvent(win32_cond->waiters_done);
    }

    pthread_mutex_unlock(&win32_cond->mtx_broadcast);
}

static void w32thread_init(void)
{
    //HANDLE kernel_dll = GetModuleHandle(TEXT("kernel32.dll"));
    HMODULE kernel_dll = GetModuleHandle(L"Kernel32");
    /* if one is available, then they should all be available */
    cond_init      =
        (TakesCV)GetProcAddress(kernel_dll, "InitializeConditionVariable");
    cond_broadcast =
        (TakesCV)GetProcAddress(kernel_dll, "WakeAllConditionVariable");
    cond_signal    =
        (TakesCV)GetProcAddress(kernel_dll, "WakeConditionVariable");
    cond_wait      =
        (SleepCVCS)GetProcAddress(kernel_dll, "SleepConditionVariableCS");
}

} //extern "C" {
#endif /* WINPTHREADS_H */
