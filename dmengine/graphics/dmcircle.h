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

#ifndef DMCIRCLE_H
#define DMCIRCLE_H
#include "dmnamespace.h"
#include "dmpoint.h"
#include "dmrect.h"
#include "dmline.h"

DM_BEGIN_NAMESPACE
class DM_DLL_EXPORT Circle
{
public:
    inline Circle(): r(0) {}
    inline Circle(int x, int y, int radius): p(x, y), r(radius) {}
    inline Circle(const Point &center, int radius): p(center), r(radius) {}
    inline Circle(const Circle &other): p(other.p), r(other.r) {}

    dbool isEmpty() const;
    dbool isNull() const;

    inline Rect boundingRect() const;
    inline Point center() const;
    inline dint radius() const;
    inline void setCenter(const Point &pos);

    dbool operator==(const Circle &other) const
    {
        if (p == other.p && \
            r == other.r)
            return true;
        else
            return false;
    }

    dbool operator!=(const Circle &other) const
    {
        return !(*this == other);
    }

    Circle& operator=(const Circle &other)
    {
        p = other.p;
        r = other.r;

        return (*this);
    }

    dbool intersects(const Rect &rect) const;
    inline dbool intersects(const Circle &other) const;
    inline dbool contains(const Point &p) const;

    friend class CircleF;
private:
    //center
    Point p;
    //radius
    int r;
};

DM_DECLARE_TYPEINFO(Circle, DM_MOVABLE_TYPE);

inline dbool Circle::isEmpty() const
{
    return isNull();
}

inline dbool Circle::isNull() const
{
    return r == 0;
}

inline Rect Circle::boundingRect() const
{
    return Rect(p.x()-r, p.y()-r, r>>1, r>>1);
}

inline Point Circle::center() const
{
    return Point(p);
}

inline dint Circle::radius() const
{
    return r;
}

inline void Circle::setCenter(const Point &pos)
{
    p = pos;
}

inline dbool Circle::intersects(const Circle &other) const
{
    dint len = Line::distance(p, other.p);
    return len > (r + other.r) ? false : true;
}

inline dbool Circle::contains(const Point &p) const
{
    dint len = Line::distance(this->p, p);
    return len > r ? false : true;
}

class DM_DLL_EXPORT CircleF
{
public:
    inline CircleF(): r(0) {}
    inline CircleF(dreal x, dreal y, dreal radius): p(x, y), r(radius) {}
    inline CircleF(const PointF &center, dreal radius): p(center), r(radius) {}
    inline CircleF(const Circle &other): p(other.p), r(static_cast<dreal>(other.r)) {}
    inline CircleF(const CircleF &other): p(other.p), r(other.r) {}

    dbool isEmpty() const;
    dbool isNull() const;

    inline RectF boundingRect() const;
    inline PointF center() const;
    inline dreal radius() const;
    inline void setCenter(const PointF &pos);

    inline Circle toCircle() const;

    dbool operator==(const CircleF &other) const
    {
        if (p == other.p && \
            r == other.r)
            return true;
        else
            return false;
    }

    dbool operator!=(const CircleF &other) const
    {
        return !(*this == other);
    }

    CircleF& operator=(const CircleF &other)
    {
        p = other.p;
        r = other.r;

        return (*this);
    }

    CircleF& operator=(const Circle &other)
    {
        p = other.p;
        r = static_cast<dreal>(other.r);

        return (*this);
    }

    dbool intersects(const RectF &rect) const;
    inline dbool intersects(const CircleF &other) const;
    inline dbool contains(const PointF &p) const;
private:
    //center
    PointF p;
    //radius
    dreal r;
};

DM_DECLARE_TYPEINFO(CircleF, DM_MOVABLE_TYPE);

inline dbool CircleF::isEmpty() const
{
    return isNull();
}

inline dbool CircleF::isNull() const
{
    return IsNull(r);
}

inline RectF CircleF::boundingRect() const
{
    return RectF(p.x()-r, p.y()-r, r/2, r/2);
}

inline PointF CircleF::center() const
{
    return PointF(p);
}

inline dreal CircleF::radius() const
{
    return r;
}

inline void CircleF::setCenter(const PointF &pos)
{
    p = pos;
}

inline Circle CircleF::toCircle() const
{
    Circle c;
    c.p = p.toPoint();
    c.r = static_cast<dint>(r);
    return c;
}

inline dbool CircleF::intersects(const CircleF &other) const
{
    dreal len = LineF::distance(p, other.p);
    return len > (r + other.r) ? false : true;
}

inline dbool CircleF::contains(const PointF &p) const
{
    dreal len = LineF::distance(this->p, p);
    return len > r ? false : true;
}


DM_END_NAMESPACE

#endif // DMCIRCLE_H
