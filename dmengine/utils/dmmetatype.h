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
#ifndef DMMETATYPE_H
#define DMMETATYPE_H
#include "dmnamespace.h"
#include "dmatomicint.h"
DM_BEGIN_NAMESPACE

class DM_DLL_EXPORT MetaType {
public:
    enum Type{
        eInvalid = 0,
        eBool = 1,
        eInt = 2,
        eUInt = 3,
        eLongLong = 4,
        eULongLong = 5,
        eDouble = 6,
        eFloat = 7,
        eChar = 8,
        eRect = 9,
        eRectF = 10,
        eCircle = 11,
        eCircleF = 12,
        eSize = 13,
        eSizeF = 14,
        eLine = 15,
        eLineF = 16,
        ePoint = 17,
        ePointF = 18,
        eString = 19,
        eWString = 20,
#if defined(DM_USE_FLOATS)
        eReal = eFloat,
#else
        eReal = eDouble,
#endif
        eVoidPtr = 128,
        eObjectPtr = 129,
        eVariant = 196,
        eUser = 256
    };

    typedef void (*Destructor)(void *);
    typedef void *(*Constructor)(const void *);

    static int registerType(const char *typeName, Destructor destructor,
                            Constructor constructor);
    static int registerTypedef(const char *typeName, int aliasId);
    static int type(const char *typeName);
    static const char *typeName(int type);
    static bool isRegistered(int type);
    static void *construct(int type, const void *copy = 0);
    static void destroy(int type, void *data);
    static void unregisterType(const char *typeName);
};

template <typename T>
void dmMetaTypeDeleteHelper(T *t)
{
    delete t;
}

template <typename T>
void *dmMetaTypeConstructHelper(const T *t)
{
    if (!t)
        return new T();
    return new T(*static_cast<const T*>(t));
}


template <typename T>
struct MetaTypeId
{
    enum { Defined = 0 };
};

template <typename T>
struct MetaTypeId2
{
    enum { Defined = MetaTypeId<T>::Defined };
    static inline int dm_metatype_id() { return MetaTypeId<T>::dm_metatype_id(); }
};

namespace DmPrivate {
    template <typename T, bool Defined = MetaTypeId2<T>::Defined>
    struct MetaTypeIdHelper {
        static inline int dm_metatype_id()
        { return MetaTypeId2<T>::dm_metatype_id(); }
    };
    template <typename T> struct MetaTypeIdHelper<T, false> {
        static inline int dm_metatype_id()
        { return -1; }
    };
}

template <typename T>
inline int dmMetaTypeId(T * /* dummy */ = 0)
{
    return MetaTypeId2<T>::dm_metatype_id();
}

template <typename T>
int dmRegisterMetaType(const char *typeName, T * dummy = 0)
{
    const int typedefOf = dummy ? -1 : DmPrivate::MetaTypeIdHelper<T>::dm_metatype_id();
    if (typedefOf != -1)
        return MetaType::registerTypedef(typeName, typedefOf);

    typedef void*(*ConstructPtr)(const T*);
    ConstructPtr cptr = dmMetaTypeConstructHelper<T>;
    typedef void(*DeletePtr)(T*);
    DeletePtr dptr = dmMetaTypeDeleteHelper<T>;

    return MetaType::registerType(typeName, reinterpret_cast<MetaType::Destructor>(dptr),
                                   reinterpret_cast<MetaType::Constructor>(cptr));
}

template <typename T>
inline int dmRegisterMetaType(T * dummy = 0)
{
#ifdef DM_CC_SUN
    return dmMetaTypeId(static_cast<T *>(0));
#else
    return dmMetaTypeId(dummy);
#endif
}

#define DM_DECLARE_METATYPE(TYPE)  \
    DM_BEGIN_NAMESPACE  \
    template <> \
    struct MetaTypeId< TYPE > \
    {  \
        enum { Defined = 1 };  \
        static int dm_metatype_id() \
            {   \
                static BasicAtomicInt metatype_id = DM_BASIC_ATOMIC_INITIALIZER(0); \
                if (!metatype_id)  \
                    metatype_id = dmRegisterMetaType< TYPE >(#TYPE,  \
                               reinterpret_cast< TYPE *>(uintptr(-1))); \
                return metatype_id; \
            }  \
    };  \
    DM_END_NAMESPACE

#define DM_DECLARE_BUILTIN_METATYPE(TYPE, NAME) \
    DM_BEGIN_NAMESPACE \
    template<> struct MetaTypeId2<TYPE> \
    { \
        enum { Defined = 1, eMetaType = MetaType::NAME }; \
        static inline int dm_metatype_id() { return MetaType::NAME; } \
    }; \
    DM_END_NAMESPACE

class Point;
class PointF;
class Size;
class SizeF;
class Line;
class LineF;
class Rect;
class RectF;
class Circle;
class CircleF;
class UtilString;
class WString;
class Variant;
class Object;
DM_END_NAMESPACE

DM_DECLARE_BUILTIN_METATYPE(UtilString, eString)
DM_DECLARE_BUILTIN_METATYPE(WString, eWString)
DM_DECLARE_BUILTIN_METATYPE(dint, eInt)
DM_DECLARE_BUILTIN_METATYPE(duint, eUInt)
DM_DECLARE_BUILTIN_METATYPE(dbool, eBool)
DM_DECLARE_BUILTIN_METATYPE(double, eDouble)
DM_DECLARE_BUILTIN_METATYPE(char, eChar)
DM_DECLARE_BUILTIN_METATYPE(float, eFloat)
DM_DECLARE_BUILTIN_METATYPE(void *, eVoidPtr)
DM_DECLARE_BUILTIN_METATYPE(Object *, eObjectPtr)
DM_DECLARE_BUILTIN_METATYPE(dlonglong, eLongLong)
DM_DECLARE_BUILTIN_METATYPE(dulonglong, eULongLong)
DM_DECLARE_BUILTIN_METATYPE(Rect, eRect)
DM_DECLARE_BUILTIN_METATYPE(RectF, eRectF)
DM_DECLARE_BUILTIN_METATYPE(Size, eSize)
DM_DECLARE_BUILTIN_METATYPE(SizeF, eSizeF)
DM_DECLARE_BUILTIN_METATYPE(Line, eLine)
DM_DECLARE_BUILTIN_METATYPE(LineF, eLineF)
DM_DECLARE_BUILTIN_METATYPE(Point, ePoint)
DM_DECLARE_BUILTIN_METATYPE(PointF, ePointF)
DM_DECLARE_BUILTIN_METATYPE(Circle, eCircle)
DM_DECLARE_BUILTIN_METATYPE(CircleF, eCircleF)
DM_DECLARE_BUILTIN_METATYPE(Variant, eVariant)
#endif // DMMETATYPE_H
