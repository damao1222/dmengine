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
#include "dmvariant.h"
#include "private/dmvariant_p.h"
#include "dmrect.h"
#include "dmcircle.h"
#include "dmsize.h"
#include "dmline.h"
#include "dmpoint.h"
#include "dmutilstring.h"
#include "dmmath.h"
#include "dmlogger.h"

DM_BEGIN_NAMESPACE
#ifdef DM_DEBUG
struct _variant_info { const char *name; } VariantTypes[] = {
    {"Invalid"},
    {"Bool"},
    {"Int"},
    {"UInt"},
    {"LongLong"},
    {"ULongLong"},
    {"Double"},
    {"Float"},
    {"Char"},
    {"Pointer"},
    {"Rect"},
    {"RectF"},
    {"Circle"},
    {"CircleF"},
    {"Size"},
    {"SizeF"},
    {"Line"},
    {"LineF"},
    {"Point"},
    {"PointF"},
    {"String"},
#ifndef DM_NO_CONTAINER_VARIANT
    {"VarList"},
    {"VarHash"},
#endif
    {"Unknown"}
};
#define VARIANT_TYPENAME(Type) (VariantTypes[Type].name)
#else
#define VARIANT_TYPENAME(Type)
#endif

static void clear(VariantPrivate::Data *data);
static dbool convert(const VariantPrivate::Data *data, Variant::Type t, void *result, bool *ok);
static void construct(VariantPrivate::Data *x, const void *copy);

static inline dbool IsClass(dint type)
{
    return type > Variant::eChar;
}

VariantPrivate::Data* VariantPrivate::create()
{
    Data *d = new Data;
    d->data.ll = 0;
    d->ref = 1;
    d->sharable = true;
    d->type = Variant::eInvalid;

    return d;
}

void VariantPrivate::detach()
{
    if (d->sharable && d->ref != 1) detach_helper();
}

void VariantPrivate::detach_helper()
{
    Data *x = __detach();
    if (!x->ref.deref())
        free(x);
}

void VariantPrivate::detach_helper(Data* data)
{
    Data *x = __detach(data);
    if (!x->ref.deref())
        free(x);
}

VariantPrivate::Data* VariantPrivate::__detach(Data* data)
{
    Data *x = d;
    Data* t = new Data;

    this->construct(t, data);
    t->ref = 1;
    t->sharable = true;
    t->isComplex = data->isComplex;

    d = t;
    return x;
}

VariantPrivate::Data* VariantPrivate::__detach()
{
    return __detach(d);
}

void VariantPrivate::free(Data *data)
{
    if (data->isComplex)
    {
        //DM_SAFE_DELETE(data->data.ptr);
        clear(data);
    }

    DM_SAFE_DELETE(data);
}

VariantPrivate::Data VariantPrivate::shared_null = { DM_BASIC_ATOMIC_INITIALIZER(1), 0, Variant::eInvalid, true, false };

template <typename T>
inline T VariantToHelper(const VariantPrivate *d, bool *ok, Variant::Type t, const T& val)
{
    if (ok)
        *ok = true;

    if (d->d->type == t)
        return val;

    T ret;
    if (!convert(d->d, t, &ret, ok))
        *ok = false;
    return ret;
}

Variant::Variant(): 
    C_D(Variant)
{
    pdm->d = &VariantPrivate::shared_null;
    pdm->d->ref.ref();
}

Variant::Variant(dbool v): 
    C_D(Variant)
{
    pdm->d = pdm->create();

    pdm->d->type = eBool;
    pdm->d->data.b = v;
    pdm->d->isComplex = false;
}

Variant::Variant(dint v): 
    C_D(Variant)
{
    pdm->d = pdm->create();

    pdm->d->type = eInt;
    pdm->d->data.i = v;
    pdm->d->isComplex = false;
}

Variant::Variant(duint v): 
    C_D(Variant)
{
    pdm->d = pdm->create();

    pdm->d->type = eUInt;
    pdm->d->data.u = v;
    pdm->d->isComplex = false;
}

Variant::Variant(dlonglong v): 
    C_D(Variant)
{
    pdm->d = pdm->create();

    pdm->d->type = eLongLong;
    pdm->d->data.ll = v;
    pdm->d->isComplex = false;
}

Variant::Variant(dulonglong v): 
    C_D(Variant)
{
    pdm->d = pdm->create();

    pdm->d->type = eULongLong;
    pdm->d->data.ull = v;
    pdm->d->isComplex = false;
}

Variant::Variant(double v): 
    C_D(Variant)
{
    pdm->d = pdm->create();

    pdm->d->type = eDouble;
    pdm->d->data.d = v;
    pdm->d->isComplex = false;
}

Variant::Variant(float v): 
    C_D(Variant)
{
    pdm->d = pdm->create();

    pdm->d->type = eFloat;
    pdm->d->data.f = v;
    pdm->d->isComplex = false;
}

Variant::Variant(dchar v): 
    C_D(Variant)
{
    pdm->d = pdm->create();

    pdm->d->type = eChar;
    pdm->d->data.c = v;
    pdm->d->isComplex = false;
}

#ifdef DM_VARIANT_PTR
Variant::Variant(void* v):
    C_D(Variant)
{
    pdm->d = pdm->create();

    pdm->d->type = eVoidPtr;
    pdm->d->data.ptr = v;
    pdm->d->isComplex = false;
}
#endif

Variant::Variant(const Rect &v): 
    C_D(Variant)
{
    pdm->d = pdm->create();

    pdm->d->type = eRect;
    pdm->d->data.ptr = new Rect(v);
    pdm->d->isComplex = true;
}

Variant::Variant(const RectF &v): 
    C_D(Variant)
{
    pdm->d = pdm->create();

    pdm->d->type = eRectF;
    pdm->d->data.ptr = new RectF(v);
    pdm->d->isComplex = true;
}

Variant::Variant(const Circle &v): 
    C_D(Variant)
{
    pdm->d = pdm->create();

    pdm->d->type = eCircle;
    pdm->d->data.ptr = new Circle(v);
    pdm->d->isComplex = true;
}

Variant::Variant(const CircleF &v): 
    C_D(Variant)
{
    pdm->d = pdm->create();

    pdm->d->type = eCircleF;
    pdm->d->data.ptr = new CircleF(v);
    pdm->d->isComplex = true;
}

Variant::Variant(const Size &v): 
    C_D(Variant)
{
    pdm->d = pdm->create();

    pdm->d->type = eSize;
    pdm->d->data.ptr = new Size(v);
    pdm->d->isComplex = true;
}

