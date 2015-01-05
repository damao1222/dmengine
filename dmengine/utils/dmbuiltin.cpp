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
#include "dmbuiltin.h"
#include "dmrect.h"
#include "dmcircle.h"
#include "dmsize.h"
#include "dmline.h"
#include "dmpoint.h"
#include "dmutilstring.h"
#include "dmvariant.h"

DM_BEGIN_NAMESPACE
template <typename T>
inline void set_cast(void *dest, const Variant &v)
{
    *static_cast<T*>(dest) = v.value<T>();
}

dbool builtin_property_setter(void *pDest, int type, const Variant &value)
{
    if (pDest)
    {
        switch (type)
        {
            case Variant::eBool:
            {
                set_cast<dbool>(pDest, value);
                break;
            }
            case Variant::eInt:
            {
                set_cast<dint>(pDest, value);
                break;
            }
            case Variant::eUInt:
            {
                set_cast<duint>(pDest, value);
                break;
            }
            case Variant::eLongLong:
            {
                set_cast<dlonglong>(pDest, value);
                break;
            }
            case Variant::eULongLong:
            {
                set_cast<dulonglong>(pDest, value);
                break;
            }
            case Variant::eDouble:
            {
                set_cast<double>(pDest, value);
                break;
            }
            case Variant::eFloat:
            {
                set_cast<float>(pDest, value);
                break;
            }
            case Variant::eChar:
            {
                set_cast<dchar>(pDest, value);
                break;
            }
#ifdef DM_VARIANT_PTR
            case Variant::eVoidPtr:
            {
                set_cast<void*>(pDest, value);
                break;
            }
#endif
            case Variant::eRect:
            {
                set_cast<Rect>(pDest, value);
                break;
            }
            case Variant::eRectF:
            {
                set_cast<RectF>(pDest, value);
                break;
            }
            case Variant::eCircle:
            {
                set_cast<Circle>(pDest, value);
                break;
            }
            case Variant::eCircleF:
            {
                set_cast<CircleF>(pDest, value);
                break;
            }
            case Variant::eSize:
            {
                set_cast<Size>(pDest, value);
                break;
            }
            case Variant::eSizeF:
            {
                set_cast<SizeF>(pDest, value);
                break;
            }
            case Variant::eLine:
            {
                set_cast<Line>(pDest, value);
                break;
            }
            case Variant::eLineF:
            {
                set_cast<LineF>(pDest, value);
                break;
            }
            case Variant::ePoint:
            {
                set_cast<Point>(pDest, value);
                break;
            }
            case Variant::ePointF:
            {
                set_cast<PointF>(pDest, value);
                break;
            }
            case Variant::eString:
            {
                set_cast<UtilString>(pDest, value);
                break;
            }
            case MetaType::eVariant:
            {
                set_cast<Variant>(pDest, value);
                break;
            }
            default:
            {
                return false;
            }
        }
    }
    return false;
}

DM_END_NAMESPACE