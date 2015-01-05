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

#include "dmwinpthreads.h"

extern "C" {

static TakesCV cond_broadcast;
static TakesCV cond_init;
static TakesCV cond_signal;
static SleepCVCS cond_wait;

unsigned __stdcall attribute_align_arg win32thread_worker(void *arg)
{
    pthread_t *h = (pthread_t *)arg;
    h->ret = h->func(h->arg);
    return 0;
}

int pthread_create(pthread_t *thread, const void *unused_attr,
                          void *(*start_routine)(void*), void *arg)
{
    thread->func   = start_routine;
    thread->arg    = arg;
    thread->handle = (void*)_beginthreadex(NULL, 0, win32thread_worker, thread,
                                           0, NULL);
    return !thread->handle;
}

void pthread_join(pthread_t thread, void **value_ptr)
{
    DWORD ret = WaitForSingleObject(thread.handle, INFINITE);
    if (ret != WAIT_OBJECT_0)
        return;
    if (value_ptr)
        *value_ptr = thread.ret;
    CloseHandle(thread.handle);
}

void pthread_cond_init(pthread_cond_t *cond, const void *unused_attr)
{
    win32_cond_t *win32_cond = NULL;
    if (cond_init) {
        cond_init(cond);
        return;
    }

    /* non native condition variables */
    win32_cond = (win32_cond_t*)DM_NS::Malloc(sizeof(win32_cond_t));
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

void pthread_cond_destroy(pthread_cond_t *cond)
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
    DM_NS::Free(&win32_cond);
    cond->ptr = NULL;
}

void pthread_cond_broadcast(pthread_cond_t *cond)
{
    if (cond_broadcast) {
        cond_broadcast(cond);
        return;
    }

    win32_cond_t *win32_cond = (win32_cond_t *)cond->ptr;
    int have_waiter;

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

int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    if (cond_wait) {
        cond_wait(cond, mutex, INFINITE);
        return 0;
    }

    win32_cond_t *win32_cond = (win32_cond_t *)cond->ptr;
    int last_waiter;

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

int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime)
{
    DWORD msec = abstime->tv_sec * 1000;
    msec += abstime->tv_nsec / 1000000;
    if (cond_wait) {
        cond_wait(cond, mutex, msec);
        return 0;
    }
    win32_cond_t *win32_cond = (win32_cond_t *)cond->ptr;
    int last_waiter;
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

void pthread_cond_signal(pthread_cond_t *cond)
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

static void __w32thread_init(void)
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

void w32thread_init(void)
{
	static char g_thread_inited = 0;
	if (!g_thread_inited)
	{
		__w32thread_init();
		g_thread_inited = 1;
	}
}

} //extern "C" {
