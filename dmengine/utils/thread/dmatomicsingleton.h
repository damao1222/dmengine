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
#ifndef DMATOMICSINGLETON_H
#define DMATOMICSINGLETON_H

#include <dmnamespace.h>
#include "thread/dmatomic.h"

DM_BEGIN_NAMESPACE
DM_BEGIN_NS_THREAD
template<class T>
class Singleton
{
public:
    static T* getInstance()
    {
        AtomicSpinLock lock(m_lock);
        if (m_instance == 0)
        {
            m_instance = new T;
        }
        return m_instance;
    }

    static T* instance()
    {
        AtomicSpinLock lock(m_lock);
        return m_instance;
    }

    static void releaseInstance()
    {
        AtomicSpinLock lock(m_lock);
        if (m_instance != 0)
        {
            delete m_instance;
            m_instance = 0;
        }
    }

    static T* m_instance;
    static dlong m_lock;
};
template<class T>
T* Singleton<T>::m_instance = 0;

template<class T>
dlong Singleton<T>::m_lock = DM_BASIC_ATOMIC_INITIALIZER(0);

DM_END_NS_THREAD
DM_END_NAMESPACE
#define DM_INSTANCE(CLASS) CLASS##::instance()
#endif // DMATOMICSINGLETON_H
