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
#ifndef DMVARIANT_H
#define DMVARIANT_H
#include "dmnamespace.h"
#ifndef DM_NO_CONTAINER_VARIANT
#include "dmvector.h"
#include "dmhash.h"
#endif //DM_NO_CONTAINER_VARIANT
#include "dmmetatype.h"

DM_BEGIN_NAMESPACE
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
#ifndef DM_NO_CONTAINER_VARIANT
class Variant;
typedef Vector<Variant> VarList;
typedef Hash<UtilString, Variant> VarHash;
#endif

//#define DM_VARIANT_PTR

DM_PRIVATE_CLASS(Variant);
class DM_DLL_EXPORT Variant
{
    DM_DECLARE_PRIVATE(Variant)
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
        eLastPrimitive = eChar,
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
#ifndef DM_NO_CONTAINER_VARIANT
        eVarList = 20,
        eVarHash  = 21,
#endif
        eLastCoreType = eVarHash,
#ifdef DM_VARIANT_PTR
        eVoidPtr = 128,
#endif
        eUserType = 256
    };

    Variant(); 
    Variant(dbool v);
    Variant(dint v);
    Variant(duint v);
    Variant(dlonglong v);
    Variant(dulonglong v);
    Variant(double v);
    Variant(float v);
    Variant(dchar v);
#ifdef DM_VARIANT_PTR
    Variant(void* v);
#endif
    Variant(const Rect &v);
    Variant(const RectF &v);
    Variant(const Circle &v);
    Variant(const CircleF &v);
    Variant(const Size &v);
    Variant(const SizeF &v);
    Variant(const Line &v);
    Variant(const LineF &v);
    Variant(const Point &v);
    Variant(const PointF &v);
    Variant(const char *v);
    Variant(const UtilString &v);

#ifndef DM_NO_CONTAINER_VARIANT
    Variant(const VarList &v);
    Variant(const VarHash &v);
#endif
    Variant(int typeOrUserType, const void *copy);
    Variant(int typeOrUserType, const void *copy, duint flags);

    Variant(const Variant &other);
    ~Variant();

    dbool isValid() const;
    dbool isNull()const;
    void setSharable(dbool sharable);
    dbool isSharedWith(const Variant &other) const;
    dbool isDetached() const;
    Type type() const;
    dint userType() const;
    dbool canConvert(Type t) const;

    dbool toBool(dbool *ok = NULL) const;
    dint toInt(dbool *ok = NULL) const;
    duint toUInt(dbool *ok = NULL) const;
    dlonglong toLongLong(dbool *ok = NULL) const;
    dulonglong toULongLong(dbool *ok = NULL) const;
    double toDouble(dbool *ok = NULL) const;
    float toFloat(dbool *ok = NULL) const;
    dchar toChar(dbool *ok = NULL) const;
#ifdef DM_VARIANT_PTR
    void* toPointer(dbool *ok = NULL) const;
#endif
    Rect toRect(dbool *ok = NULL) const;
    RectF toRectF(dbool *ok = NULL) const;
    Circle toCircle(dbool *ok = NULL) const;
    CircleF toCircleF(dbool *ok = NULL) const;
    Size toSize(dbool *ok = NULL) const;
    SizeF toSizeF(dbool *ok = NULL) const;
    Line toLine(dbool *ok = NULL) const;
    LineF toLineF(dbool *ok = NULL) const;
    Point toPoint(dbool *ok = NULL) const;
    PointF toPointF(dbool *ok = NULL) const;
    UtilString toString(dbool *ok = NULL) const;

    dbool isBool() const;
    dbool isInt() const;
    dbool isUInt() const;
    dbool isLongLong() const;
    dbool isULongLong() const;
    dbool isDouble() const;
    dbool isFloat() const;
    dbool isChar() const;
#ifdef DM_VARIANT_PTR
    dbool isPointer() const;
