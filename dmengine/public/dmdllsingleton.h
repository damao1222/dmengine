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
#ifndef DMDLLSINGLETON_H
#define DMDLLSINGLETON_H

#include "dmglobal.h"

#ifndef DM_NO_THREAD
  #include "thread/dmsinglelock.h"

  #define DM_DECLARE_SINGLETON(CLASS) \
                    static CLASS* instance##CLASS; \
                    static Mutex instanceMutex;

  #define DM_PRIVATE_SINGLETON(CLASS) \
                    CLASS* CLASS##Private::instance##CLASS = NULL; \
                    Mutex CLASS##Private::instanceMutex;

  #define DM_PRIVATE_SINGLETON_LOCK(CLASS) \
                    DM_NS::SingleLock _instance_lock(&CLASS##Private::instanceMutex)

#else // DM_NO_THREAD defined
  #define DM_DECLARE_SINGLETON(CLASS) \
                    static CLASS* instance##CLASS; 

#define DM_PRIVATE_SINGLETON(CLASS) \
                    CLASS* CLASS##Private::instance##CLASS = NULL; 

#define DM_PRIVATE_SINGLETON_LOCK(CLASS)

#endif // DM_NO_THREAD

#define DM_SINGLETON_GET_INSTANCE(CLASS) \
    DM_PRIVATE_SINGLETON_LOCK(CLASS); \
    if (!CLASS##Private::instance##CLASS) CLASS##Private::instance##CLASS = new CLASS; \
    return CLASS##Private::instance##CLASS;

//no lock
#define DM_SINGLETON_INSTANCE(CLASS) \
    return CLASS##Private::instance##CLASS;

#define DM_SINGLETON_RELEASE_INSTANCE(CLASS) \
    DM_PRIVATE_SINGLETON_LOCK(CLASS); \
    DM_SAFE_DELETE(CLASS##Private::instance##CLASS);

#define DM_GETINSTANCE_FUNCNAME                     getInstance
#define DM_INSTANCE_FUNCNAME                            instance
#define DM_RELEASEINSTANCE_FUNCNAME             releaseInstance

#define DM_DECLARE_SINGLETON_FUNC(CLASS) \
    static CLASS* DM_GETINSTANCE_FUNCNAME(); \
    static CLASS* DM_INSTANCE_FUNCNAME(); \
    static void DM_RELEASEINSTANCE_FUNCNAME();

#define DM_INTERNAL_SINGLETON(CLASS) \
    DM_PRIVATE_SINGLETON(CLASS) \
    CLASS* CLASS::DM_GETINSTANCE_FUNCNAME() { DM_SINGLETON_GET_INSTANCE(CLASS) } \
    CLASS* CLASS::DM_INSTANCE_FUNCNAME() { DM_SINGLETON_INSTANCE(CLASS) } \
    void CLASS::DM_RELEASEINSTANCE_FUNCNAME() { DM_SINGLETON_RELEASE_INSTANCE(CLASS) } 

#endif // DMDLLSINGLETON_H
