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

#include "dmgraphicsutils.h"
#include "dmpoint.h"
#include "dmline.h"

DM_BEGIN_NAMESPACE
#ifdef DM_GRAPHICS_USE_REAL
PointF GraphicsUtils::movePointSimple(const PointF &ptStart, const PointF &ptEnd, dreal distance, dreal &left)
{
    if (distance <= 0)
    {
        left = -1;
        return PointF(ptStart);
    }

    dbool horizontal = IsEqual(ptStart.x(), ptEnd.x()) ? false : true;

    if (horizontal)
    {
        dreal x = Abs(ptEnd.x() - ptStart.x());
        if (x <= distance)
        {
            left = 0;
            return ptEnd;
        }

        if (ptEnd.x() < ptStart.x())
        {
            left = x;
            return PointF(ptStart.x() - distance, ptStart.y());
        }
        else
        {
            left = x;
            return PointF(ptStart.x() + distance, ptStart.y());
        }
    }
    else
    {
        dreal y = Abs(ptEnd.y() - ptStart.y());
        if (y <= distance)
        {
            left = 0;
            return ptEnd;
        }

        if (ptEnd.y() < ptStart.y())
        {
            left = y;
            return PointF(ptStart.x(), ptStart.y() - distance);
        }
        else
        {
            left = y;
            return PointF(ptStart.x(), ptStart.y() + distance);
        }
    }
}

void GraphicsUtils::movePointSimple(const PointF &ptStart, const PointF &ptEnd, dreal distance, MoveListener *listener)
{
    DM_ASSERT(listener);
    if (distance <= 0)
    {
        listener->onMove(ptStart, -1);
        return ;
    }

    dbool horizontal = IsEqual(ptStart.x(), ptEnd.x()) ? false : true;

    if (horizontal)
    {
        dreal x = Abs(ptEnd.x() - ptStart.x());
        if (x <= distance)
        {
            listener->onMove(ptEnd, 0);
            return ;
        }

        if (ptEnd.x() < ptStart.x())
        {
            listener->onMove(PointF(ptStart.x() - distance, ptStart.y()), x);
            return;
        }
        else
        {
            listener->onMove(PointF(ptStart.x() + distance, ptStart.y()), x);
            return ;
        }
    }
    else
    {
        dreal y = Abs(ptEnd.y() - ptStart.y());
        if (y <= distance)
        {
            listener->onMove(ptEnd, 0);
            return ;
        }

        if (ptEnd.y() < ptStart.y())
        {
            listener->onMove(PointF(ptStart.x(), ptStart.y() - distance), y);
            return ;
        }
        else
        {
            listener->onMove(PointF(ptStart.x(), ptStart.y() + distance), y);
            return ;
        }
    }
}

PointF GraphicsUtils::movePoint(const PointF &ptStart, const PointF &ptEnd, dreal distance, dreal &left)
{
    if (IsEqual(ptStart.x(), ptEnd.x()) || IsEqual(ptStart.y(), ptEnd.y()))
    {
        return movePointSimple(ptStart, ptEnd, distance, left);
    }

    if (distance <= 0)
    {
        left = -1;
        return PointF(ptStart);
    }
    dreal nDistance = LineF::distance(ptStart, ptEnd);
    if (nDistance <= distance)
    {
        left = 0;
        return PointF(ptEnd);
    }
    else
    {
        PointF current(ptStart);
        dreal fScale = distance / nDistance;
        current += PointF(((ptEnd.x()-ptStart.x())*fScale), \
                          ((ptEnd.y()-ptStart.y())*fScale));
        left = nDistance - distance;
        return current;
    }
}

void GraphicsUtils::movePoint(const PointF &ptStart, const PointF &ptEnd, dreal distance, MoveListener *listener)
{
    if (IsEqual(ptStart.x(), ptEnd.x()) || IsEqual(ptStart.y(), ptEnd.y()))
    {
        movePointSimple(ptStart, ptEnd, distance, listener);
        return ;
    }

    DM_ASSERT(listener);
    if (distance <= 0)
    {
        listener->onMove(ptStart, -1);
        return ;
    }

    dint nDistance = LineF::distance(ptStart, ptEnd);
    if (nDistance <= distance)
    {
        listener->onMove(ptEnd, 0);
        return ;
    }
    else
    {
        PointF current(ptStart);
        dreal fScale = distance / nDistance;
        current += PointF(((ptEnd.x()-ptStart.x())*fScale), \
                          ((ptEnd.y()-ptStart.y())*fScale));
        listener->onMove(current, nDistance - distance);
        return ;
    }
}

dbool GraphicsUtils::inRange(dreal x0, dreal y0, dreal x1, dreal y1, dreal range)
{
    dreal distance = LineF::distance(x0, y0, x1, y1);
    return distance > range ? false : true;
}

