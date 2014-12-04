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

#ifndef DMTYPEINFO_H
#define DMTYPEINFO_H
#include "dmglobal.h"
DM_BEGIN_NAMESPACE
#if 1
template <typename T>
class TypeInfo
{
public:
    enum {
        isPointer = false,
        isComplex = true,
        isStatic = true,
        isLarge = (sizeof(T)>sizeof(void*)),
        isDummy = false
    };
};

template <typename T>
class TypeInfo<T*>
{
public:
    enum {
        isPointer = true,
        isComplex = false,
        isStatic = false,
        isLarge = false,
        isDummy = false
    };
};
#else
template <typename T> char TypeInfoHelper(T*(*)());
void* TypeInfoHelper(...);
template <typename T>
class TypeInfo
{
public:
    enum {
        isPointer = (1 == sizeof(TypeInfoHelper((T(*)())0))),
        isComplex = !isPointer,
        isStatic = !isPointer,
        isLarge = (sizeof(T)>sizeof(void*)),
        isDummy = false
    };
};
#endif

#define DM_DECLARE_TYPEINFO(TYPE, FLAGS) \
template<> \
DM_DECLARE_TYPEINFO_BODY(TYPE, FLAGS)

/*
   Specialize a specific type with:

     DM_DECLARE_TYPEINFO(type, flags);

   where 'type' is the name of the type to specialize and 'flags' is
   logically-OR'ed combination of the flags below.
*/
enum { /* TYPEINFO flags */
    DM_COMPLEX_TYPE = 0,
    DM_PRIMITIVE_TYPE = 0x1,
    DM_STATIC_TYPE = 0,
    DM_MOVABLE_TYPE = 0x2,
    DM_DUMMY_TYPE = 0x4
};

#define DM_DECLARE_TYPEINFO_BODY(TYPE, FLAGS) \
class TypeInfo<TYPE > \
{ \
public: \
    enum { \
        isComplex = (((FLAGS) & DM_PRIMITIVE_TYPE) == 0), \
        isStatic = (((FLAGS) & (DM_MOVABLE_TYPE | DM_PRIMITIVE_TYPE)) == 0), \
        isLarge = (sizeof(TYPE)>sizeof(void*)), \
        isPointer = false, \
        isDummy = (((FLAGS) & DM_DUMMY_TYPE) != 0) \
    }; \
    static inline const char *name() { return #TYPE; } \
}

#define DM_DECLARE_TYPEINFO(TYPE, FLAGS) \
template<> \
DM_DECLARE_TYPEINFO_BODY(TYPE, FLAGS)

DM_END_NAMESPACE
#endif // DMTYPEINFO_H
