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

#ifndef DMREADWRITELOCK_H
#define DMREADWRITELOCK_H

#include "dmnamespace.h"

#define DM_SHARE_READ_COUNT 1024

DM_BEGIN_NAMESPACE
DM_PRIVATE_CLASS(ReadWriteLock);
class DM_DLL_EXPORT ReadWriteLock
{
    DM_DECLARE_PRIVATE(ReadWriteLock);
public:  
    /** 
     * 构造函数
     * @param share 读操作的共享数，在写优先模式下此参数无效
     */
    ReadWriteLock(dint share = DM_SHARE_READ_COUNT);

    /** 
     * 析构函数
     */
    ~ReadWriteLock();

    /** 
     * 锁定读操作
     */
    void lockForRead();  

    /** 
     * 解锁读操作
     */
    void unlockFoRead(); 

    /** 
     * 锁定写操作
     */
    void lockForWrite();

    /** 
     * 解锁写操作
     */
    void unlockForWrite();
};

class DM_DLL_EXPORT ReadLocker
{
public:
    /** 
     * 构造函数，自动加锁
     * @param l 读写锁指针
     */
    inline ReadLocker(ReadWriteLock *l) : lock(l) { relock(); }

    /** 
     * 析构函数，自动解锁
     */
    inline ~ReadLocker() { unlock(); }

    /** 
     * 当读操作完成时用于解锁
     */
    inline void unlock() { lock->unlockFoRead(); }

    /** 
     * 当准备进行读操作时用于加锁
     */
    inline void relock() { lock->lockForRead(); }

    /** 
     * 获得读写锁
     * @return 读写锁指针
     */
    inline ReadWriteLock *readWriteLock() const { return lock; }

private:
    ReadWriteLock *lock;
    DM_DISABLE_COPY(ReadLocker)
};

class DM_DLL_EXPORT WriteLocker
{
public:
    /** 
     * 构造函数，自动加锁
     * @param l 读写锁指针
     */
    inline WriteLocker(ReadWriteLock *l) : lock(l) { relock(); }

    /** 
     * 析构函数，自动解锁
     */
    inline ~WriteLocker() { unlock(); }

    /** 
     * 当写操作完成时用于解锁
     */
    inline void unlock() { lock->unlockForWrite(); }

    /** 
     * 当准备进行写操作时用于加锁
     */
    inline void relock() { lock->lockForWrite(); }

    /** 
     * 获得读写锁
     * @return 读写锁指针
     */
    inline ReadWriteLock *readWriteLock() const { return lock; }

private:
    ReadWriteLock *lock;
    DM_DISABLE_COPY(WriteLocker)
};
DM_END_NAMESPACE
#endif // DMREADWRITELOCK_H