dbool GraphicsUtils::inRange(const PointF &from, const PointF &target, dreal range)
{
    dreal distance = LineF::distance(from, target);
    return distance > range ? false : true;
}
#else
Point GraphicsUtils::movePointSimple(const Point &ptStart, const Point &ptEnd, dint distance, dint &left)
{
    if (distance <= 0)
    {
        left = -1;
        return Point(ptStart);
    }

    dbool horizontal = ptStart.x() == ptEnd.x() ? false : true;

    if (horizontal)
    {
        dint x = Abs<int>(ptEnd.x() - ptStart.x());
        if (x <= distance)
        {
            left = 0;
            return ptEnd;
        }

        if (ptEnd.x() < ptStart.x())
        {
            left = x;
            return Point(ptStart.x() - distance, ptStart.y());
        }
        else
        {
            left = x;
            return Point(ptStart.x() + distance, ptStart.y());
        }
    }
    else
    {
        dint y = Abs<int>(ptEnd.y() - ptStart.y());
        if (y <= distance)
        {
            left = 0;
            return ptEnd;
        }

        if (ptEnd.y() < ptStart.y())
        {
            left = y;
            return Point(ptStart.x(), ptStart.y() - distance);
        }
        else
        {
            left = y;
            return Point(ptStart.x(), ptStart.y() + distance);
        }
    }
}

void GraphicsUtils::movePointSimple(const Point &ptStart, const Point &ptEnd, dint distance, MoveListener *listener)
{
    DM_ASSERT(listener);
    if (distance <= 0)
    {
        listener->onMove(ptStart, -1);
        return ;
    }

    dbool horizontal = ptStart.x() == ptEnd.x() ? false : true;

    if (horizontal)
    {
        dint x = Abs<int>(ptEnd.x() - ptStart.x());
        if (x <= distance)
        {
            listener->onMove(ptEnd, 0);
            return ;
        }

        if (ptEnd.x() < ptStart.x())
        {
            listener->onMove(Point(ptStart.x() - distance, ptStart.y()), x);
            return;
        }
        else
        {
            listener->onMove(Point(ptStart.x() + distance, ptStart.y()), x);
            return ;
        }
    }
    else
    {
        dint y = Abs<int>(ptEnd.y() - ptStart.y());
        if (y <= distance)
        {
            listener->onMove(ptEnd, 0);
            return ;
        }

        if (ptEnd.y() < ptStart.y())
        {
            listener->onMove(Point(ptStart.x(), ptStart.y() - distance), y);
            return ;
        }
        else
        {
            listener->onMove(Point(ptStart.x(), ptStart.y() + distance), y);
            return ;
        }
    }
}

Point GraphicsUtils::movePoint(const Point &ptStart, const Point &ptEnd, dint distance, dint &left)
{
    if (ptStart.x() == ptEnd.x() || ptStart.y() == ptEnd.y())
    {
        return movePointSimple(ptStart, ptEnd, distance, left);
    }

    if (distance <= 0)
    {
        left = -1;
        return Point(ptStart);
    }
    dint nDistance = Line::distance(ptStart, ptEnd);
    if (nDistance <= distance)
    {
        left = 0;
        return Point(ptEnd);
    }
    else
    {
        Point current(ptStart);
        dreal fScale = static_cast<dreal>(distance) / nDistance;
        current += Point(static_cast<dint>((ptEnd.x()-ptStart.x())*fScale), \
                         static_cast<dint>((ptEnd.y()-ptStart.y())*fScale));
        left = nDistance - distance;
        return current;
    }
}

void GraphicsUtils::movePoint(const Point &ptStart, const Point &ptEnd, dint distance, MoveListener *listener)
{
    if (ptStart.x() == ptEnd.x() || ptStart.y() == ptEnd.y())
    {
        movePointSimple(ptStart, ptEnd, distance, listener);
        return ;
    }

    DM_ASSERT(listener);
    if (distance <= 0)
    {
        listener->onMove(ptStart, -1);
        return ;
    }

    dint nDistance = Line::distance(ptStart, ptEnd);
    if (nDistance <= distance)
    {
        listener->onMove(ptEnd, 0);
        return ;
    }
    else
    {
        Point current(ptStart);
        dreal fScale = static_cast<dreal>(distance) / nDistance;
        current += Point(static_cast<dint>((ptEnd.x()-ptStart.x())*fScale), \
                         static_cast<dint>((ptEnd.y()-ptStart.y())*fScale));
        listener->onMove(current, nDistance - distance);
        return ;
    }
}

dbool GraphicsUtils::inRange(dint x0, dint y0, dint x1, dint y1, dint range)
{
    dint nDistance = Line::distance(x0, y0, x1, y1);
    return nDistance > range ? false : true;
}

dbool GraphicsUtils::inRange(const Point &from, const Point &target, dint range)
{
    dint nDistance = Line::distance(from, target);
    return nDistance > range ? false : true;
}

#endif
DM_END_NAMESPACE
