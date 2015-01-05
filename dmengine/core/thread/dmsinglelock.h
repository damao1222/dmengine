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

#ifndef DMSINGLELOCK_H
#define DMSINGLELOCK_H

#include "dmatomicint.h"
#include "dmmutex.h"

DM_BEGIN_NAMESPACE
class DM_DLL_EXPORT SingleLock
{
public:
    /** 
     * ���캯�����������Ļ�������Ϊ�գ������
     * @param m ������
     */
    explicit SingleLock(Mutex *m);

    /** 
     * ��������������
     */
    ~SingleLock(); 

    /** 
     * ���������û���������
     */
    void lock();

    /** 
     * ���������û�������Լ�������������̳߳���������ֱ�ӷ���false
     * @return �����������̳߳��У��򷵻�false���ɹ�����true
     */
    dbool tryLock();

    /** 
     * �ͷ���
     */
    void unlock(); 

    /** 
     * lock�����õĴ���
     * @return lock����
     */
    dint sharedLockCount() const;

    /** 
     * ��û�����
     * @return ����������
     */
    Mutex& getMutex();

protected:
    Mutex *_mutex;
    AtomicInt _lock_count;
};

inline SingleLock::SingleLock(Mutex *m):
    _mutex(m),
    _lock_count(0)
{
    lock();
}

inline SingleLock::~SingleLock()
{
    unlock();
}

inline void SingleLock::lock()
{
    if (_lock_count == 0 && _mutex)
        _mutex->lock();

    _lock_count.ref();
}

inline dbool SingleLock::tryLock()
{
    if (_lock_count == 0 && _mutex)
    {
        if (_mutex->tryLock())
        {
            _lock_count.ref();
            return true;
        }
    }
    return false;
}

inline void SingleLock::unlock()
{
    DM_ASSERT(_lock_count >= 0);
    if (_lock_count > 0 && _mutex)
    {
        _mutex->unlock();
    }
    _lock_count.deref();
}

inline dint SingleLock::sharedLockCount() const
{
    return _lock_count;
}

inline Mutex& SingleLock::getMutex()
{
    return *_mutex;
}

DM_END_NAMESPACE
#endif // DMSINGLELOCK_H