Variant::Variant(const SizeF &v): 
    C_D(Variant)
{
    pdm->d = pdm->create();

    pdm->d->type = eSizeF;
    pdm->d->data.ptr = new SizeF(v);
    pdm->d->isComplex = true;
}

Variant::Variant(const Line &v): 
    C_D(Variant)
{
    pdm->d = pdm->create();

    pdm->d->type = eLine;
    pdm->d->data.ptr = new Line(v);
    pdm->d->isComplex = true;
}

Variant::Variant(const LineF &v): 
    C_D(Variant)
{
    pdm->d = pdm->create();

    pdm->d->type = eLineF;
    pdm->d->data.ptr = new LineF(v);
    pdm->d->isComplex = true;
}

Variant::Variant(const Point &v): 
    C_D(Variant)
{
    pdm->d = pdm->create();

    pdm->d->type = ePoint;
    pdm->d->data.ptr = new Point(v);
    pdm->d->isComplex = true;
}

Variant::Variant(const PointF &v): 
    C_D(Variant)
{
    pdm->d = pdm->create();

    pdm->d->type = ePointF;
    pdm->d->data.ptr = new PointF(v);
    pdm->d->isComplex = true;
}

Variant::Variant(const char *v): 
    C_D(Variant)
{
    DM_ASSERT(v);
    pdm->d = pdm->create();

    pdm->d->type = eString;
    pdm->d->data.ptr = new UtilString(v);
    pdm->d->isComplex = true;
}

Variant::Variant(const UtilString &v): 
    C_D(Variant)
{
    pdm->d = pdm->create();

    pdm->d->type = eString;
    pdm->d->data.ptr = new UtilString(v);
    pdm->d->isComplex = true;
}

#ifndef DM_NO_CONTAINER_VARIANT
Variant::Variant(const VarList &v): 
    C_D(Variant)
{
    pdm->d = pdm->create();

    pdm->d->type = eVarList;
    pdm->d->data.ptr = new VarList(v);
    pdm->d->isComplex = true;
}

Variant::Variant(const VarHash &v): 
    C_D(Variant)
{
    pdm->d = pdm->create();

    pdm->d->type = eVarHash;
    pdm->d->data.ptr = new VarHash(v);
    pdm->d->isComplex = true;
}    
#endif

Variant::Variant(int typeOrUserType, const void *copy):
    C_D(Variant)
{
    pdm->d = pdm->create();

    pdm->d->type = typeOrUserType;

    construct(pdm->d, copy);
}

Variant::Variant(int typeOrUserType, const void *copy, duint flags):
    C_D(Variant)
{
    pdm->d = pdm->create();
    if (flags) { //type is a pointer type
        pdm->d->type = typeOrUserType;
        pdm->d->isComplex = false;
        pdm->d->data.ptr = *reinterpret_cast<void *const*>(copy);
    } else {
        pdm->d->type = typeOrUserType;
        construct(pdm->d, copy);
    }
}
    
Variant::Variant(const Variant &other):
    C_D(Variant)
{
    pdm->d = other.pdm->d;
    pdm->d->ref.ref();
    if (!pdm->d->sharable)
    {
        pdm->detach_helper();
    }
}

Variant::~Variant()
{
    if (!pdm->d->ref.deref())
        pdm->free(pdm->d);

    D_D(Variant);
}

dbool Variant::isValid() const
{
    return pdm->d->type != eInvalid;
}

dbool Variant::isNull()const
{
    return pdm->d->data.ll == 0;
}

void Variant::setSharable(dbool sharable)
{
    if (!sharable) 
        pdm->detach(); 

    pdm->d->sharable = sharable;
}

dbool Variant::isSharedWith(const Variant &other) const
{
    return pdm->d == other.pdm->d;
}

dbool Variant::isDetached() const
{ 
    return !pdm->d->sharable || pdm->d->ref == 1; 
}

Variant::Type Variant::type() const
{
    return pdm->d->type >= MetaType::eUser ? eUserType : static_cast<Type>(pdm->d->type);
}

dint Variant::userType() const
{
    return pdm->d->type;
}

static const duint32 CanConvertMatrix[Variant::eLastCoreType + 1] =
{
    /*Invalid*/     0,

    /*Bool*/          1 << Variant::eInt        | 1 << Variant::eUInt
    | 1 << Variant::eLongLong   | 1 << Variant::eULongLong  | 1 << Variant::eDouble
    | 1 << Variant::eFloat      | 1 << Variant::eChar       | 1 << Variant::eString,

    /*Int*/           1 << Variant::eBool       | 1 << Variant::eUInt
    | 1 << Variant::eLongLong   | 1 << Variant::eULongLong  | 1 << Variant::eDouble
    | 1 << Variant::eFloat      | 1 << Variant::eChar       | 1 << Variant::eString,

    /*UInt*/          1 << Variant::eBool       | 1 << Variant::eInt        
    | 1 << Variant::eLongLong   | 1 << Variant::eULongLong  | 1 << Variant::eDouble
    | 1 << Variant::eFloat      | 1 << Variant::eChar       | 1 << Variant::eString,

    /*LLong*/         1 << Variant::eBool       | 1 << Variant::eInt        | 1 << Variant::eUInt
    | 1 << Variant::eULongLong  | 1 << Variant::eDouble
    | 1 << Variant::eFloat      | 1 << Variant::eChar       | 1 << Variant::eString,

    /*ULlong*/        1 << Variant::eBool       | 1 << Variant::eInt        | 1 << Variant::eUInt
    | 1 << Variant::eLongLong   | 1 << Variant::eDouble
    | 1 << Variant::eFloat      | 1 << Variant::eChar       | 1 << Variant::eString,

    /*double*/        1 << Variant::eBool       | 1 << Variant::eInt        | 1 << Variant::eUInt
    | 1 << Variant::eLongLong   | 1 << Variant::eULongLong  
    | 1 << Variant::eFloat      | 1 << Variant::eChar       | 1 << Variant::eString,

    /*float*/         1 << Variant::eBool       | 1 << Variant::eInt        | 1 << Variant::eUInt
    | 1 << Variant::eLongLong   | 1 << Variant::eULongLong  | 1 << Variant::eDouble
    | 1 << Variant::eChar       |1 << Variant::eString,

    /*char*/          1 << Variant::eBool       | 1 << Variant::eInt        | 1 << Variant::eUInt
    | 1 << Variant::eLongLong   | 1 << Variant::eULongLong  | 1 << Variant::eDouble
    | 1 << Variant::eFloat      | 1 << Variant::eString,

    /*Rect*/         1 << Variant::eRectF,

    /*RectF*/        1 << Variant::eRect,

    /*Circle*/       1 << Variant::eCircleF,

    /*CircleF*/      1 << Variant::eCircle,

    /*Size*/         1 << Variant::eSizeF,

    /*SizeF*/        1 << Variant::eSize,

    /*Line*/         1 << Variant::eLineF,

    /*LineF*/        1 << Variant::eLine,

    /*Point*/        1 << Variant::ePointF,

    /*PointF*/       1 << Variant::ePoint,

    /*String*/        1 << Variant::eBool       | 1 << Variant::eInt        | 1 << Variant::eUInt
    | 1 << Variant::eLongLong   | 1 << Variant::eULongLong  | 1 << Variant::eDouble
    | 1 << Variant::eFloat      | 1 << Variant::eChar,

    /*list*/         0,

    /*hash*/         0
};

