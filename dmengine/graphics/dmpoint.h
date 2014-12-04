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
#ifndef DMPOINT_H
#define DMPOINT_H

#include "dmnamespace.h"
#include "dmmath.h"

DM_BEGIN_NAMESPACE
class DM_DLL_EXPORT Point
{
public:
    Point():
        mX(0),
        mY(0)
    {
    }

    Point(dint nX, dint nY):
        mX(nX),
        mY(nY)
    {
    }

    Point(const Point& other):
        mX(other.mX),
        mY(other.mY)
    {
    }

    dbool isNull() const;

    dint x() const { return mX; }
    dint y() const { return mY; }

    void setX(dint nX)
    {
        mX = nX;
    }

    void setY(dint nY)
    {
        mY = nY;
    }

    void set(dint nX, dint nY)
    {
        setX(nX);
        setY(nY);
    }

    static Point bezier(const Point &p1, const Point &p2, const Point &p3, dreal t );

    Point& operator=(const Point& other)
    {
        mX = other.mX;
        mY = other.mY;

        return (*this);
    }

    dbool operator==(const Point &other) const
    {
        if (mX == other.mX &&           \
            mY == other.mY
        )
            return true;
        else
            return false;
    }

    dbool operator!=(const Point &other) const
    {
        return !(*this == other);
    }

    const Point& operator+=(const Point& other)
    {
        mX += other.mX;
        mY += other.mY;
        return (*this);
    }

    const Point& operator-=(const Point& other)
    {
        mX -= other.mX;
        mY -= other.mY;
        return (*this);
    }

    const Point& operator*=(const Point& other)
    {
        mX *= other.mX;
        mY *= other.mY;
        return (*this);
    }

    const Point& operator/=(const Point& other)
    {
        mX /= other.mX;
        mY /= other.mY;
        return (*this);
    }
#if 0
    friend inline bool operator==(const Point &p1, const Point &p2);
    friend inline bool operator!=(const Point &p1, const Point &p2);
#endif
    friend inline const Point operator+(const Point &, const Point &);
    friend inline const Point operator-(const Point &, const Point &);
    friend inline const Point operator*(const Point &, float);
    friend inline const Point operator*(float, const Point &);
    friend inline const Point operator*(const Point &, double);
    friend inline const Point operator*(double, const Point &);
    friend inline const Point operator*(const Point &, int);
    friend inline const Point operator*(int, const Point &);
    friend inline const Point operator-(const Point &);
    friend inline const Point operator/(const Point &, dreal);
    friend class Rect;
    friend class PointF;
private:
    dint mX;
    dint mY;
};

DM_DECLARE_TYPEINFO(Point, DM_MOVABLE_TYPE);

inline dbool Point::isNull() const
{ return mX == 0 && mY == 0; }

inline const Point operator+(const Point &p1, const Point &p2)
{ return Point(p1.mX+p2.mX, p1.mY+p2.mY); }

inline const Point operator-(const Point &p1, const Point &p2)
{ return Point(p1.mX-p2.mX, p1.mY-p2.mY); }

inline const Point operator*(const Point &p, float c)
{ return Point(Round(p.mX*c), Round(p.mY*c)); }

inline const Point operator*(const Point &p, double c)
{ return Point(Round(static_cast<dreal>(p.mX*c)), Round(static_cast<dreal>(p.mY*c))); }

inline const Point operator*(const Point &p, int c)
{ return Point(p.mX*c, p.mY*c); }

inline const Point operator*(float c, const Point &p)
{ return Point(Round(p.mX*c), Round(p.mY*c)); }

inline const Point operator*(double c, const Point &p)
{ return Point(Round(static_cast<dreal>(p.mX*c)), Round(static_cast<dreal>(p.mY*c))); }

inline const Point operator*(int c, const Point &p)
{ return Point(p.mX*c, p.mY*c); }

inline const Point operator-(const Point &p)
{ return Point(-p.mX, -p.mY); }

inline const Point operator/(const Point &p, dreal c)
{
    return Point(Round(p.mX/c), Round(p.mY/c));
}
#if 0
inline bool operator==(const Point &p1, const Point &p2)
{ return p1.mX == p2.mX && p1.mY == p2.mY; }

inline bool operator!=(const Point &p1, const Point &p2)
{ return p1.mX != p2.mX || p1.mY != p2.mY; }
#endif
class DM_DLL_EXPORT PointF
{
public:
    PointF():
        mX(0.0f),
        mY(0.0f)
    {
    }

