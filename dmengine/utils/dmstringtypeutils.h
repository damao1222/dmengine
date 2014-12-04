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
#ifndef DMSTRINGTYPEUTILS_H
#define DMSTRINGTYPEUTILS_H
#include "dmnamespace.h"
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
class DM_DLL_EXPORT StringUtils
{
public:
    static int type(const UtilString &str);
    static Point toPoint(const UtilString &str);
    static PointF toPointF(const UtilString &str);
    static Size toSize(const UtilString &str);
    static SizeF toSizeF(const UtilString &str);
    static Line toLine(const UtilString &str);
    static LineF toLineF(const UtilString &str);
    static Rect toRect(const UtilString &str);
    static RectF toRectF(const UtilString &str);
    static Circle toCircle(const UtilString &str);
    static CircleF toCircleF(const UtilString &str);
};
DM_END_NAMESPACE
#endif // DMSTRINGTYPEUTILS_H