dbool Variant::canConvert(Type t) const
{
    if (pdm->d->type == t)
        return true;

    if (t > eLastCoreType)
        return false;

    return (CanConvertMatrix[t] & (1 << pdm->d->type)) > 0;
}

dbool Variant::toBool(dbool *ok /*= NULL*/) const
{
    return VariantToHelper<dbool>(pdm, ok, eBool, pdm->d->data.b);
}

dint Variant::toInt(dbool *ok /*= NULL*/) const
{
    return VariantToHelper<dint>(pdm, ok, eInt, pdm->d->data.i);
}

duint Variant::toUInt(dbool *ok /*= NULL*/) const
{
    return VariantToHelper<duint>(pdm, ok, eUInt, pdm->d->data.u);
}

dlonglong Variant::toLongLong(dbool *ok /*= NULL*/) const
{
    return VariantToHelper<dlonglong>(pdm, ok, eLongLong, pdm->d->data.ll);
}

dulonglong Variant::toULongLong(dbool *ok /*= NULL*/) const
{
    return VariantToHelper<dulonglong>(pdm, ok, eULongLong, pdm->d->data.ull);
}

double Variant::toDouble(dbool *ok /*= NULL*/) const
{
    return VariantToHelper<double>(pdm, ok, eDouble, pdm->d->data.d);
}

float Variant::toFloat(dbool *ok /*= NULL*/) const
{
    return VariantToHelper<float>(pdm, ok, eFloat, pdm->d->data.f);
}

dchar Variant::toChar(dbool *ok /*= NULL*/) const
{
    return VariantToHelper<dchar>(pdm, ok, eChar, pdm->d->data.c);
}
#ifdef DM_VARIANT_PTR
void* Variant::toPointer(dbool *ok /*= NULL*/) const
{
    return VariantToHelper<void*>(pdm, ok, eVoidPtr, pdm->d->data.ptr);
}
#endif
Rect Variant::toRect(dbool *ok /*= NULL*/) const
{
    return VariantToHelper<Rect>(pdm, ok, eRect, *v_cast<Rect>(pdm->d));
}

RectF Variant::toRectF(dbool *ok /*= NULL*/) const
{
    return VariantToHelper<RectF>(pdm, ok, eRectF, *v_cast<RectF>(pdm->d));
}

Circle Variant::toCircle(dbool *ok /*= NULL*/) const
{
    return VariantToHelper<Circle>(pdm, ok, eCircle, *v_cast<Circle>(pdm->d));
}

CircleF Variant::toCircleF(dbool *ok /*= NULL*/) const
{
    return VariantToHelper<CircleF>(pdm, ok, eCircleF, *v_cast<CircleF>(pdm->d));
}

Size Variant::toSize(dbool *ok /*= NULL*/) const
{
    return VariantToHelper<Size>(pdm, ok, eSize, *v_cast<Size>(pdm->d));
}

SizeF Variant::toSizeF(dbool *ok /*= NULL*/) const
{
    return VariantToHelper<SizeF>(pdm, ok, eSizeF, *v_cast<SizeF>(pdm->d));
}

Line Variant::toLine(dbool *ok /*= NULL*/) const
{
    return VariantToHelper<Line>(pdm, ok, eLine, *v_cast<Line>(pdm->d));
}

LineF Variant::toLineF(dbool *ok /*= NULL*/) const
{
    return VariantToHelper<LineF>(pdm, ok, eLineF, *v_cast<LineF>(pdm->d));
}

Point Variant::toPoint(dbool *ok /*= NULL*/) const
{
    return VariantToHelper<Point>(pdm, ok, ePoint, *v_cast<Point>(pdm->d));
}

PointF Variant::toPointF(dbool *ok /*= NULL*/) const
{
    return VariantToHelper<PointF>(pdm, ok, ePointF, *v_cast<PointF>(pdm->d));
}

UtilString Variant::toString(dbool *ok /*= NULL*/) const
{
    return VariantToHelper<UtilString>(pdm, ok, eString, *v_cast<UtilString>(pdm->d));
}

dbool Variant::isBool() const
{
    return pdm->d->type == eBool;
}

dbool Variant::isInt() const
{
    return pdm->d->type == eInt;
}

dbool Variant::isUInt() const
{
    return pdm->d->type == eUInt;
}

dbool Variant::isLongLong() const
{
    return pdm->d->type == eLongLong;
}

dbool Variant::isULongLong() const
{
    return pdm->d->type == eULongLong;
}

dbool Variant::isDouble() const
{
    return pdm->d->type == eDouble;
}

dbool Variant::isFloat() const
{
    return pdm->d->type == eFloat;
}

dbool Variant::isChar() const
{
    return pdm->d->type == eChar;
}

#ifdef DM_VARIANT_PTR
dbool Variant::isPointer() const
{
    return pdm->d->type == eVoidPtr;
}
#endif

dbool Variant::isRect() const
{
    return pdm->d->type == eRect;
}

dbool Variant::isRectF() const
{
    return pdm->d->type == eRectF;
}

dbool Variant::isCircle() const
{
    return pdm->d->type == eCircle;
}

dbool Variant::isCircleF() const
{
    return pdm->d->type == eCircleF;
}

dbool Variant::isSize() const
{
    return pdm->d->type == eSize;
}

dbool Variant::isSizeF() const
{
    return pdm->d->type == eSizeF;
}

dbool Variant::isLine() const
{
    return pdm->d->type == eLine;
}

