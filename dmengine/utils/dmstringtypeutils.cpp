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
#include "dmstringtypeutils.h"
#include "dmrect.h"
#include "dmcircle.h"
#include "dmsize.h"
#include "dmline.h"
#include "dmpoint.h"
#include "dmutilstring.h"
#include "dmvariant.h"
#include "dmlogger.h"

DM_BEGIN_NAMESPACE

static Variant::Type GetType(const UtilString &str, dint rpos)
{
    DM_ASSERT(rpos > 0);
    if (!str.isEmpty())
    {
        dbool real = str.at(rpos) == 'F';
        switch (str.at(0))
        {
        case 'R':
            return real ? Variant::eRectF : Variant::eRect;
        case 'C':
            return real ? Variant::eCircleF : Variant::eCircle;
        case 'S':
            return real ? Variant::eSizeF : Variant::eSize;
        case 'P':
            return real ? Variant::ePointF : Variant::ePoint;
        case 'L':
            return real ? Variant::eLineF : Variant::eLine;
        default:
            break;
        }
    }
    return Variant::eInvalid;
}

int StringUtils::type(const UtilString &str)
{
    const UtilString &s = str.trimmed();
    
    dint begin = s.indexOf('(');
    return GetType(str, begin - 1);
}

Point StringUtils::toPoint(const UtilString &str)
{
    const UtilString &s = str.trimmed();
    
    dint begin = s.indexOf('(');
    Variant::Type type = GetType(str, begin - 1);
    if (type == Variant::ePoint)
    {
        dint end = s.lastIndexOf(')');

        UtilStringList l = s.subStr(begin + 1, end - begin - 1).split(',');
        return Point(l.at(0).toInt(), l.at(1).toInt());
    }
    else if (type == Variant::ePointF)
    {
        dint end = s.lastIndexOf(')');

        UtilStringList l = s.subStr(begin + 1, end - begin - 1).split(',');
        return PointF(l.at(0).toFloat(), l.at(1).toFloat()).toPoint();
    }
    DM_FATAL("Error Type: %d", type);
    return Point();
}

PointF StringUtils::toPointF(const UtilString &str)
{
    const UtilString &s = str.trimmed();
    
    dint begin = s.indexOf('(');
    Variant::Type type = GetType(str, begin - 1);
    if (type == Variant::ePoint)
    {
        dint end = s.lastIndexOf(')');

        UtilStringList l = s.subStr(begin + 1, end - begin - 1).split(',');
        return PointF(l.at(0).toInt(), l.at(1).toInt());
    }
    else if (type == Variant::ePointF)
    {
        dint end = s.lastIndexOf(')');

        UtilStringList l = s.subStr(begin + 1, end - begin - 1).split(',');
        return PointF(l.at(0).toFloat(), l.at(1).toFloat());
    }
    DM_FATAL("Error Type: %d", type);
    return PointF();
}

Size StringUtils::toSize(const UtilString &str)
{
    const UtilString &s = str.trimmed();
    
    dint begin = s.indexOf('(');
    Variant::Type type = GetType(str, begin - 1);
    if (type == Variant::eSize)
    {
        dint end = s.lastIndexOf(')');

        UtilStringList l = s.subStr(begin + 1, end - begin - 1).split(',');
        return Size(l.at(0).toInt(), l.at(1).toInt());
    }
    else if (type == Variant::eSizeF)
    {
        dint end = s.lastIndexOf(')');

        UtilStringList l = s.subStr(begin + 1, end - begin - 1).split(',');
        return SizeF(l.at(0).toFloat(), l.at(1).toFloat()).toSize();
    }
    DM_FATAL("Error Type: %d", type);
    return Size();
}

SizeF StringUtils::toSizeF(const UtilString &str)
{
    const UtilString &s = str.trimmed();
    
    dint begin = s.indexOf('(');
    Variant::Type type = GetType(str, begin - 1);
    if (type == Variant::eSize)
    {
        dint end = s.lastIndexOf(')');

        UtilStringList l = s.subStr(begin + 1, end - begin - 1).split(',');
        return SizeF(l.at(0).toInt(), l.at(1).toInt());
    }
    else if (type == Variant::eSizeF)
    {
        dint end = s.lastIndexOf(')');

        UtilStringList l = s.subStr(begin + 1, end - begin - 1).split(',');
        return SizeF(l.at(0).toFloat(), l.at(1).toFloat());
    }
    DM_FATAL("Error Type: %d", type);
    return SizeF();
}

Line StringUtils::toLine(const UtilString &str)
{
    const UtilString &s = str.trimmed();
    
    dint begin = s.indexOf('(');
    Variant::Type type = GetType(str, begin - 1);
    if (type == Variant::eLine)
    {
        dint end = s.lastIndexOf(')');

        UtilStringList l = s.subStr(begin + 1, end - begin - 1).split(',');
        return Line(l.at(0).toInt(), l.at(1).toInt(), l.at(2).toInt(), l.at(3).toInt());
    }
    else if (type == Variant::eLineF)
    {
        dint end = s.lastIndexOf(')');

        UtilStringList l = s.subStr(begin + 1, end - begin - 1).split(',');
        return LineF(l.at(0).toFloat(), l.at(1).toFloat(), l.at(2).toFloat(), l.at(3).toFloat()).toLine();
    }
    DM_FATAL("Error Type: %d", type);
    return Line();
}

