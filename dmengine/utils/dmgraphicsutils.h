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

#ifndef DMGRAPHICSUTILS_H
#define DMGRAPHICSUTILS_H
#include "dmnamespace.h"
DM_BEGIN_NAMESPACE
#ifdef DM_GRAPHICS_USE_REAL
class PointF;
class LineF;
class RectF;
class DM_DLL_EXPORT GraphicsUtils
{
public:
    class MoveListener
    {
    public:
        virtual void onMove(const PointF &pos, dreal left) = 0;
    };
    static PointF movePoint(const PointF &ptStart, const PointF &ptEnd, dreal distance, dreal &left);
    static void movePoint(const PointF &ptStart, const PointF &ptEnd, dreal distance, MoveListener *listener);

    static dbool inRange(dreal x0, dreal y0, dreal x1, dreal y1, dreal range);
    static dbool inRange(const PointF &from, const PointF &target, dreal range);

private:
    static PointF movePointSimple(const PointF &ptStart, const PointF &ptEnd, dreal distance, dreal &left);
    static void movePointSimple(const PointF &ptStart, const PointF &ptEnd, dreal distance, MoveListener *listener);
};
#else
class Point;
class Line;
class Rect;
class GraphicsUtils
{
public:
    class MoveListener
    {
    public:
        virtual void onMove(const Point &pos, dint left) = 0;
    };
    static Point movePoint(const Point &ptStart, const Point &ptEnd, dint distance, dint &left);
    static void movePoint(const Point &ptStart, const Point &ptEnd, dint distance, MoveListener *listener);

    static dbool inRange(dint x0, dint y0, dint x1, dint y1, dint range);
    static dbool inRange(const Point &from, const Point &target, dint range);

private:
    static Point movePointSimple(const Point &ptStart, const Point &ptEnd, dint distance, dint &left);
    static void movePointSimple(const Point &ptStart, const Point &ptEnd, dint distance, MoveListener *listener);
};
#endif
DM_END_NAMESPACE
#endif // DMGRAPHICSUTILS_H