dbool Variant::isLineF() const
{
    return pdm->d->type == eLineF;
}

dbool Variant::isPoint() const
{
    return pdm->d->type == ePoint;
}

dbool Variant::isPointF() const
{
    return pdm->d->type == ePointF;
}

dbool Variant::isString() const
{
    return pdm->d->type == eString;
}


#ifndef DM_NO_CONTAINER_VARIANT
VarList Variant::toList(dbool *ok /*= NULL*/) const
{
    return VariantToHelper<VarList>(pdm, ok, eVarList, *v_cast<VarList>(pdm->d));
}

VarHash Variant::toHash(dbool *ok /*= NULL*/) const
{
    return VariantToHelper<VarHash>(pdm, ok, eVarHash, *v_cast<VarHash>(pdm->d));
}

dbool Variant::isList() const
{
    return pdm->d->type == eVarList;
}

dbool Variant::isHash() const
{
    return pdm->d->type == eVarHash;
}

void Variant::append(const Variant &v)
{
    if (pdm->d->type == eInvalid)
    {
        if (!pdm->d->ref.deref())
            pdm->free(pdm->d);

        pdm->d = pdm->create();

        pdm->d->type = eVarList;
        pdm->d->data.ptr = new VarList();
    }

    if (pdm->d->type == eVarList)
    {
        pdm->detach();
        v_cast<VarList>(pdm->d)->append(v);
    }
    else
        DM_LOGW("Error Type: %s isn't a VarList but use the API of VarList", VARIANT_TYPENAME(pdm->d->type));
}

void Variant::prepend(const Variant &v)
{
    if (pdm->d->type == eInvalid)
    {
        if (!pdm->d->ref.deref())
            pdm->free(pdm->d);

        pdm->d = pdm->create();

        pdm->d->type = eVarList;
        pdm->d->data.ptr = new VarList();
    }

    if (pdm->d->type == eVarList)
    {
        pdm->detach();
        v_cast<VarList>(pdm->d)->prepend(v);
    }
    else
        DM_LOGW("Error Type: %s isn't a VarList but use the API of VarList", VARIANT_TYPENAME(pdm->d->type));
}

void Variant::insert(const UtilString &key, const Variant &value)
{
    if (pdm->d->type == eInvalid)
    {
        if (!pdm->d->ref.deref())
            pdm->free(pdm->d);

        pdm->d = pdm->create();

        pdm->d->type = eVarHash;
        pdm->d->data.ptr = new VarHash();
    }

    if (pdm->d->type == eVarHash)
    {
        pdm->detach();
        v_cast<VarHash>(pdm->d)->insert(key, value);
    }
    else
        DM_LOGW("Error Type: %s isn't a VarHash but use the API of VarHash", VARIANT_TYPENAME(pdm->d->type));
}

dint Variant::size() const
{
    if (pdm->d->type == eInvalid)
        return 0;
    else if (pdm->d->type == eVarList)
        return v_cast<VarList>(pdm->d)->size();
    else if (pdm->d->type == eVarHash)
        return v_cast<VarHash>(pdm->d)->size();
    else
    {
        DM_LOGW("Error Type: %s isn't a container but use the API of container type", VARIANT_TYPENAME(pdm->d->type));
        return 1;
    }
}

const Variant& Variant::at(dint i) const
{
    if (pdm->d->type == eVarList)
        return v_cast<VarList>(pdm->d)->at(i);
    else
    {
        DM_LOGW("Error Type: %s isn't a VarList but use the API of VarList", VARIANT_TYPENAME(pdm->d->type));
        return Variant();
    }
}

const Variant Variant::value(const UtilString &key) const
{
    if (pdm->d->type == eVarHash)
        return v_cast<VarHash>(pdm->d)->value(key);
    else
    {
        DM_LOGW("Error Type: %s isn't a VarHash but use the API of VarHash", VARIANT_TYPENAME(pdm->d->type));
        return Variant();
    }
}

const Variant Variant::value(const UtilString &key, const Variant &defaultValue) const
{
    if (pdm->d->type == eVarHash)
        return v_cast<VarHash>(pdm->d)->value(key, defaultValue);
    else
    {
        DM_LOGW("Error Type: %s isn't a VarHash but use the API of VarHash", VARIANT_TYPENAME(pdm->d->type));
        return defaultValue;
    }
}

void Variant::clear()
{
#if 0
    if (pdm->d->type == eVarList)
        v_cast<VarList>(pdm->d)->clear();
    else if (pdm->d->type == eVarHash)
        v_cast<VarHash>(pdm->d)->clear();
    else
    {
        DM_LOGW("Variant::clear: %s isn't a container but use the API of container type", VARIANT_TYPENAME(pdm->d->type));
        *this = Variant();
    }
#endif
    *this = Variant();
}
#endif

dbool Variant::operator==(const Variant &other) const
{
    if (pdm->d == other.pdm->d)
        return true;

    if (pdm->d->type == other.pdm->d->type)
    {
        if (pdm->d->type == eInvalid)
            return true;

        switch (pdm->d->type)
        {
            case eBool:
            {
                return pdm->d->data.b == other.pdm->d->data.b;
            }
            case eInt:
            {
                return pdm->d->data.i == other.pdm->d->data.i;
            }
            case eUInt:
            {
                return pdm->d->data.u == other.pdm->d->data.u;
            }
            case eLongLong:
            {
                return pdm->d->data.ll == other.pdm->d->data.ll;
            }
            case eULongLong:
            {
                return pdm->d->data.ull == other.pdm->d->data.ull;
            }
            case eDouble:
            {
                return FuzzyCompare(pdm->d->data.d, other.pdm->d->data.d);
            }
            case eFloat:
            {
                return FuzzyCompare(pdm->d->data.f, other.pdm->d->data.f);
            }
            case eChar:
            {
                return pdm->d->data.c == other.pdm->d->data.c;
            }
#ifdef DM_VARIANT_PTR
            case eVoidPtr:
            {
                return pdm->d->data.ptr == other.pdm->d->data.ptr;
            }
#endif
            case eRect:
            {
                return *v_cast<Rect>(pdm->d) == *v_cast<Rect>(other.pdm->d);
            }
            case eRectF:
            {
                return *v_cast<RectF>(pdm->d) == *v_cast<RectF>(other.pdm->d);
            }
            case eCircle:
            {
                return *v_cast<Circle>(pdm->d) == *v_cast<Circle>(other.pdm->d);
            }
            case eCircleF:
            {
                return *v_cast<CircleF>(pdm->d) == *v_cast<CircleF>(other.pdm->d);
            }
            case eSize:
            {
                return *v_cast<Size>(pdm->d) == *v_cast<Size>(other.pdm->d);
            }
            case eSizeF:
            {
                return *v_cast<SizeF>(pdm->d) == *v_cast<SizeF>(other.pdm->d);
            }
            case eLine:
            {
                return *v_cast<Line>(pdm->d) == *v_cast<Line>(other.pdm->d);
            }
            case eLineF:
            {
                return *v_cast<LineF>(pdm->d) == *v_cast<LineF>(other.pdm->d);
            }
            case ePoint:
            {
                return *v_cast<Point>(pdm->d) == *v_cast<Point>(other.pdm->d);
            }
            case ePointF:
            {
                return *v_cast<PointF>(pdm->d) == *v_cast<PointF>(other.pdm->d);
            }
            case eString:
            {
                return *v_cast<UtilString>(pdm->d) == *v_cast<UtilString>(other.pdm->d);
            }
#ifndef DM_NO_CONTAINER_VARIANT
            case eVarList:
            {
                return *v_cast<VarList>(pdm->d) == *v_cast<VarList>(other.pdm->d);
            }
            case eVarHash:
            {
                return *v_cast<VarHash>(pdm->d) == *v_cast<VarHash>(other.pdm->d);
            }
#endif
            default:
            {
                return false;
            }
        }
    }

    return false;
}

