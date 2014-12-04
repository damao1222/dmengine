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
#ifndef DMSINGLETON_H
#define DMSINGLETON_H

#include <dmnamespace.h>

DM_BEGIN_NAMESPACE

template<class T>
class Singleton
{
public:
    static T* getInstance()
    {
        DM_APP_LOCK
        if (m_instance == 0)
        {
            m_instance = new T;
        }
        DM_APP_UNLOCK
        return m_instance;
    }

    static T* instance()
    {
        return m_instance;
    }

    static void releaseInstance()
    {
        DM_APP_LOCK
        if (m_instance != 0)
        {
            delete m_instance;
            m_instance = 0;
        }
        DM_APP_UNLOCK
    }

    static T* m_instance;
};
template<class T>
T* Singleton<T>::m_instance = 0;

DM_END_NAMESPACE
#define DM_INSTANCE(CLASS) CLASS##::instance()
#endif // DMSINGLETON_H