LineF StringUtils::toLineF(const UtilString &str)
{
    const UtilString &s = str.trimmed();
    
    dint begin = s.indexOf('(');
    Variant::Type type = GetType(str, begin - 1);
    if (type == Variant::eLine)
    {
        dint end = s.lastIndexOf(')');

        UtilStringList l = s.subStr(begin + 1, end - begin - 1).split(',');
        return LineF(l.at(0).toInt(), l.at(1).toInt(), l.at(2).toInt(), l.at(3).toInt());
    }
    else if (type == Variant::eLineF)
    {
        dint end = s.lastIndexOf(')');

        UtilStringList l = s.subStr(begin + 1, end - begin - 1).split(',');
        return LineF(l.at(0).toFloat(), l.at(1).toFloat(), l.at(2).toFloat(), l.at(3).toFloat());
    }
    DM_FATAL("Error Type: %d", type);
    return LineF();
}

Rect StringUtils::toRect(const UtilString &str)
{
    const UtilString &s = str.trimmed();
    
    dint begin = s.indexOf('(');
    Variant::Type type = GetType(str, begin - 1);
    if (type == Variant::eRect)
    {
        dint end = s.lastIndexOf(')');

        UtilStringList l = s.subStr(begin + 1, end - begin - 1).split(',');
        return Rect(l.at(0).toInt(), l.at(1).toInt(), l.at(2).toInt(), l.at(3).toInt());
    }
    else if (type == Variant::eRectF)
    {
        dint end = s.lastIndexOf(')');

        UtilStringList l = s.subStr(begin + 1, end - begin - 1).split(',');
        return RectF(l.at(0).toFloat(), l.at(1).toFloat(), l.at(2).toFloat(), l.at(3).toFloat()).toRect();
    }
    DM_FATAL("Error Type: %d", type);
    return Rect();
}

RectF StringUtils::toRectF(const UtilString &str)
{
    const UtilString &s = str.trimmed();
    
    dint begin = s.indexOf('(');
    Variant::Type type = GetType(str, begin - 1);
    if (type == Variant::eRect)
    {
        dint end = s.lastIndexOf(')');

        UtilStringList l = s.subStr(begin + 1, end - begin - 1).split(',');
        return RectF(l.at(0).toInt(), l.at(1).toInt(), l.at(2).toInt(), l.at(3).toInt());
    }
    else if (type == Variant::eRectF)
    {
        dint end = s.lastIndexOf(')');

        UtilStringList l = s.subStr(begin + 1, end - begin - 1).split(',');
        return RectF(l.at(0).toFloat(), l.at(1).toFloat(), l.at(2).toFloat(), l.at(3).toFloat());
    }
    DM_FATAL("Error Type: %d", type);
    return RectF();
}

Circle StringUtils::toCircle(const UtilString &str)
{
    const UtilString &s = str.trimmed();
    
    dint begin = s.indexOf('(');
    Variant::Type type = GetType(str, begin - 1);
    if (type == Variant::eCircle)
    {
        dint end = s.lastIndexOf(')');

        UtilStringList l = s.subStr(begin + 1, end - begin - 1).split(',');
        return Circle(l.at(0).toInt(), l.at(1).toInt(), l.at(2).toInt());
    }
    else if (type == Variant::eCircleF)
    {
        dint end = s.lastIndexOf(')');

        UtilStringList l = s.subStr(begin + 1, end - begin - 1).split(',');
        return CircleF(l.at(0).toFloat(), l.at(1).toFloat(), l.at(2).toFloat()).toCircle();
    }
    DM_FATAL("Error Type: %d", type);
    return Circle();
}

CircleF StringUtils::toCircleF(const UtilString &str)
{
    const UtilString &s = str.trimmed();
    
    dint begin = s.indexOf('(');
    Variant::Type type = GetType(str, begin - 1);
    if (type == Variant::eCircle)
    {
        dint end = s.lastIndexOf(')');

        UtilStringList l = s.subStr(begin + 1, end - begin - 1).split(',');
        return CircleF(l.at(0).toInt(), l.at(1).toInt(), l.at(2).toInt());
    }
    else if (type == Variant::eCircleF)
    {
        dint end = s.lastIndexOf(')');

        UtilStringList l = s.subStr(begin + 1, end - begin - 1).split(',');
        return CircleF(l.at(0).toFloat(), l.at(1).toFloat(), l.at(2).toFloat());
    }
    DM_FATAL("Error Type: %d", type);
    return CircleF();
}

DM_END_NAMESPACE