Variant& Variant::operator=(const Variant &other)
{
    if (this == &other)
        return *this;

    if (pdm->d != other.pdm->d) 
    {
        VariantPrivate::Data *o = other.pdm->d;
        o->ref.ref();
        if (!pdm->d->ref.deref())
            pdm->free(pdm->d);
        pdm->d = o;
        if (!pdm->d->sharable)
            pdm->detach_helper();
    }

    return *this;
}

#ifndef DM_NO_CONTAINER_VARIANT
const Variant &Variant::operator[](dint i) const
{
    if (pdm->d->type == eVarList)
        return (*v_cast<VarList>(pdm->d))[i];
    else
    {
        DM_LOGW("Error Type: %s is not VarList", VARIANT_TYPENAME(pdm->d->type));
        return Variant();
    }
}

Variant &Variant::operator[](dint i)
{
    if (pdm->d->type == eVarList)
    {
        pdm->detach();
        return (*v_cast<VarList>(pdm->d))[i];
    }
    else
    {
        DM_LOGW("Error Type: %s is not VarList", VARIANT_TYPENAME(pdm->d->type));
        return *this;
    }
}

const Variant &Variant::operator[](const UtilString &key) const
{
    if (pdm->d->type == eVarHash)
        return (*v_cast<VarHash>(pdm->d))[key];
    else
    {
        DM_LOGW("Error Type: %s is not VarHash", VARIANT_TYPENAME(pdm->d->type));
        return Variant();
    }
}

Variant &Variant::operator[](const UtilString &key)
{
    if (pdm->d->type == eInvalid)
    {
        if (!pdm->d->ref.deref())
            pdm->free(pdm->d);

        pdm->d = pdm->create();

        pdm->d->type = eVarHash;
        pdm->d->data.ptr = new VarHash();
    }

    if (pdm->d->type == eVarHash)
    {
        pdm->detach();
        return (*v_cast<VarHash>(pdm->d))[key];
    }
    else
    {
        DM_LOGW("Error Type: %s is not VarHash", VARIANT_TYPENAME(pdm->d->type));
        return *this;
    }
}
#endif

void *Variant::data()
{
    pdm->detach();
    return pdm->d->isComplex ? pdm->d->data.ptr : &pdm->d->data.ptr;
}

const void *Variant::constData() const
{
    return pdm->d->isComplex ? pdm->d->data.ptr : &pdm->d->data.ptr;
}

void VariantPrivate::construct(Data *&d, Data *data)
{
    d->type = data->type;

    switch (d->type)
    {
    case Variant::eBool:
        {
            d->data.b = data->data.b;
            break;
        }
        case Variant::eInt:
        {
            d->data.i = data->data.i;
            break;
        }
        case Variant::eUInt:
        {
            d->data.u = data->data.u;
            break;
        }
        case Variant::eLongLong:
        {
            d->data.ll = data->data.ll;
            break;
        }
        case Variant::eULongLong:
        {
            d->data.ull = data->data.ull;
            break;
        }
        case Variant::eDouble:
        {
            d->data.d = data->data.d;
            break;
        }
        case Variant::eFloat:
        {
            d->data.f = data->data.f;
            break;
        }
        case Variant::eChar:
        {
            d->data.c = data->data.c;
            break;
        }
#ifdef DM_VARIANT_PTR
        case Variant::eVoidPtr:
        {
            d->data.ptr = data->data.ptr;
            break;
        }
#endif
        case Variant::eRect:
        {
            d->data.ptr = new Rect(*v_cast<Rect>(data));
            break;
        }
        case Variant::eRectF:
        {
            d->data.ptr = new RectF(*v_cast<RectF>(data));
            break;
        }
        case Variant::eCircle:
        {
            d->data.ptr = new Circle(*v_cast<Circle>(data));
            break;
        }
        case Variant::eCircleF:
        {
            d->data.ptr = new CircleF(*v_cast<CircleF>(data));
            break;
        }
        case Variant::eSize:
        {
            d->data.ptr = new Size(*v_cast<Size>(data));
            break;
        }
        case Variant::eSizeF:
        {
            d->data.ptr = new SizeF(*v_cast<SizeF>(data));
            break;
        }
        case Variant::eLine:
        {
            d->data.ptr = new Line(*v_cast<Line>(data));
            break;
        }
        case Variant::eLineF:
        {
            d->data.ptr = new LineF(*v_cast<LineF>(data));
            break;
        }
        case Variant::ePoint:
        {
            d->data.ptr = new Point(*v_cast<Point>(data));
            break;
        }
        case Variant::ePointF:
        {
            d->data.ptr = new PointF(*v_cast<PointF>(data));
            break;
        }
        case Variant::eString:
        {
            d->data.ptr = new UtilString(*v_cast<UtilString>(data));
            break;
        }
#ifndef DM_NO_CONTAINER_VARIANT
        case Variant::eVarList:
        {
            d->data.ptr = new VarList(*v_cast<VarList>(data));
            break;
        }
        case Variant::eVarHash:
        {
            d->data.ptr = new VarHash(*v_cast<VarHash>(data));
            break;
        }
#endif
        default:
        {
            break;
        }
    }
}

