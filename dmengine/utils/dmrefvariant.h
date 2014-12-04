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
#ifndef DMREFVARIANT_H
#define DMREFVARIANT_H
#include "dmnamespace.h"
DM_BEGIN_NAMESPACE
class DM_DLL_EXPORT RefVariant
{
public:
    RefVariant(); 
    RefVariant(dbool v);
    RefVariant(dint v);
    RefVariant(duint v);
    RefVariant(dlonglong v);
    RefVariant(dulonglong v);
    RefVariant(double v);
    RefVariant(float v);
    RefVariant(dchar v);
    RefVariant(const Rect &v);
    RefVariant(const RectF &v);
    RefVariant(const Size &v);
    RefVariant(const SizeF &v);
    RefVariant(const Line &v);
    RefVariant(const LineF &v);
    RefVariant(const Point &v);
    RefVariant(const PointF &v);
    virtual ~RefVariant();

    dbool isValid() const;
    dbool isNull()const;
    Type type() const;

    dbool toBool(dbool *ok = NULL) const;
    dint toInt(dbool *ok = NULL) const;
    duint toUInt(dbool *ok = NULL) const;
    dlonglong toLongLong(dbool *ok = NULL) const;
    dulonglong toULongLong(dbool *ok = NULL) const;
    double toDouble(dbool *ok = NULL) const;
    float toFloat(dbool *ok = NULL) const;
    dchar toChar(dbool *ok = NULL) const;
    Rect toRect(dbool *ok = NULL) const;
    RectF toRectF(dbool *ok = NULL) const;
    Size toSize(dbool *ok = NULL) const;
    SizeF toSizeF(dbool *ok = NULL) const;
    Line toLine(dbool *ok = NULL) const;
    LineF toLineF(dbool *ok = NULL) const;
    Point toPoint(dbool *ok = NULL) const;
    PointF toPointF(dbool *ok = NULL) const;

    dbool operator==(const RefVariant &other) const;
    dbool operator!=(const RefVariant &other) const
    { return !(*this == other); }
    RefVariant& operator=(const RefVariant &other);
};
DMREFVARIANT_H
#endif // DMVARIANT_H