    PointF(dreal fX, dreal fY):
        mX(fX),
        mY(fY)
    {
    }

    PointF(const PointF& other):
        mX(other.mX),
        mY(other.mY)
    {
    }

    PointF(const Point& point):
        mX(static_cast<dreal>(point.mX)),
        mY(static_cast<dreal>(point.mY))
    {
    }

    dbool isNull() const;

    dreal x() const { return mX; }
    dreal y() const { return mY; }

    void setX(dreal fX)
    {
        mX = fX;
    }

    void setY(dreal fY)
    {
        mY = fY;
    }

    void set(dreal fX, dreal fY)
    {
        setX(fX);
        setY(fY);
    }

    Point toPoint() const
    {
        Point p;
        p.mX = static_cast<dint>(mX);
        p.mY = static_cast<dint>(mY);
        return p;
    }

    PointF& operator=(const PointF& other)
    {
        mX = other.mX;
        mY = other.mY;

        return (*this);
    }

    PointF& operator=(const Point& point)
    {
        mX = static_cast<dreal>(point.mX);
        mY = static_cast<dreal>(point.mY);

        return (*this);
    }

    dbool operator==(const PointF& other) const
    {
        if (IsEqual(mX, other.mX) &&           \
            IsEqual(mY, other.mY)
        )
            return true;
        else
            return false;
    }

    dbool operator!=(const PointF& other) const
    {
        return !(*this == other);
    }

    const PointF& operator+=(const PointF& other)
    {
        mX += other.mX;
        mY += other.mY;
        return (*this);
    }

    const PointF& operator-=(const PointF& other)
    {
        mX -= other.mX;
        mY -= other.mY;
        return (*this);
    }

    const PointF& operator*=(const PointF& other)
    {
        mX *= other.mX;
        mY *= other.mY;
        return (*this);
    }

    const PointF& operator/=(const PointF& other)
    {
        mX /= other.mX;
        mY /= other.mY;
        return (*this);
    }
    friend inline const PointF operator+(const PointF &, const PointF &);
    friend inline const PointF operator-(const PointF &, const PointF &);
    friend inline const PointF operator*(const PointF &, float);
    friend inline const PointF operator*(float, const PointF &);
    friend inline const PointF operator*(const PointF &, double);
    friend inline const PointF operator*(double, const PointF &);
    friend inline const PointF operator*(const PointF &, int);
    friend inline const PointF operator*(int, const PointF &);
    friend inline const PointF operator-(const PointF &);
    friend inline const PointF operator/(const PointF &, dreal);
    friend class RectF;
    friend class Point;
private:
    dreal mX;
    dreal mY;
};

DM_DECLARE_TYPEINFO(PointF, DM_MOVABLE_TYPE);

inline dbool PointF::isNull() const
{ return IsNull(mX) && IsNull(mY); }

inline const PointF operator+(const PointF &p1, const PointF &p2)
{ return PointF(p1.mX+p2.mX, p1.mY+p2.mY); }

inline const PointF operator-(const PointF &p1, const PointF &p2)
{ return PointF(p1.mX-p2.mX, p1.mY-p2.mY); }

inline const PointF operator*(const PointF &p, float c)
{ return PointF(p.mX*c, p.mY*c); }

inline const PointF operator*(const PointF &p, double c)
{ return PointF(static_cast<dreal>(p.mX*c), static_cast<dreal>(p.mY*c)); }

inline const PointF operator*(const PointF &p, int c)
{ return PointF(p.mX*c, p.mY*c); }

inline const PointF operator*(float c, const PointF &p)
{ return PointF(p.mX*c, p.mY*c); }

inline const PointF operator*(double c, const PointF &p)
{ return PointF(static_cast<dreal>(p.mX*c), static_cast<dreal>(p.mY*c)); }

inline const PointF operator*(int c, const PointF &p)
{ return PointF(p.mX*c, p.mY*c); }

inline const PointF operator-(const PointF &p)
{ return PointF(-p.mX, -p.mY); }

inline const PointF operator/(const PointF &p, dreal c)
{
    return PointF(p.mX/c, p.mY/c);
}

template<class A, class B>
A convertPoint(const B &b)
{
    return A(b.x(), b.y());
}

DM_END_NAMESPACE
#endif // DMPOINT_H
