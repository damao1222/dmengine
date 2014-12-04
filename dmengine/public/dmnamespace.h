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

#ifndef DMNAMESPACE_H
#define DMNAMESPACE_H
#include "dmglobal.h"
#include "dmtypeinfo.h"
DM_BEGIN_NAMESPACE
typedef unsigned char duint8;
typedef unsigned short duint16;
typedef unsigned int duint32;
typedef unsigned long dulong;
typedef signed char dint8;
typedef signed short dint16;
typedef signed int dint32;
typedef long dlong;
typedef bool dbool;

#if defined(DM_USE_FLOATS) || defined(DM_NO_FPU) || defined(DM_ARCH_ARM) || defined(DM_ARCH_WINDOWSCE) || defined(DM_ARCH_SYMBIAN)
typedef float dreal;
#else
#define DM_DOUBLE_AS_REAL
typedef double dreal;
#endif

typedef char dchar;
typedef duint8 duchar;
typedef duchar dbyte;
typedef dint32 dint;
typedef duint32 duint;

#if defined(DM_OS_WIN) && !defined(DM_CC_GNU) && !defined(DM_CC_MWERKS)
#  define DM_INT64_C(c) c ## i64    /* signed 64 bit constant */
#  define DM_UINT64_C(c) c ## ui64   /* unsigned 64 bit constant */
typedef __int64 dint64;            /* 64 bit signed */
typedef unsigned __int64 duint64;  /* 64 bit unsigned */
#else
#  define DM_INT64_C(c) static_cast<long long>(c ## LL)     /* signed 64 bit constant */
#  define DM_UINT64_C(c) static_cast<unsigned long long>(c ## ULL) /* unsigned 64 bit constant */
typedef long long dint64;           /* 64 bit signed */
typedef unsigned long long duint64; /* 64 bit unsigned */
#endif

//typedef long long dint64;
typedef dint64 dlonglong;
//typedef unsigned long long duint64;
typedef duint64 dulonglong;

DM_DECLARE_TYPEINFO(dbool, DM_PRIMITIVE_TYPE);
DM_DECLARE_TYPEINFO(dchar, DM_PRIMITIVE_TYPE);
//DM_DECLARE_TYPEINFO(signed char, DM_PRIMITIVE_TYPE);
DM_DECLARE_TYPEINFO(duchar, DM_PRIMITIVE_TYPE);
DM_DECLARE_TYPEINFO(dint16, DM_PRIMITIVE_TYPE);
DM_DECLARE_TYPEINFO(duint16, DM_PRIMITIVE_TYPE);
DM_DECLARE_TYPEINFO(dint, DM_PRIMITIVE_TYPE);
DM_DECLARE_TYPEINFO(duint, DM_PRIMITIVE_TYPE);
DM_DECLARE_TYPEINFO(dlong, DM_PRIMITIVE_TYPE);
DM_DECLARE_TYPEINFO(dulong, DM_PRIMITIVE_TYPE);
DM_DECLARE_TYPEINFO(dint64, DM_PRIMITIVE_TYPE);
DM_DECLARE_TYPEINFO(duint64, DM_PRIMITIVE_TYPE);
DM_DECLARE_TYPEINFO(float, DM_PRIMITIVE_TYPE);
DM_DECLARE_TYPEINFO(double, DM_PRIMITIVE_TYPE);

template <int> struct IntegerForSize;
template <>    struct IntegerForSize<1> { typedef duint8  Unsigned; typedef dint8  Signed; };
template <>    struct IntegerForSize<2> { typedef duint16 Unsigned; typedef dint16 Signed; };
template <>    struct IntegerForSize<4> { typedef duint32 Unsigned; typedef dint32 Signed; };
template <>    struct IntegerForSize<8> { typedef duint64 Unsigned; typedef dint64 Signed; };
template <class T> struct IntegerForSizeof: IntegerForSize<sizeof(T)> { };
typedef IntegerForSizeof<void*>::Unsigned uintptr;
typedef IntegerForSizeof<void*>::Signed ptrdiff;

typedef enum{
    UnknowShape = 0,
    PointShape,
    LineShape,
    RectShape,
    CircleShape,
    UserShape = 1 << 12
} ShapeType;
DM_END_NAMESPACE
//Special type
class
DM_DLL_EXPORT
Dm{
public:
#if defined(DM_OS_MAC)
    typedef void * HANDLE;
#elif defined(DM_OS_WIN)
    typedef void *HANDLE;
#elif defined(DM_OS_UNIX)
    typedef unsigned long HANDLE;
#elif defined(DM_WS_QWS) || defined(DM_WS_QPA)
    typedef void * HANDLE;
#elif defined(DM_OS_SYMBIAN)
    typedef unsigned long int HANDLE; // equivalent to TUint32
#endif
    typedef DM::duint16 WORD;
};
#endif // DMNAMESPACE_H