static void clear(VariantPrivate::Data *data)
{
    switch (data->type)
    {
        case Variant::eRect:
        {
            v_clear<Rect>(data);
            break;
        }
        case Variant::eRectF:
        {
            v_clear<RectF>(data);
            break;
        }
        case Variant::eCircle:
        {
            v_clear<Circle>(data);
            break;
        }
        case Variant::eCircleF:
        {
            v_clear<CircleF>(data);
            break;
        }
        case Variant::eSize:
        {
            v_clear<Size>(data);
            break;
        }
        case Variant::eSizeF:
        {
            v_clear<SizeF>(data);
            break;
        }
        case Variant::eLine:
        {
            v_clear<Line>(data);
            break;
        }
        case Variant::eLineF:
        {
            v_clear<LineF>(data);
            break;
        }
        case Variant::ePoint:
        {
            v_clear<Point>(data);
            break;
        }
        case Variant::ePointF:
        {
            v_clear<PointF>(data);
            break;
        }
        case Variant::eString:
        {
            v_clear<UtilString>(data);
            break;
        }
#ifndef DM_NO_CONTAINER_VARIANT
        case Variant::eVarList:
        {
            v_clear<VarList>(data);
            break;
        }
        case Variant::eVarHash:
        {
            v_clear<VarHash>(data);
            break;
        }
#endif
        default:
        {
            MetaType::destroy(data->type, data->data.ptr);
            break;
        }
    }
}

static dlonglong MetaTypeNumber(const VariantPrivate::Data *d)
{
    switch (d->type) {
    case Variant::eInt:
        return d->data.i;
    case Variant::eLongLong:
        return d->data.ll;
    case Variant::eChar:
        return d->data.c;
    case Variant::eFloat:
        return Round64(d->data.f);
    case Variant::eDouble:
        return Round64(d->data.d);
    }
    DM_ASSERT(false);
    return 0;
}

static dulonglong MetaTypeUNumber(const VariantPrivate::Data *d)
{
    switch (d->type) {
    case Variant::eUInt:
        return d->data.u;
    case Variant::eULongLong:
        return d->data.ull;
    }
    DM_ASSERT(false);
    return 0;
}

static dlonglong ConvertToNumber(const VariantPrivate::Data *d, bool *ok)
{
    *ok = true;

    switch (duint(d->type)) {
    case Variant::eString:
        return v_cast<UtilString>(d)->toLongLong();
    case Variant::eBool:
        return dlonglong(d->data.b);
    case Variant::eDouble:
    case Variant::eInt:
    case Variant::eChar:
    case Variant::eFloat:
    case Variant::eLongLong:
        return MetaTypeNumber(d);
    case Variant::eULongLong:
    case Variant::eUInt:
        return dlonglong(MetaTypeUNumber(d));
    }

    *ok = false;
    return DM_INT64_C(0);
}

static dulonglong ConvertToUnsignedNumber(const VariantPrivate::Data *d, bool *ok)
{
    *ok = true;

    switch (duint(d->type)) {
    case Variant::eString:
        return v_cast<UtilString>(d)->toULongLong();
    case Variant::eBool:
        return dulonglong(d->data.b);
    case Variant::eDouble:
    case Variant::eInt:
    case Variant::eChar:
    case Variant::eFloat:
    case Variant::eLongLong:
        return dulonglong(MetaTypeNumber(d));
    case Variant::eULongLong:
    case Variant::eUInt:
        return MetaTypeUNumber(d);
    }

    *ok = false;
    return DM_UINT64_C(0);
}