#endif
    dbool isRect() const;
    dbool isRectF() const;
    dbool isCircle() const;
    dbool isCircleF() const;
    dbool isSize() const;
    dbool isSizeF() const;
    dbool isLine() const;
    dbool isLineF() const;
    dbool isPoint() const;
    dbool isPointF() const;
    dbool isString() const;

#ifndef DM_NO_CONTAINER_VARIANT
    VarList toList(dbool *ok = NULL) const;
    VarHash toHash(dbool *ok = NULL) const;

    dbool isList() const;
    dbool isHash() const;

    //Container APIs
    void append(const Variant &v);
    void prepend(const Variant &v);
    void insert(const UtilString &key, const Variant &value);
    dint size() const;
    const Variant& at(dint i) const; 
    const Variant value(const UtilString &key) const; 
    const Variant value(const UtilString &key, const Variant &defaultValue) const; 
    void clear();

    const Variant &operator[](dint i) const;
    Variant &operator[](dint i);
    const Variant &operator[](const UtilString &key) const;
    Variant &operator[](const UtilString &key);
#endif

    dbool operator==(const Variant &other) const;
    dbool operator!=(const Variant &other) const
    { return !(*this == other); }

    Variant& operator=(const Variant &other);

    void *data();
    const void *constData() const;
    inline const void *data() const { return constData(); }

    template<typename T>
    inline void setValue(const T &value);

    template<typename T>
    inline T value() const;

    template<typename T>
    static inline Variant fromValue(const T &value);

    template<typename T>
    dbool canConvert() const;

    friend DM_DLL_EXPORT dbool variant_cast_helper(const Variant &v, Variant::Type tp, void *ptr);
    friend DM_DLL_EXPORT dbool variant_set_helper(Variant &v, Variant::Type tp, duint flag);
};

DM_DECLARE_TYPEINFO(Variant, DM_MOVABLE_TYPE);

template<typename T> 
inline T variant_cast(const Variant &v)
{
    const int vid = dmMetaTypeId<T>(static_cast<T *>(0));
    if (vid == v.userType())
        return *reinterpret_cast<const T *>(v.constData());
    if (vid < dint(MetaType::eUser)) {
        T t;
        if (variant_cast_helper(v, Variant::Type(vid), &t))
            return t;
    }
    return T();
}

template <typename T>
inline void VariantSetValue(Variant &v, const T &t)
{
    //if possible we reuse the current Variant private
    const duint type = dmMetaTypeId<T>(reinterpret_cast<T *>(0));

    duint primitiveType = Variant::eLastPrimitive;
    if (v.isDetached() && (type == v.userType() || (type <= primitiveType && v.userType() <= primitiveType))) {
        T *old = reinterpret_cast<T*>(v.data());
        if (TypeInfo<T>::isComplex)
            old->~T();
        new (old) T(t); //call the copy constructor
        variant_set_helper(v, Variant::Type(type), TypeInfo<T>::isComplex);
    } else {
        v = Variant(type, &t, TypeInfo<T>::isPointer);
    }
}

template <>
inline void VariantSetValue<Variant>(Variant &v, const Variant &t)
{
    v = t;
}

template<typename T>
inline void Variant::setValue(const T &avalue)
{ VariantSetValue(*this, avalue); }

template<typename T>
inline T Variant::value() const
{
    return variant_cast<T>(*this);
}

template<>
inline Variant Variant::value<Variant>() const
{
    return *this;
}

template <typename T>
inline Variant VariantFromValue(const T &t)
{
    return Variant(dmMetaTypeId<T>(reinterpret_cast<T *>(0)), &t, TypeInfo<T>::isPointer);
}

template <>
inline Variant VariantFromValue(const Variant &t) { return t; }

template<typename T>
inline Variant Variant::fromValue(const T &value)
{
    return VariantFromValue(value);
}

template<typename T>
inline dbool Variant::canConvert() const
{ 
    return canConvert(Type(dmMetaTypeId<T>())); 
}

DM_END_NAMESPACE
#endif // DMVARIANT_H
