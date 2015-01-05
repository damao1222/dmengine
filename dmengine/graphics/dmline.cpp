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
#include "dmline.h"
DM_BEGIN_NAMESPACE
static dreal dist(const Point &p, dreal a, dreal b, dreal c)
{
    return Abs<dreal>((a*p.x()+b*p.y()+c)*dmRSqrt(a*a+b*b));
}

static dreal dist(const PointF &p, dreal a, dreal b, dreal c)
{
    return Abs<dreal>((a*p.x()+b*p.y()+c)*dmRSqrt(a*a+b*b));
}

//FIX: mP1.x() == mP0.x() mP1.y() == mP0.y()
dreal Line::pointDistance(const Point &p)
{
    dreal a, b, c;
    getMathLine(a, b, c);
    return dist(p, a, b ,c);
}

void Line::getMathLine(dreal &a, dreal &b, dreal &c)
{
    if (mP1.x() == mP0.x()) {
        a = 1;
        b = 0;
        c = -static_cast<dreal>(mP1.x());
    }
    else if (mP1.y() == mP0.y()) {
        a = 0;
        b = 1;
        c = -static_cast<dreal>(mP1.y());
    }
    else {
        a = static_cast<dreal>(mP1.y() - mP0.y()) / (mP1.x() - mP0.x());
        b = -1;
        c = -a*mP0.x() + mP0.y();
    }
}

dint Line::distance(const Point &p0, const Point &p1)
{
    dint nX = p0.x() - p1.x();
    dint nY = p0.y() - p1.y();

    return static_cast<dint>(dmSqrt(nX*nX+nY*nY));
}

dint Line::distance(dint nX0, dint nY0, dint nX1, dint nY1)
{
    dint nX = nX0 - nX1;
    dint nY = nY0 - nY1;

    return static_cast<dint>(dmSqrt(nX*nX+nY*nY));
}

dreal Line::radian(const Point &p0, const Point &p1)
{
    return Atan2(p1.y()-p0.y(), p1.x()-p0.x());
}

dreal Line::radian(dint nX0, dint nY0, dint nX1, dint nY1)
{
    return Atan2(nY1-nY0, nX1-nX0);
}

dbool Line::simpleHitLine( const Point& pt, const Point& ptStart, const Point& ptEnd)
{
    if (ptStart.x() == ptEnd.x()&&ptEnd.x() == pt.x())
    {
        if ((pt.y() > ptStart.y() && pt.y() < ptEnd.y()) || \
            (pt.y() > ptEnd.y() && pt.y() < ptStart.y()))
            return true;
    }
    else if (ptStart.y() == ptEnd.y()&&ptEnd.y() == pt.y())
    {
        if ((pt.x() > ptStart.x() && pt.x() < ptEnd.x()) || \
            (pt.x() > ptEnd.x() && pt.x() < ptStart.x()))
            return true;
    }

    return false;
}

dreal LineF::pointDistance(const PointF &p)
{
    dreal a, b, c;
    getMathLine(a, b, c);
    return dist(p, a, b ,c);
}

dreal LineF::distance(const PointF &p0, const PointF &p1)
{
    dreal nX = p0.x() - p1.x();
    dreal nY = p0.y() - p1.y();
    return dmSqrt(nX*nX+nY*nY);
}

dreal LineF::distance(dreal nX0, dreal nY0, dreal nX1, dreal nY1)
{
    dreal nX = nX0 - nX1;
    dreal nY = nY0 - nY1;
    return dmSqrt(nX*nX+nY*nY);
}

dreal LineF::radian(const PointF &p0, const PointF &p1)
{
    return Atan2(p1.y()-p0.y(), p1.x()-p0.x());
}

dreal LineF::radian(dreal nX0, dreal nY0, dreal nX1, dreal nY1)
{
    return Atan2(nY1-nY0, nX1-nX0);
}

void LineF::getMathLine(dreal &a, dreal &b, dreal &c)
{
    if (IsEqual(mP1.x(), mP0.x())) {
        a = 1;
        b = 0;
        c = -mP1.x();
    }
    else if (IsEqual(mP1.y(), mP0.y())) {
        a = 0;
        b = 1;
        c = -mP1.y();
    }
    else {
        a = (mP1.y() - mP0.y()) / (mP1.x() - mP0.x());
        b = -1;
        c = -a*mP0.x() + mP0.y();
    }
}

dreal Pt2Pt(dreal x1, dreal y1, dreal x2, dreal y2)
{
    return dmSqrt( (dreal)( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) ) );
}

dreal DistPt2Line( dreal lx1, dreal ly1, dreal lx2, dreal ly2, dreal px, dreal py )
{
    double   abx   =   lx2   -   lx1;
    double   aby   =   ly2   -   ly1;
    double   acx   =   px   -   lx1;
    double   acy   =   py   -   ly1;
    double   f   =   abx*acx   +   aby*acy;
    if   (   f<0   )   return   Pt2Pt(lx1, ly1, px, py);

    double   d   =   abx*abx   +   aby*aby;
    if   (   f>d   )   return   Pt2Pt(lx2, ly2, px, py);

    //   D   =   a   +   [(ab*ac)/(ab*ab)]*ab
    f   /=   d;
    double   dx   =   lx1   +   f*abx;
    double   dy   =   ly1   +   f*aby;
    return   Pt2Pt(dx,   dy,   px,   py);
}

dbool LineF::hitLine( const PointF& pt, const PointF& ptStart, const PointF& ptEnd, int nNear )
{
    if( DistPt2Line(ptStart.x(), ptStart.y(), ptEnd.x(), ptEnd.y(), pt.x(), pt.y()) < (double)nNear )
        return true;
    else
        return false;
}

dbool LineF::hitLine( const PointF &pt, const LineF &line, int nNear)
{
    return hitLine(pt, line.mP0, line.mP1, nNear);
}

DM_END_NAMESPACE