static dbool convert(const VariantPrivate::Data *data, Variant::Type t, void *result, bool *ok)
{
    DM_ASSERT(data->type != duint(t));
    DM_ASSERT(result);

    bool dummy;
    if (!ok)
        ok = &dummy;

    switch (duint(t))
    {
        case Variant::eBool:
        {
            bool *b = static_cast<bool *>(result);
            switch (data->type)
            {
                case Variant::eInt:
                case Variant::eLongLong:
                case Variant::eDouble:
                case Variant::eFloat:
                case Variant::eChar:
                {
                    *b = MetaTypeNumber(data) != DM_INT64_C(0);
                    break;
                }
                case Variant::eUInt:
                case Variant::eULongLong:
                {
                    *b = MetaTypeUNumber(data) != DM_UINT64_C(0);
                    break;
                }
#ifdef DM_VARIANT_PTR
                case Variant::eVoidPtr:
                {
                    *b = data->data.ptr != NULL;
                    break;
                }
#endif
                case Variant::eString:
                {
                    *b = v_cast<UtilString>(data)->toBool();
                    break;
                }
                default:
                {
                    *b = false;
                    return false;
                }
            }
            break;
        }
        case Variant::eInt:
        {
            *static_cast<dint *>(result) = dint(ConvertToNumber(data, ok));
            return *ok;
        }
        case Variant::eUInt:
        {
            *static_cast<duint *>(result) = duint(ConvertToUnsignedNumber(data, ok));
            return *ok;
        }
        case Variant::eLongLong:
        {
            *static_cast<dlonglong *>(result) = dlonglong(ConvertToNumber(data, ok));
            return *ok;
        }
        case Variant::eULongLong:
        {
            *static_cast<dulonglong *>(result) = dulonglong(ConvertToUnsignedNumber(data, ok));
            return *ok;
        }
        case Variant::eDouble:
        {
            double *d = static_cast<double *>(result);
            switch (data->type)
            {
                case Variant::eBool:
                {
                    *d = double(data->data.b);
                    break;
                }
                case Variant::eChar:
                case Variant::eInt:
                case Variant::eLongLong:
                {
                    *d = double(MetaTypeNumber(data));
                    break;
                }
                case Variant::eUInt:
                case Variant::eULongLong:
                {
                    *d = double(MetaTypeUNumber(data));
                    break;
                }
                case Variant::eFloat:
                {
                    *d = double(data->data.f);
                    break;
                }
                case Variant::eString:
                {
                    *d = v_cast<UtilString>(data)->toDouble();
                    break;
                }
                default:
                {
                    *d = 0.0;
                    return false;
                }
            }
            break;
        }
        case Variant::eFloat:
        {
            float *f = static_cast<float *>(result);
            switch (data->type)
            {
                case Variant::eBool:
                {
                    *f = float(data->data.b);
                    break;
                }
                case Variant::eChar:
                case Variant::eInt:
                case Variant::eLongLong:
                {
                    *f = float(MetaTypeNumber(data));
                    break;
                }
                case Variant::eUInt:
                case Variant::eULongLong:
                {
                    *f = float(MetaTypeUNumber(data));
                    break;
                }
                case Variant::eDouble:
                {
                    *f = float(data->data.d);
                    break;
                }
                case Variant::eString:
                {
                    *f = v_cast<UtilString>(data)->toFloat();
                    break;
                }
                default:
                {
                    *f = 0.0f;
                    return false;
                }
            }
            break;
        }
        case Variant::eChar:
        {
            dint8 *c = static_cast<dint8 *>(result);
            switch (data->type)
            {
                case Variant::eBool:
                {
                    *c = dint8(data->data.b);
                    break;
                }
                case Variant::eInt:
                case Variant::eLongLong:
                {
                    *c = dint8(MetaTypeNumber(data));
                    break;
                }
                case Variant::eUInt:
                case Variant::eULongLong:
                {
                    *c = dint8(MetaTypeUNumber(data));
                    break;
                }
                case Variant::eDouble:
                {
                    *c = dint8(data->data.d);
                    break;
                }
                case Variant::eFloat:
                {
                    *c = dint8(data->data.f);
                    break;
                }
                case Variant::eString:
                {
                    const UtilString *s = v_cast<UtilString>(data);
                    *c = s->isEmpty() ? 0 : s->at(0);
                    break;
                }
                default:
                {
                    *c = 0;
                    return false;
                }
            }
            break;
        }
#ifdef DM_VARIANT_PTR
        case Variant::eVoidPtr:
        {
            return false;
        }
#endif
        case Variant::eRect:
        {
            Rect *s = static_cast<Rect *>(result);
            switch (data->type) {
            case Variant::eRectF:
                *s = v_cast<RectF>(data)->toRect();
                break;
            default:
                return false;
            }
            break;
        }
        case Variant::eRectF:
        {
            RectF *s = static_cast<RectF *>(result);
            switch (data->type) {
            case Variant::eRect:
                *s = *v_cast<Rect>(data);
                break;
            default:
                return false;
            }
            break;
        }
        case Variant::eCircle:
        {
            Circle *s = static_cast<Circle *>(result);
            switch (data->type) {
            case Variant::eCircleF:
                *s = v_cast<CircleF>(data)->toCircle();
                break;
            default:
                return false;
            }
            break;
        }
        case Variant::eCircleF:
        {
            CircleF *s = static_cast<CircleF *>(result);
            switch (data->type) {
            case Variant::eCircle:
                *s = *v_cast<Circle>(data);
                break;
            default:
                return false;
            }
            break;
        }
        case Variant::eSize:
        {
            Size *s = static_cast<Size *>(result);
            switch (data->type) {
            case Variant::eSizeF:
                *s = v_cast<SizeF>(data)->toSize();
                break;
            default:
                return false;
            }
            break;
        }
        case Variant::eSizeF:
        {
            SizeF *s = static_cast<SizeF *>(result);
            switch (data->type) {
            case Variant::eSize:
                *s = *v_cast<Size>(data);
                break;
            default:
                return false;
            }
            break;
        }
        case Variant::eLine:
        {
            Line *s = static_cast<Line *>(result);
            switch (data->type) {
            case Variant::eLineF:
                *s = v_cast<LineF>(data)->toLine();
                break;
            default:
                return false;
            }
            break;
        }
        case Variant::eLineF:
        {
            LineF *s = static_cast<LineF *>(result);
            switch (data->type) {
            case Variant::eLine:
                *s = *v_cast<Line>(data);
                break;
            default:
                return false;
            }
            break;
        }
        case Variant::ePoint:
        {
            Point *s = static_cast<Point *>(result);
            switch (data->type) {
            case Variant::ePointF:
                *s = v_cast<PointF>(data)->toPoint();
                break;
            default:
                return false;
            }
            break;
        }
        case Variant::ePointF:
        {
            PointF *s = static_cast<PointF *>(result);
            switch (data->type) {
            case Variant::ePoint:
                *s = *v_cast<Point>(data);
                break;
            default:
                return false;
            }
            break;
        }
        case Variant::eString:
        {
            UtilString *s = static_cast<UtilString *>(result);
            switch (data->type)
            {
                case Variant::eBool:
                {
                    *s = UtilString::sprintf("%s", data->data.b ? "true" : "false");
                    break;
                }
                case Variant::eInt:
                {
                    *s = UtilString::sprintf("%d", data->data.i);
                    break;
                }
                case Variant::eUInt:
                {
                    *s = UtilString::sprintf("%u", data->data.u);
                    break;
                }
                case Variant::eLongLong:
                {
                    *s = UtilString::sprintf(DM_FmI64, data->data.ll);
                    break;
                }
                case Variant::eULongLong:
                {
                    *s = UtilString::sprintf(DM_FmIu64, data->data.ull);
                    break;
                }
                case Variant::eDouble:
                {
                    *s = UtilString::sprintf("%g", data->data.d);
                    break;
                }
                case Variant::eFloat:
                {
                    *s = UtilString::sprintf("%g", data->data.f);
                    break;
                }
                case Variant::eChar:
                {
                    *s = UtilString::sprintf("%c", data->data.c);
                    break;
                }
#if 0
                case Variant::eVoidPtr:
                {
                    *s = UtilString::sprintf("%p", data->data.ptr);
                    break;
                }
#endif
                case Variant::eRect:
                {
                    Rect r = *v_cast<Rect>(data);
                    *s = UtilString::sprintf("Rect(%d,%d,%d,%d)", r.x(), r.y(), r.width(), r.height());
                    break;
                }
                case Variant::eRectF:
                {
                    RectF r = *v_cast<RectF>(data);
                    *s = UtilString::sprintf("RectF(%g,%g,%g,%g)", r.x(), r.y(), r.width(), r.height());
                    break;
                }
                case Variant::eCircle:
                {
                    Circle c = *v_cast<Circle>(data);
                    *s = UtilString::sprintf("Circle(%d,%d,%d)", c.center().x(), c.center().y(), c.radius());
                    break;
                }
                case Variant::eCircleF:
                {
                    CircleF c = *v_cast<CircleF>(data);
                    *s = UtilString::sprintf("CircleF(%g,%g,%g)", c.center().x(), c.center().y(), c.radius());
                    break;
                }
                case Variant::eSize:
                {
                    Size size = *v_cast<Size>(data);
                    *s = UtilString::sprintf("Size(%d,%d)", size.width(), size.height());
                    break;
                }
                case Variant::eSizeF:
                {
                    SizeF size = *v_cast<SizeF>(data);
                    *s = UtilString::sprintf("SizeF(%g,%g)", size.width(), size.height());
                    break;
                }
                case Variant::eLine:
                {
                    Line l = *v_cast<Line>(data);
                    *s = UtilString::sprintf("Line(%d,%d,%d,%d)", l.start().x(), l.start().y(), l.end().x(), l.end().y());
                    break;
                }
                case Variant::eLineF:
                {
                    LineF l = *v_cast<LineF>(data);
                    *s = UtilString::sprintf("LineF(%g,%g,%g,%g)", l.start().x(), l.start().y(), l.end().x(), l.end().y());
                    break;
                }
                case Variant::ePoint:
                {
                    Point p = *v_cast<Point>(data);
                    *s = UtilString::sprintf("Point(%d,%d)", p.x(), p.y());
                    break;
                }
                case Variant::ePointF:
                {
                    PointF p = *v_cast<PointF>(data);
                    *s = UtilString::sprintf("PointF(%g,%g)", p.x(), p.y());
                    break;
                }
                default:
                {
                    *s = UtilString();
                    return false;
                }
            }
            break;
        }
        default:
        {
            return false;
        }
    }

    return true;
}

