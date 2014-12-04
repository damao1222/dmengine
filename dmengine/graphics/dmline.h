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
#ifndef DMLINE_H
#define DMLINE_H

#include "dmpoint.h"
#include "dmmath.h"

DM_BEGIN_NAMESPACE
class DM_DLL_EXPORT Line
{
public:
    Line()
    {}

    Line(dint nX0, dint nY0, dint nX1, dint nY1):
        mP0(nX0, nY0),
        mP1(nX1, nY1)
    {}

    Line(const Point &p0, const Point &p1):
        mP0(p0),
        mP1(p1)
    {}

    Line(const Line &other):
        mP0(other.mP0),
        mP1(other.mP1)
    {}

    dbool isEmpty() const;
    dbool isNull() const;

    const Point& start() const
    {
        return mP0;
    }

    const Point& end() const
    {
        return mP1;
    }

    Point center() const
    {
        dint x = mP0.x() + ((mP1.x()-mP0.x())>>1);
        dint y = mP0.y() + ((mP1.y()-mP0.y())>>1);
        return Point(x, y);
    }

    inline void setCenter(const Point &pos);

    dint length() const
    {
        return distance(mP0, mP1);
    }

    dreal radian() const;

    void getMathLine(dreal &a, dreal &b, dreal &c);

    dreal pointDistance(const Point &p);

    static dint distance(const Point &p0, const Point &p1);
    static dint distance(dint nX0, dint nY0, dint nX1, dint nY1);

    static dreal radian(const Point &p0, const Point &p1);
    static dreal radian(dint nX0, dint nY0, dint nX1, dint nY1);

    dbool operator==(const Line &other) const
    {
        if (mP0 == other.mP0 && \
            mP1 == other.mP1)
            return true;
        else
            return false;
    }

    dbool operator!=(const Line &other) const
    {
        return !(*this == other);
    }

    Line& operator=(const Line &other)
    {
        mP0 = other.mP0;
        mP1 = other.mP1;

        return (*this);
    }

    static dbool simpleHitLine( const Point& pt, const Point& ptStart, const Point& ptEnd);

    friend class LineF;

private:
    Point mP0;
    Point mP1;
};

DM_DECLARE_TYPEINFO(Line, DM_MOVABLE_TYPE);

inline dbool Line::isEmpty() const
{
    return isNull();
}

inline dbool Line::isNull() const
{
    return mP0 == mP1;
}

inline void Line::setCenter(const Point &pos)
{
    Point p = center();
    dint x = pos.x() - p.x();
    dint y = pos.y() - p.y();
    
    mP0.setX(mP0.x() + x);
    mP0.setY(mP0.y() + y);

    mP1.setX(mP1.x() + x);
    mP1.setY(mP1.y() + y);
}

inline dreal Line::radian() const
{
    return Atan2(static_cast<dreal>(mP1.y() - mP0.y()), static_cast<dreal>(mP1.x() - mP0.x()));
}


class DM_DLL_EXPORT LineF
{
public:
    LineF()
    {}

    LineF(dreal nX0, dreal nY0, dreal nX1, dreal nY1):
        mP0(nX0, nY0),
        mP1(nX1, nY1)
    {}

    LineF(const PointF &p0, const PointF &p1):
        mP0(p0),
        mP1(p1)
    {}

    LineF(const Line &line):
    mP0(line.mP0),
    mP1(line.mP1)
    {}

    LineF(const LineF &other):
        mP0(other.mP0),
        mP1(other.mP1)
    {}

    dbool isEmpty() const;
    dbool isNull() const;

    const PointF& start() const
    {
        return mP0;
    }

    const PointF& end() const
    {
        return mP1;
    }

    PointF center() const
    {
        dreal x = mP0.x() + (mP1.x()-mP0.x())/2;
        dreal y = mP0.y() + (mP1.y()-mP0.y())/2;
        return PointF(x, y);
    }

    inline void setCenter(const PointF &pos);

    dreal length() const
    {
        return distance(mP0, mP1);
    }

    Line toLine() const;

    dreal radian() const;

    void getMathLine(dreal &a, dreal &b, dreal &c);

    dreal pointDistance(const PointF &p);

    static dreal distance(const PointF &p0, const PointF &p1);
    static dreal distance(dreal nX0, dreal nY0, dreal nX1, dreal nY1);

    static dreal radian(const PointF &p0, const PointF &p1);
    static dreal radian(dreal nX0, dreal nY0, dreal nX1, dreal nY1);

    dbool operator==(const LineF &other) const
    {
        if (mP0 == other.mP0 && \
            mP1 == other.mP1)
            return true;
        else
            return false;
    }

    dbool operator!=(const LineF &other) const
    {
        return !(*this == other);
    }

    LineF& operator=(const LineF &other)
    {
        mP0 = other.mP0;
        mP1 = other.mP1;

        return (*this);
    }

    static dbool hitLine( const PointF &pt, const PointF &ptStart, const PointF &ptEnd, int nNear);
    static dbool hitLine( const PointF &pt, const LineF &line, int nNear);
    
private:
    PointF mP0;
    PointF mP1;
};

DM_DECLARE_TYPEINFO(LineF, DM_MOVABLE_TYPE);

inline dbool LineF::isEmpty() const
{
    return isNull();
}

inline dbool LineF::isNull() const
{
    return mP0 == mP1;
}

inline void LineF::setCenter(const PointF &pos)
{
    PointF p = center();
    dreal x = pos.x() - p.x();
    dreal y = pos.y() - p.y();
    
    mP0.setX(mP0.x() + x);
    mP0.setY(mP0.y() + y);

    mP1.setX(mP1.x() + x);
    mP1.setY(mP1.y() + y);
}

inline Line LineF::toLine() const
{
    Line l;
    l.mP0 = mP0.toPoint();
    l.mP1 = mP1.toPoint();
    return l;
}

inline dreal LineF::radian() const
{
    return radian(mP0, mP1);
}
DM_END_NAMESPACE
#endif // DMLINE_H
