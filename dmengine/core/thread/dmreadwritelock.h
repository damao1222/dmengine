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
     * ���캯��
     * @param share �������Ĺ���������д����ģʽ�´˲�����Ч
     */
    ReadWriteLock(dint share = DM_SHARE_READ_COUNT);

    /** 
     * ��������
     */
    ~ReadWriteLock();

    /** 
     * ����������
     */
    void lockForRead();  

    /** 
     * ����������
     */
    void unlockFoRead(); 

    /** 
     * ����д����
     */
    void lockForWrite();

    /** 
     * ����д����
     */
    void unlockForWrite();
};

class DM_DLL_EXPORT ReadLocker
{
public:
    /** 
     * ���캯�����Զ�����
     * @param l ��д��ָ��
     */
    inline ReadLocker(ReadWriteLock *l) : lock(l) { relock(); }

    /** 
     * �����������Զ�����
     */
    inline ~ReadLocker() { unlock(); }

    /** 
     * �����������ʱ���ڽ���
     */
    inline void unlock() { lock->unlockFoRead(); }

    /** 
     * ��׼�����ж�����ʱ���ڼ���
     */
    inline void relock() { lock->lockForRead(); }

    /** 
     * ��ö�д��
     * @return ��д��ָ��
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
     * ���캯�����Զ�����
     * @param l ��д��ָ��
     */
    inline WriteLocker(ReadWriteLock *l) : lock(l) { relock(); }

    /** 
     * �����������Զ�����
     */
    inline ~WriteLocker() { unlock(); }

    /** 
     * ��д�������ʱ���ڽ���
     */
    inline void unlock() { lock->unlockForWrite(); }

    /** 
     * ��׼������д����ʱ���ڼ���
     */
    inline void relock() { lock->lockForWrite(); }

    /** 
     * ��ö�д��
     * @return ��д��ָ��
     */
    inline ReadWriteLock *readWriteLock() const { return lock; }

private:
    ReadWriteLock *lock;
    DM_DISABLE_COPY(WriteLocker)
};
DM_END_NAMESPACE
#endif // DMREADWRITELOCK_H