static void construct(VariantPrivate::Data *x, const void *copy)
{
    switch (x->type) {
    case Variant::eString:
    {
        //set before v_construct
        x->isComplex = true;
        v_construct<UtilString>(x, copy);
        break;
    }
    case Variant::eRect:
    {
        //set before v_construct
        x->isComplex = true;
        v_construct<Rect>(x, copy);
        break;
    }
    case Variant::eRectF:
    {
        //set before v_construct
        x->isComplex = true;
        v_construct<RectF>(x, copy);
        break;
    }
    case Variant::eCircle:
    {
        //set before v_construct
        x->isComplex = true;
        v_construct<Circle>(x, copy);
        break;
    }
    case Variant::eCircleF:
    {
        //set before v_construct
        x->isComplex = true;
        v_construct<CircleF>(x, copy);
        break;
    }
    case Variant::eSize:
    {
        //set before v_construct
        x->isComplex = true;
        v_construct<Size>(x, copy);
        break;
    }
    case Variant::eSizeF:
    {
        //set before v_construct
        x->isComplex = true;
        v_construct<SizeF>(x, copy);
        break;
    }
    case Variant::eLine:
    {
        //set before v_construct
        x->isComplex = true;
        v_construct<Line>(x, copy);
        break;
    }
    case Variant::eLineF:
    {
        //set before v_construct
        x->isComplex = true;
        v_construct<LineF>(x, copy);
        break;
    }
    case Variant::ePoint:
    {
        //set before v_construct
        x->isComplex = true;
        v_construct<Point>(x, copy);
        break;
    }
    case Variant::ePointF:
    {
        //set before v_construct
        x->isComplex = true;
        v_construct<PointF>(x, copy);
        break;
    }
    case Variant::eVarHash:
    {
        //set before v_construct
        x->isComplex = true;
        v_construct<VarHash>(x, copy);
        break;
    }
    case Variant::eVarList:
    {
        //set before v_construct
        x->isComplex = true;
        v_construct<VarList>(x, copy);
        break;
    }
    case Variant::eBool:
    {
        x->data.b = copy ? *static_cast<const dbool *>(copy) : false;
        x->isComplex = false;
        break;
    }
    case Variant::eInt:
    {
        x->data.i = copy ? *static_cast<const dint *>(copy) : 0;
        x->isComplex = false;
        break;
    }
    case Variant::eUInt:
    {
        x->data.u = copy ? *static_cast<const duint *>(copy) : 0;
        x->isComplex = false;
        break;
    }
    case Variant::eLongLong:
    {
        x->data.ll = copy ? *static_cast<const dlonglong *>(copy) : 0;
        x->isComplex = false;
        break;
    }
    case Variant::eULongLong:
    {
        x->data.ull = copy ? *static_cast<const dlonglong *>(copy) : 0;
        x->isComplex = false;
        break;
    }
    case Variant::eDouble:
    {
        x->data.d = copy ? *static_cast<const double *>(copy) : 0;
        x->isComplex = false;
        break;
    }
    case Variant::eFloat:
    {
        x->data.f = copy ? *static_cast<const float *>(copy) : 0;
        x->isComplex = false;
        break;
    }
    case Variant::eChar:
    {
        x->data.c = copy ? *static_cast<const dchar *>(copy) : 0;
        x->isComplex = false;
        break;
    }
#ifdef DM_VARIANT_PTR
    case Variant::eVoidPtr:
    {
        x->data.ptr = copy ? *static_cast<void* const *>(copy) : 0;
        x->isComplex = false;
        break;
    }
#endif
    case MetaType::eObjectPtr:
    {
        x->data.ptr = copy ? *static_cast<Object* const *>(copy) : 0;
        x->isComplex = false;
        break;
    }
    default:
        void *ptr = MetaType::construct(x->type, copy);
        if (!ptr) {
            x->type = Variant::eInvalid;
            DM_ASSERT(ptr);
        } else {
            x->sharable = true;
            x->isComplex = true;
            x->data.ptr = ptr;
        }
        break;
    }
}

#if 0
static void test()
{
    Variant v = RectF(1.5f,2,3.4f,4);
    UtilString rrr = v.toString();

    int begin = rrr.indexOf('(');
    int end = rrr.indexOf(')');

    l = rrr.subStr(begin + 1, end - begin - 1).split(' ');
    for (int i=0; i<l.size(); ++i)
    {
        DM_LOGI("%s", l.at(i).toCharStr());
    }
    RectF r(l.at(0).toFloat(), l.at(1).toFloat(), l.at(2).toFloat(), l.at(3).toFloat());
}
#endif

DM_DLL_EXPORT dbool variant_cast_helper(const Variant &v, Variant::Type tp, void *ptr)
{
    return convert(v.pdm->d, tp, ptr, 0);
}

DM_DLL_EXPORT dbool variant_set_helper(Variant &v, Variant::Type tp, duint flag)
{
    v.pdm->d->type = tp;
    v.pdm->d->isComplex = flag;

    return true;
}

DM_END_NAMESPACE
