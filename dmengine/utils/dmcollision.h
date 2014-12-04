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
#ifndef DMCOLLISION_H
#define DMCOLLISION_H
#include "dmgraphicsmath.h"
#include "dmrect.h"
#include "dmcircle.h"

DM_BEGIN_NAMESPACE
#ifdef DM_GRAPHICS_USE_REAL
class PointF;
class LineF;
class Collision
{
public:
    inline static dbool isCollision(const RectF &rect, const PointF &point);
    inline static dbool isCollision(const PointF &point, const RectF &rect);
    inline static dbool isCollision(const RectF &rect1, const RectF &rect2);
    inline static dbool isCollision(const RectF &rect, const CircleF &circle);
    inline static dbool isCollision(const CircleF &circle, const RectF &rect);
    inline static dbool isCollision(const CircleF &circle, const PointF &point);
    inline static dbool isCollision(const PointF &point, const CircleF &circle);
    inline static dbool isCollision(const CircleF &circle1, const CircleF &circle2);
    inline static dbool isCollision(const RectF &rect, const LineF &line);
    inline static dbool isCollision(const LineF &line, const RectF &rect);
};

inline dbool Collision::isCollision(const RectF &rect, const PointF &point)
{
    return rect.contains(point);
}

inline dbool Collision::isCollision(const PointF &point, const RectF &rect)
{
    return rect.contains(point);
}

inline dbool Collision::isCollision(const RectF &rect1, const RectF &rect2)
{
    return rect1.intersects(rect2);
}

inline dbool Collision::isCollision(const RectF &rect, const CircleF &circle)
{
    return circle.intersects(rect);
}

inline dbool Collision::isCollision(const CircleF &circle, const RectF &rect)
{
    return circle.intersects(rect);
}

inline dbool Collision::isCollision(const CircleF &circle, const PointF &point)
{
    return circle.contains(point);
}

inline dbool Collision::isCollision(const PointF &point, const CircleF &circle)
{
    return circle.contains(point);
}

inline dbool Collision::isCollision(const CircleF &circle1, const CircleF &circle2)
{
    return circle1.intersects(circle2);
}

inline dbool Collision::isCollision(const RectF &rect, const LineF &line)
{
    return GraphicsMath::lineIntersectRectF(line, rect);
}

inline dbool Collision::isCollision(const LineF &line, const RectF &rect)
{
    return GraphicsMath::lineIntersectRectF(line, rect);
}
#else
class Point;
class Line;
class Collision
{
public:
    inline static dbool isCollision(const Rect &rect, const Point &point);
    inline static dbool isCollision(const Point &point, const Rect &rect);
    inline static dbool isCollision(const Rect &rect1, const Rect &rect2);
    inline static dbool isCollision(const Rect &rect, const Circle &circle);
    inline static dbool isCollision(const Circle &circle, const Rect &rect);
    inline static dbool isCollision(const Circle &circle, const Point &point);
    inline static dbool isCollision(const Point &point, const Circle &circle);
    inline static dbool isCollision(const Circle &circle1, const Circle &circle2);
    inline static dbool isCollision(const Rect &rect, const Line &line);
    inline static dbool isCollision(const Line &line, const Rect &rect);
};

inline dbool Collision::isCollision(const Rect &rect, const Point &point)
{
    return rect.contains(point);
}

inline dbool Collision::isCollision(const Point &point, const Rect &rect)
{
    return rect.contains(point);
}

inline dbool Collision::isCollision(const Rect &rect1, const Rect &rect2)
{
    return rect1.intersects(rect2);
}

inline dbool Collision::isCollision(const Rect &rect, const Circle &circle)
{
    return circle.intersects(rect);
}

inline dbool Collision::isCollision(const Circle &circle, const Rect &rect)
{
    return circle.intersects(rect);
}

inline dbool Collision::isCollision(const Circle &circle, const Point &point)
{
    return circle.contains(point);
}

inline dbool Collision::isCollision(const Point &point, const Circle &circle)
{
    return circle.contains(point);
}

inline dbool Collision::isCollision(const Circle &circle1, const Circle &circle2)
{
    return circle1.intersects(circle2);
}

inline dbool Collision::isCollision(const Rect &rect, const Line &line)
{
    //return GraphicsMath::isLineIntersectRect(line, rect);
    return GraphicsMath::lineIntersectRect(line, rect);
}

inline dbool Collision::isCollision(const Line &line, const Rect &rect)
{
    //return GraphicsMath::isLineIntersectRect(line, rect);
    return GraphicsMath::lineIntersectRect(line, rect);
}
#endif
DM_END_NAMESPACE

#endif //DMCOLLISION_H
